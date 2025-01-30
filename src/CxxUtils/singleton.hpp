#ifndef CXX_UTILS_SINGLETON_HPP_
#define CXX_UTILS_SINGLETON_HPP_

#include <CxxUtils/defines.hpp>

CXX_UTILS_DECL_START_

template <typename T>
struct Singleton {
    ~Singleton() { delete _instance; }

    // Access the instance, NOTE: _instance needs to be initialized before usage
    static T &GetInstance()
    {
        assert(_instance != nullptr && "Not inited Singleton instance!");
        return *_instance;
    }

    static bool IsInited() { return _instance != nullptr; }

    static void DeleteInstance()
    {
        delete _instance;
        _instance = nullptr;
    }

    protected:
    // Note: Should be used in some deriving class init api.
    static void InitInstance(T *readyInstance) { _instance = readyInstance; }

    // Note: protected constructor -> class is not able to instantiate outside of child private methods
    Singleton() = default;

    static T *_instance;
};

template <typename T>
T *Singleton<T>::_instance{};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_SINGLETON_HPP_
