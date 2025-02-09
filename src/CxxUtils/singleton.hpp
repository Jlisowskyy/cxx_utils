#ifndef CXX_UTILS_SINGLETON_HPP_
#define CXX_UTILS_SINGLETON_HPP_

#include <CxxUtils/defines.hpp>

#include <cassert>
#include <memory>
#include <mutex>

CXX_UTILS_DECL_START_

template <typename T>
struct Singleton {
    virtual ~Singleton() = default;

    // Access the instance, NOTE: _instance needs to be initialized before usage
    FAST_CALL_ static T &GetInstance()
    {
        assert(_instance != nullptr && "Not inited Singleton instance!");
        return *_instance;
    }

    FAST_CALL_ static bool IsInited() { return _instance != nullptr; }

    FAST_CALL_ static void DeleteInstance()
    {
        delete _instance;
        _instance = nullptr;
    }

    WRAP_CALL_ void Lock() { _mutex.lock(); }
    WRAP_CALL_ void Unlock() { _mutex.unlock(); }

    protected:
    // Note: Should be used in some deriving class init api.
    FAST_CALL_ static void InitInstance(T *readyInstance) { _instance = readyInstance; }

    // Note: protected constructor -> class is not able to instantiate outside of child private methods
    Singleton() = default;

    static T *_instance;
    std::mutex _mutex{};
};

template <typename T>
T *Singleton<T>::_instance{};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_SINGLETON_HPP_
