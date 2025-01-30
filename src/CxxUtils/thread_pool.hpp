#ifndef CXX_UTILS_THREAD_POOL_HPP_
#define CXX_UTILS_THREAD_POOL_HPP_

#include <cassert>
#include <memory>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

#include <CxxUtils/defines.hpp>

CXX_UTILS_DECL_START_

class ThreadPool
{
    // ------------------------------
    // Internal types
    // ------------------------------

    using sem_t = std::counting_semaphore<UINT16_MAX>;

    struct _taskBase {
        explicit _taskBase(const uint32_t thread_idx, sem_t *sem) : m_threadIdx(thread_idx), m_sem(sem) {}

        virtual ~_taskBase() = default;

        void Run()
        {
            _run(m_threadIdx);
            m_sem->release();
        }

        protected:
        virtual void _run(uint32_t thread_idx) = 0;

        uint32_t m_threadIdx;
        sem_t *m_sem;
    };

    template <class FuncT, class... Args>
    struct _task final : _taskBase {
        FuncT m_func;
        std::tuple<Args...> m_args;

        _task(sem_t *sem, const uint32_t thread_idx, FuncT &&func, Args &&...args)
            : _taskBase(thread_idx, sem), m_func(std::forward<FuncT>(func)), m_args(std::forward<Args>(args)...)
        {
        }

        void _run(uint32_t thread_idx) override
        {
            std::apply(m_func, std::tuple_cat(std::make_tuple(thread_idx), m_args));
        }
    };

    public:
    // ------------------------------
    // Class creation
    // ------------------------------

    explicit ThreadPool(const uint32_t numThreads) : m_localJobsSemaphore(0), m_numThreads(numThreads)
    {
        /* extend global thread pool if needed */
        _tryExtendingGlobalWorkers(numThreads);
    }

    ~ThreadPool() { assert(m_wasWaited && "Detected early destruction of thread pool"); }

    // ------------------------------
    // Class interaction
    // ------------------------------

    template <class FuncT, class... Args>
    void RunThreads(FuncT &&func, Args &&...args)
    {
        assert(!m_wasRun && "Detected double run on thread pool");

        for (uint32_t thread_idx = 0; thread_idx < m_numThreads; ++thread_idx) {
            std::lock_guard lock(m_globalMutex);

            m_globalTasks.emplace_back(std::make_unique<_task<FuncT, Args...> >(
                &m_localJobsSemaphore, thread_idx, std::forward<FuncT>(func), std::forward<Args>(args)...
            ));
        }

        m_globalJobsSemaphore.release(m_numThreads);
        m_wasRun = true;
    }

    void Wait();

    void Reset() { Reset(m_numThreads); }

    void Reset(const uint32_t numThreads);

    // ------------------------------------------
    // static global thread pool management
    // ------------------------------------------

    static void InitGlobalWorkers() { _tryExtendingGlobalWorkers(std::thread::hardware_concurrency()); }

    protected:
    static void _tryExtendingGlobalWorkers(const size_t num_workers);

    static void _addGlobalWorkers(size_t num_workers);

    static void _globalWorkerThread();

    // ------------------------------
    // Class fields
    // ------------------------------

    static std::vector<std::shared_ptr<std::thread> > m_globalThreads;
    static volatile std::atomic<bool> m_shouldStop;
    static sem_t m_globalJobsSemaphore;
    static std::mutex m_globalMutex;
    static std::vector<std::unique_ptr<_taskBase> > m_globalTasks;

    sem_t m_localJobsSemaphore;
    uint32_t m_numThreads;
    bool m_wasWaited = false;
    bool m_wasRun    = false;
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_THREAD_POOL_HPP_
