#ifndef CXX_UTILS_STATIC_SINGLETON_HPP_
#define CXX_UTILS_STATIC_SINGLETON_HPP_

#include <CxxUtils/defines.hpp>
#include <CxxUtils/singleton.hpp>

#include <cassert>
#include <memory>
#include <mutex>
#include <type_traits>

CXX_UTILS_DECL_START_

struct StaticSingletonHelper {
    virtual ~StaticSingletonHelper() = default;
    void Lock() { _mutex.lock(); }
    void Unlock() { _mutex.unlock(); }

    protected:
    // Note: protected constructor -> class is not able to instantiate outside of child private methods
    StaticSingletonHelper() = default;

    /* Instance fields */
    std::mutex _mutex{};
};

template <typename T>
concept DerivedFromHelper = std::is_base_of_v<StaticSingletonHelper, T>;

template <typename T>
    requires DerivedFromHelper<T>
struct StaticSingleton {
    // ------------------------------------
    // Static Accessors and Utilities
    // ------------------------------------

    static T &GetInstance()
    {
        assert(_is_instance_inited && "Not inited Singleton instance!");
        return *reinterpret_cast<T *>(_instance_memory);
    }

    static bool IsInited() { return _is_instance_inited; }

    static void DeleteInstance()
    {
        GetInstance().~T();
        _is_instance_inited = false;
    }

    template <typename... Args>
    static T &InitInstance(Args &&...args)
    {
        assert(!IsInited());
        auto ptr = new (_instance_memory) T(std::forward<Args>(args)...);
        assert(ptr == reinterpret_cast<T *>(_instance_memory));

        _is_instance_inited = true;
        return GetInstance();
    }

    protected:
    /* static memory */
    static bool _is_instance_inited;
    alignas(alignof(T)) static unsigned char _instance_memory[sizeof(T) + alignof(T)];
};

template <typename T>
    requires DerivedFromHelper<T>
bool StaticSingleton<T>::_is_instance_inited = false;

template <typename T>
    requires DerivedFromHelper<T>
unsigned char StaticSingleton<T>::_instance_memory[sizeof(T) + alignof(T)]{};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_STATIC_SINGLETON_HPP_
