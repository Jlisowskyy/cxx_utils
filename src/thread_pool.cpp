#include <CxxUtils/thread_pool.hpp>

#include <cassert>
#include <memory>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

#include <CxxUtils/data_types/multi_vector.hpp>
#include <CxxUtils/type_checks.hpp>

#include <CxxUtils/data_types/extended_map.hpp>
#include <CxxUtils/data_types/extended_vector.hpp>
#include <CxxUtils/data_types/listeners.hpp>
#include <CxxUtils/data_types/multi_vector.hpp>
#include <CxxUtils/template_helpers.hpp>

std::vector<std::shared_ptr<std::thread>> CxxUtils::ThreadPool::m_globalThreads{};
volatile std::atomic<bool> CxxUtils::ThreadPool::m_shouldStop{false};
CxxUtils::ThreadPool::sem_t CxxUtils::ThreadPool::m_globalJobsSemaphore{0};
std::mutex CxxUtils::ThreadPool::m_globalMutex{};
std::vector<std::unique_ptr<CxxUtils::ThreadPool::_taskBase>> CxxUtils::ThreadPool::m_globalTasks{};

void CxxUtils::ThreadPool::Wait()
{
    assert(!m_wasWaited && "Detected double wait on thread pool");
    assert(m_wasRun && "Detected wait without run on thread pool");

    m_wasWaited = true;

    for (uint32_t retry = 0; retry < m_numThreads; ++retry) {
        m_localJobsSemaphore.acquire();
    }
}

void CxxUtils::ThreadPool::Reset(const uint32_t numThreads)
{
    assert(m_wasRun && m_wasWaited && "Detected reset without wait on thread pool");

    if (numThreads > m_numThreads) {
        _tryExtendingGlobalWorkers(numThreads);
    }

    m_numThreads = numThreads;
    m_wasRun     = false;
    m_wasWaited  = false;
}

void CxxUtils::ThreadPool::_tryExtendingGlobalWorkers(const size_t num_workers)
{
    std::lock_guard lock(m_globalMutex);

    if (m_globalThreads.size() < num_workers) {
        _addGlobalWorkers(num_workers - m_globalThreads.size());
    }
}

void CxxUtils::ThreadPool::_addGlobalWorkers(size_t num_workers)
{
    while (num_workers-- > 0) {
        m_globalThreads.emplace_back(new std::thread(_globalWorkerThread), [](std::thread *pThread) {
            /* allows automatic cleanup of threads on program stop */
            m_shouldStop.store(true);
            m_globalJobsSemaphore.release(static_cast<ptrdiff_t>(m_globalThreads.size()));
            pThread->join();
            delete pThread;
        });
    }
}

void CxxUtils::ThreadPool::_globalWorkerThread()
{
    while (!m_shouldStop) {
        m_globalJobsSemaphore.acquire();

        /* perform sanity check */
        if (m_shouldStop) {
            break;
        }

        /* load task */
        std::unique_ptr<_taskBase> task;
        {
            std::lock_guard lock(m_globalMutex);
            assert(!m_globalTasks.empty());

            task = std::move(m_globalTasks.back());
            m_globalTasks.pop_back();
        }

        /* run task */
        assert(task);
        task->Run();
    }
}
