#include <memory>

namespace CGL {

template<typename Type> class AutoPtr
{
public:
    explicit AutoPtr(Type* pi_pType = 0)
    {
    }
    AutoPtr(AutoPtr<Type>& pio_rAutoPtr)
    {
    }
    AutoPtr(AutoPtr<Type>&& pio_rAutoPtr)
    {
    }
    AutoPtr<Type>& operator=(Type* pi_pType)
    {
        return *this;
    }
    AutoPtr<Type>& operator=(AutoPtr<Type>& pio_rAutoPtr)
    {
        return *this;
    }
    AutoPtr<Type>& operator=(AutoPtr<Type>&& pio_rAutoPtr)
    {
        return *this;
    }
    ~AutoPtr()
    {
    }

    Type& operator*() const
    {
        static Type x{};
        return x;
    }
    Type* operator->() const
    {
        return nullptr;
    }
    Type* Get() const
    {
        return nullptr;
    }
    Type* Release()
    {
        return nullptr;
    }
    bool IsNull() const
    {
        return true;
    }

    void swap(AutoPtr<Type>& p_LHS)
    {
    }

    // Methods to assist with migration from AutoPtr to unique_ptr
    Type* release() noexcept
    {
        return nullptr;
    };
    Type* get() const noexcept
    {
        return nullptr;
    };
    void reset(Type* pi_pType = 0) noexcept
    {
    }
    AutoPtr(std::unique_ptr<Type> rhs)
    {
    }
    AutoPtr<Type>& operator=(std::unique_ptr<Type[]> rhs)
    {
        return *this;
    }
};

}  // namespace CGL

namespace std {

template<typename T> void swap(CGL::AutoPtr<T>& p_LHS, CGL::AutoPtr<T>& p_RHS)
{
}
}  // namespace std
