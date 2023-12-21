// #define TEST_WITH_ARRAYAUTOPTR_BEING_AN_ALIAS_OF_STD_UNIQUE_PTR
#ifdef TEST_WITH_ARRAYAUTOPTR_BEING_AN_ALIAS_OF_STD_UNIQUE_PTR
#    include "CGLArrayAutoPtrIsUniquePtr.h"
#else
#    include "CGLArrayAutoPtrStub.h"
#endif

#include <memory>  // std::make_unique
#include <string>
#include <vector>
#include <set>

namespace {
const static int testMethod(int a, int b)
{
    return a + b;
}
template<class T> static CGL::ArrayAutoPtr<T> returnArrayAutoPtrStub()
{
    CGL::ArrayAutoPtr<T> x;
    return x;
}
struct TestStruct {
    int val;
    TestStruct() : val(3)
    {
    }
    int getVal()
    {
        return val;
    }
};

template<typename D> class PrivateDecryptionKeyImpl
{
public:
    PrivateDecryptionKeyImpl<D>(D x);

private:
    CGL::ArrayAutoPtr<D> m_pDecryptor;  // The object used to decrypt data.
};

template<typename D> PrivateDecryptionKeyImpl<D>::PrivateDecryptionKeyImpl(D source) : m_pDecryptor()
{
    try {
        // Create the decryption key
        m_pDecryptor = new D[source];
    } catch (...) {
        throw;
    }
}

template<class H1> class DualHashing
{
    typedef CGL::ArrayAutoPtr<H1> HashingType1P;

public:
    virtual bool GetDigest() const;

private:
    HashingType1P m_pFirstHashing;
};

template<class H1> inline bool DualHashing<H1>::GetDigest() const
{
    [[maybe_unused]] bool bb = m_pFirstHashing.IsNull();
    [[maybe_unused]] bool b  = !m_pFirstHashing.IsNull();
    H1                    a[3];
    std::unique_ptr<H1>   c;
    CGL::ArrayAutoPtr<H1> d;
    HashingType1P         e;
    m_pFirstHashing = c;
    m_pFirstHashing = d;
    m_pFirstHashing = e;
    m_pFirstHashing = a;
    return m_pFirstHashing.Get() != nullptr;
}

typedef CGL::ArrayAutoPtr<int> TestDefInt;

[[maybe_unused]] void TestCallsReplacements()
{
    // Namespaced Constructors
    {
        // test ArrayAutoPtr(Type* pi_pType = 0);
        CGL::ArrayAutoPtr<char>                  a(new char[3]);
        CGL::ArrayAutoPtr<std::set<std::string>> b(new std::set<std::string>[3]);
        CGL::ArrayAutoPtr<char>                  c(new char[std::max(1, 2)]);
        CGL::ArrayAutoPtr<unsigned char>         d(new unsigned char[testMethod(1, 2)]);
        CGL::ArrayAutoPtr<int>                   e(nullptr);

        // second setup, there should be no change
        int                    test_array_int[5];
        int                    test_array_int_vals[] = {1, 2, 3, 4};
        CGL::ArrayAutoPtr<int> f(test_array_int);
        CGL::ArrayAutoPtr<int> g(test_array_int_vals);

        std::string                    test_array_s[5];
        std::string                    test_array_s_vals[] = {"asdfa", "dsfas"};
        CGL::ArrayAutoPtr<std::string> h(test_array_s);
        CGL::ArrayAutoPtr<std::string> i(test_array_s_vals);

        // test ArrayAutoPtr(ArrayAutoPtr<Type>& pio_rArrayAutoPtr);
        CGL::ArrayAutoPtr<std::string>           j(i);
        CGL::ArrayAutoPtr<int>                   k(f);
        CGL::ArrayAutoPtr<std::set<std::string>> l(b);

        // test ArrayAutoPtr(ArrayAutoPtr<Type>&& pio_rArrayAutoPtr);
        // no change
        CGL::ArrayAutoPtr<std::string>           m(std::move(j));
        CGL::ArrayAutoPtr<int>                   n(std::move(k));
        CGL::ArrayAutoPtr<std::set<std::string>> p(std::move(l));

        // test ArrayAutoPtr(std::unique_ptr<Type[]> rhs)
        // no change
        CGL::ArrayAutoPtr<int>                   y = std::make_unique<int[]>(5);
        CGL::ArrayAutoPtr<std::set<std::string>> z = std::make_unique<std::set<std::string>[]>(5);
    }

    using namespace CGL;

    // Constructors
    {
        // test ArrayAutoPtr(Type* pi_pType = 0);
        ArrayAutoPtr<std::set<std::string>> b(new std::set<std::string>[3]);
        ArrayAutoPtr<unsigned char>         d(new unsigned char[testMethod(1, 2)]);

        // second setup, there should be no change
        int               test_array_int[5];
        ArrayAutoPtr<int> f(test_array_int);

        std::string               test_array_s_vals[] = {"asdfa", "dsfas"};
        ArrayAutoPtr<std::string> i(test_array_s_vals);

        // test ArrayAutoPtr(ArrayAutoPtr<Type>& pio_rArrayAutoPtr);
        ArrayAutoPtr<std::string>                j(i);
        CGL::ArrayAutoPtr<std::set<std::string>> l(b);

        // test ArrayAutoPtr(ArrayAutoPtr<Type>&& pio_rArrayAutoPtr);
        // no change
        ArrayAutoPtr<std::string>           m(std::move(j));
        ArrayAutoPtr<std::set<std::string>> p(std::move(l));

        // test ArrayAutoPtr(std::unique_ptr<Type[]> rhs)
        // no change
        ArrayAutoPtr<int>                   y = std::make_unique<int[]>(5);
        ArrayAutoPtr<std::set<std::string>> z = std::make_unique<std::set<std::string>[]>(5);
    }

    {
        TestDefInt i(new int[5]);
        TestDefInt ii(new int[testMethod(1, 2)]);
        TestDefInt iii(false ? new int[5] : new int[2]);
        int*       p = new int[1];
        int*       y = new int[5];
        TestDefInt iiii(p);
        i.reset(p);

        TestDefInt iiiii(i.IsNull() ? i : ii);

        i = ii;
        i = ii.IsNull() ? i : ii;

        i = p;
        i = false ? p : y;

        i = new int[5];
        i = new int[testMethod(1, 2)];
        i = false ? new int[5] : new int[testMethod(1, 2)];
        i = i.IsNull() ? nullptr : new int[5];

        i.Release();
        i.Get();
        [[maybe_unused]] bool tt = i.IsNull();
        [[maybe_unused]] bool t  = !i.IsNull();
    }

    ArrayAutoPtr<std::set<std::string>>              stubSet(nullptr);
    ArrayAutoPtr<std::string>                        stubString(nullptr);
    ArrayAutoPtr<int>                                stubInt(nullptr);
    ArrayAutoPtr<std::set<std::string>>              stubSet2(nullptr);
    ArrayAutoPtr<std::string>                        stubString2(nullptr);
    ArrayAutoPtr<int>                                stubInt2(nullptr);
    std::vector<ArrayAutoPtr<int>>                   stubIntVector(1);
    std::vector<ArrayAutoPtr<std::set<std::string>>> stubSetVector(1);
    std::vector<ArrayAutoPtr<std::string>>           stubStringVector(1);

    // Assignment Operators
    {
        // test ArrayAutoPtr<Type>& operator=(Type* pi_pType)
        stubSet    = new std::set<std::string>[3];
        stubString = new std::string[5];
        stubInt    = new int[5];

        // second setup, should use reset
        int test_array_int[5];
        int test_array_int_vals[] = {1, 2, 3, 4};
        stubInt                   = test_array_int;
        stubInt                   = test_array_int_vals;

        // test ArrayAutoPtr<Type>& operator=(ArrayAutoPtr<Type>& pio_rArrayAutoPtr)
        stubSet    = stubSet2;
        stubString = stubString2;
        stubInt    = stubInt2;

        // test ArrayAutoPtr<Type>& operator=(ArrayAutoPtr<Type>&& pio_rArrayAutoPtr)
        // no change
        stubSet    = std::move(stubSet2);
        stubString = std::move(stubString2);
        stubInt    = std::move(stubInt2);

        // test ArrayAutoPtr<Type>& operator=(std::unique_ptr<Type[]> rhs)
        // no change
        stubInt    = std::make_unique<int[]>(5);
        stubString = std::make_unique<std::string[]>(5);
        stubSet    = std::make_unique<std::set<std::string>[]>(5);
    }

    // Methods
    {
        // Type* Release();
        stubSet.Release();
        stubInt.Release();
        stubString.Release();
        stubStringVector[0].Release();
        stubIntVector[0].Release();
        stubSetVector[0].Release();

        stubSet.release();
        stubInt.release();
        stubString.release();
        stubStringVector[0].release();
        stubIntVector[0].release();
        stubSetVector[0].release();

        // Type* Get() const;
        stubSet.Get();
        stubInt.Get();
        stubString.Get();
        stubStringVector[0].Get();
        stubIntVector[0].Get();
        stubSetVector[0].Get();

        stubSet.get();
        stubInt.get();
        stubString.get();
        stubStringVector[0].get();
        stubIntVector[0].get();
        stubSetVector[0].get();

        // Type& operator[](const size_t pi_Index) const
        [[maybe_unused]] std::set<std::string> aa  = stubSet[0];
        [[maybe_unused]] int                   ii  = stubInt[1];
        [[maybe_unused]] std::string           ss  = stubString[0];
        [[maybe_unused]] std::string           sss = stubStringVector[0][0];

        // Type& operator*() const
        [[maybe_unused]] std::set<std::string> aaa = *stubSet;

        [[maybe_unused]] int         a     = *(returnArrayAutoPtrStub<int>());
        [[maybe_unused]] int         iii   = *stubInt;
        [[maybe_unused]] std::string ssss  = *stubString;
        [[maybe_unused]] std::string sssss = *(stubStringVector[0]);

        // Type* operator->() const
        ArrayAutoPtr<TestStruct> ts = std::make_unique<TestStruct[]>(1);
        ts->getVal();
        std::vector<ArrayAutoPtr<TestStruct>> tsVector(1);
        tsVector[0]->getVal();

        [[maybe_unused]] int b = returnArrayAutoPtrStub<TestStruct>()->getVal();

        // bool IsNull() const;
        [[maybe_unused]] bool isNullI      = stubInt.IsNull();
        [[maybe_unused]] bool isNullS      = stubSet.IsNull();
        [[maybe_unused]] bool isNullString = stubString.IsNull();
        isNullI                            = !stubInt.IsNull();
        isNullS                            = !stubSet.IsNull();

        [[maybe_unused]] bool isNullIv      = stubIntVector[0].IsNull();
        [[maybe_unused]] bool isNullSv      = stubSetVector[0].IsNull();
        [[maybe_unused]] bool isNullStringv = stubStringVector[0].IsNull();
        isNullStringv                       = !stubStringVector[0].IsNull();
        isNullSv                            = !stubSetVector[0].IsNull();

        // swap(CGL::ArrayAutoPtr<T>& p_LHS, CGL::ArrayAutoPtr<T>& p_RHS)
        stubSet.swap(stubSet2);
        stubInt.swap(stubInt2);
        stubSet.swap(stubSet2);
    }
}

}  // namespace
