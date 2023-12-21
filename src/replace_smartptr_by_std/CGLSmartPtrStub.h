#include <memory>

namespace CGL {

template<class T> class SmartPtr
{
    template<typename> friend class SmartPtr;  // friend self for base class conversions
public:
    SmartPtr(T* p_pObject = nullptr, bool p_InitPointer = true)
    {
    }
    SmartPtr(const SmartPtr<T>& p_Object)
    {
    }
    template<typename U> SmartPtr(const SmartPtr<U>& p_Object)
    {
    }
    SmartPtr(SmartPtr<T>&& p_Object)
    {
    }
    template<typename U> SmartPtr(SmartPtr<U>&& p_Object)
    {
    }
    ~SmartPtr()
    {
    }

    SmartPtr<T>& operator=(T* p_pObject)
    {
        return *this;
    }
    SmartPtr<T>& operator=(const SmartPtr<T>& p_Object)
    {
        return *this;
    }
    template<typename U> SmartPtr<T>& operator=(const SmartPtr<U>& p_Object)
    {
        return *this;
    }
    SmartPtr<T>& operator=(SmartPtr<T>&& p_Object)
    {
        return *this;
    }
    template<typename U> SmartPtr<T>& operator=(SmartPtr<U>&& p_Object)
    {
        return *this;
    }
    void Swap(SmartPtr<T>& p_rObject)
    {
    }

    T* GetPtr() const
    {
        return nullptr;
    }
    T& operator*() const
    {
        static T x{};
        return x;
    }
    T* operator->() const
    {
        return nullptr;
    }
    operator T*() const
    {
        return nullptr;
    }

    bool IsNull() const
    {
        return true;
    }

    bool operator==(const SmartPtr<T>& p_Object) const
    {
        return false;
    }
    template<typename U> bool operator==(const U* p_pObject) const
    {
        return false;
    }
    template<typename U> bool operator==(U* p_pObject) const
    {
        return false;
    }
    bool operator==(const decltype(nullptr)* p_pObject) const
    {
        return true;
    }
    bool operator!=(const SmartPtr<T>& p_Object) const
    {
        return false;
    }
    template<typename U> bool operator!=(const U* p_pObject) const
    {
        return false;
    }
    template<typename U> bool operator!=(U* p_pObject) const
    {
        return false;
    }
    bool operator!=(const decltype(nullptr)* p_pObject) const
    {
        return true;
    }
    bool operator<(const SmartPtr<T>& p_Object) const
    {
        return false;
    }
    template<typename U> bool operator<(const U* p_pObject) const
    {
        return false;
    }
    template<typename U> bool operator<(U* p_pObject) const
    {
        return false;
    }

    // Methods to assist with migration from SmartPtr to shared_pointer
    T* get() const noexcept
    {
        return nullptr;
    }
    template<typename U> void reset(U* pi_pType = 0) noexcept
    {
    }
    void reset() noexcept
    {
    }
    void swap(SmartPtr<T>& r) noexcept
    {
    }
};

template<typename T, typename... Args> SmartPtr<T> MakeSmartPtr(Args... args)
{
    return SmartPtr<T>(new T(std::forward<Args>(args)...));  // NOLINT
}

}  // namespace CGL

namespace std {

// std::swap specialization for SmartPtrs
template<typename T> void swap(CGL::SmartPtr<T>& p_LHS, CGL::SmartPtr<T>& p_RHS){};

}  // namespace std
