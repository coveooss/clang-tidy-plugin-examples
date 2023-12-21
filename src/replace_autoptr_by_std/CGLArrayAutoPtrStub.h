#include <cstddef>
#include <memory>

namespace CGL {

template<typename Type> class ArrayAutoPtr
{
public:
    explicit ArrayAutoPtr(Type* pi_pType = 0)
    {
    }
    ArrayAutoPtr(ArrayAutoPtr<Type>& pio_rArrayAutoPtr)
    {
    }
    ArrayAutoPtr(ArrayAutoPtr<Type>&& pio_rArrayAutoPtr)
    {
    }
    ArrayAutoPtr<Type>& operator=(Type* pi_pType)
    {
        return *this;
    }
    ArrayAutoPtr<Type>& operator=(ArrayAutoPtr<Type>& pio_rArrayAutoPtr)
    {
        return *this;
    }
    ArrayAutoPtr<Type>& operator=(ArrayAutoPtr<Type>&& pio_rArrayAutoPtr)
    {
        return *this;
    }
    ~ArrayAutoPtr()
    {
    }

    const Type& operator[](const size_t pi_Index) const
    {
        static const Type x{};
        return x;
    }
    Type& operator[](const size_t pi_Index)
    {
        static Type x{};
        return x;
    }

    Type& operator*() const
    {
        static Type x{};
        return x;
    }
    Type* operator->() const
    {
        static Type x{};
        return &x;
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

    void swap(ArrayAutoPtr<Type>& p_LHS)
    {
    }

    // Methods to assist with migration from ArrayAutoPtr to unique_ptr
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
    ArrayAutoPtr(std::unique_ptr<Type[]> rhs)
    {
    }
    ArrayAutoPtr<Type>& operator=(std::unique_ptr<Type[]> rhs)
    {
        return *this;
    }
};

}  // namespace CGL

namespace std {

template<typename T> void swap(CGL::ArrayAutoPtr<T>& p_LHS, CGL::ArrayAutoPtr<T>& p_RHS)
{
}
}  // namespace std
