// #define TEST_WITH_AUTOPTR_BEING_AN_ALIAS_OF_STD_UNIQUE_PTR
#ifdef TEST_WITH_AUTOPTR_BEING_AN_ALIAS_OF_STD_UNIQUE_PTR
#    include "CGLAutoPtrIsUniquePtr.h"
#else
#    include "CGLAutoPtrStub.h"
#endif

#include <memory>  // std::make_unique
#include <string>
#include <vector>
#include <set>
#include <limits>

namespace Merlin {
class ExpressionsCache
{
public:
    template<typename Duration> ExpressionsCache(std::string p_FileName, size_t p_MaximumCacheSize, const Duration& p_ExpressionTTL){};
    void Load(){};
};
}  // namespace Merlin

namespace {

template<typename D> class PrivateDecryptionKeyImpl
{
public:
    PrivateDecryptionKeyImpl<D>(D x);

private:
    CGL::AutoPtr<D> m_pDecryptor;  // The object used to decrypt data.
};

template<typename D> PrivateDecryptionKeyImpl<D>::PrivateDecryptionKeyImpl(D source) : m_pDecryptor()
{
    try {
        // Create the decryption key
        m_pDecryptor = new D(source);
    } catch (...) {
        throw;
    }
}

template<class H1> class DualHashing
{
    typedef CGL::AutoPtr<H1> HashingType1P;

public:
    virtual bool GetDigest() const;

private:
    HashingType1P m_pFirstHashing;
};

template<class H1> inline bool DualHashing<H1>::GetDigest() const
{
    [[maybe_unused]] bool bb = m_pFirstHashing.IsNull();
    [[maybe_unused]] bool b  = !m_pFirstHashing.IsNull();
    H1                    a  = new H1;
    std::unique_ptr<H1>   c;
    CGL::AutoPtr<H1>      d;
    HashingType1P         e;
    m_pFirstHashing = c;
    m_pFirstHashing = d;
    m_pFirstHashing = e;
    m_pFirstHashing = a;
    return m_pFirstHashing.Get() != nullptr;
}

typedef CGL::AutoPtr<int> TestDefInt;

[[maybe_unused]] void TestCallsReplacements()
{
    CGL::AutoPtr<Merlin::ExpressionsCache> apCache;
    apCache = new Merlin::ExpressionsCache("dfs", std::numeric_limits<uint32_t>::max(), 1);
    apCache->Load();
    // Constructors
    {
        // test AutoPtr(Type* pi_pType = 0);
        CGL::AutoPtr<std::vector<int>> a(new std::vector<int>(5, 5));
        CGL::AutoPtr<std::set<int>>    b(new std::set<int>());
        CGL::AutoPtr<std::string>      c(new std::string("hello"));
        CGL::AutoPtr<int>              cc(new int(5));

        CGL::AutoPtr<int> d(nullptr);

        // second setup, there should be no change
        int*                      test_pointer_int = new int(5);
        CGL::AutoPtr<int>         e(test_pointer_int);
        std::string*              test_pointer_string = new std::string("hello2");
        CGL::AutoPtr<std::string> f(test_pointer_string);

        // test AutoPtr(AutoPtr<Type>& pio_rAutoPtr);
        CGL::AutoPtr<int>              g(e);
        CGL::AutoPtr<std::string>      h(f);
        CGL::AutoPtr<std::vector<int>> i(a);

        // test AutoPtr(AutoPtr<Type>&& pio_rAutoPtr);
        // no change
        CGL::AutoPtr<std::string>      j(std::move(h));
        CGL::AutoPtr<std::vector<int>> k(std::move(i));
    }

    using namespace CGL;

    AutoPtr<std::vector<int>>              stubVector(nullptr);
    AutoPtr<std::set<int>>                 stubSet(nullptr);
    AutoPtr<std::string>                   stubString(nullptr);
    AutoPtr<int>                           stubInt(nullptr);
    std::vector<AutoPtr<int>>              stubIntVector(1);
    std::vector<AutoPtr<std::set<int>>>    stubSetVector(1);
    std::vector<AutoPtr<std::vector<int>>> stubVectorVector(1);
    std::vector<AutoPtr<std::string>>      stubStringVector(1);

    // Constructors
    {
        // test AutoPtr(Type* pi_pType = 0);
        AutoPtr<std::vector<int>> a(new std::vector<int>(5, 5));
        AutoPtr<std::set<int>>    b(new std::set<int>());
        AutoPtr<std::string>      c(new std::string("hello"));
        AutoPtr<int>              cc(new int(5));
        AutoPtr<int>              ccc(new int);
        AutoPtr<std::vector<int>> vv(new std::vector<int>);

        AutoPtr<int> d(nullptr);

        // second setup, there should be no change
        int*                 test_pointer_int = new int(5);
        AutoPtr<int>         e(test_pointer_int);
        std::string*         test_pointer_string = new std::string("hello2");
        AutoPtr<std::string> f(test_pointer_string);

        // test AutoPtr(AutoPtr<Type>& pio_rAutoPtr);
        AutoPtr<int>              g(e);
        AutoPtr<std::string>      h(f);
        AutoPtr<std::vector<int>> i(a);

        // test AutoPtr(AutoPtr<Type>&& pio_rAutoPtr);
        // no change
        AutoPtr<std::string>      j(std::move(h));
        AutoPtr<std::vector<int>> k(std::move(i));
    }

    {
        TestDefInt i(new int(5));
        TestDefInt ii(new int);
        TestDefInt iii(false ? new int(5) : new int);
        int*       p = new int;
        int*       y = new int;
        TestDefInt iiii(p);
        i.reset(p);

        TestDefInt iiiii(i.IsNull() ? i : ii);

        i = ii;
        i = ii.IsNull() ? i : ii;

        i = p;
        i = false ? p : y;

        i = new int(5);
        i = new int;
        i = false ? new int(5) : new int;
        i = i.IsNull() ? nullptr : new int;

        i.Release();
        i.Get();
        [[maybe_unused]] bool tt = i.IsNull();
        [[maybe_unused]] bool t  = !i.IsNull();
    }

    // Assignment Operators
    {
        // test AutoPtr<Type>& operator=(Type* pi_pType);
        stubVector = new std::vector<int>(5, 5);
        stubVector = new std::vector<int>;
        stubSet    = new std::set<int>();
        stubInt    = new int(5);
        stubInt    = new int;
        stubString = new std::string("test");

        // second setup, should use reset
        int* test_pointer_int            = new int(5);
        stubInt                          = test_pointer_int;
        std::string* test_pointer_string = new std::string("hello2");
        stubString                       = test_pointer_string;

        // test AutoPtr<Type>& operator=(AutoPtr<Type>& pio_rAutoPtr);
        AutoPtr<std::vector<int>> stubVector2(nullptr);
        AutoPtr<std::set<int>>    stubSet2(nullptr);
        AutoPtr<std::string>      stubString2(nullptr);
        AutoPtr<int>              stubInt2(nullptr);

        stubVector = stubVector2;
        stubSet    = stubSet2;
        stubString = stubString2;
        stubInt    = stubInt2;

        // test AutoPtr<Type>& operator=(AutoPtr<Type>&& pio_rAutoPtr);
        // no change
        stubVector = std::move(stubVector2);
        stubSet    = std::move(stubSet2);
        stubString = std::move(stubString2);
        stubInt    = std::move(stubInt2);
    }

    // Methods
    {
        // Type* Release();
        stubVector.Release();
        stubInt.Release();
        stubSet.Release();
        stubString.Release();

        stubStringVector[0].Release();
        stubVectorVector[0].Release();
        stubSetVector[0].Release();
        stubIntVector[0].Release();

        stubVector.release();
        stubInt.release();
        stubSet.release();
        stubString.release();

        // Type* Get() const;
        stubVector.Get();
        stubInt.Get();
        stubSet.Get();
        stubString.Get();

        stubStringVector[0].Get();
        stubVectorVector[0].Get();
        stubSetVector[0].Get();
        stubIntVector[0].Get();

        stubVector.get();
        stubInt.get();
        stubSet.get();
        stubString.get();

        // bool IsNull() const;
        [[maybe_unused]] bool isNullV      = stubVector.IsNull();
        [[maybe_unused]] bool isNullI      = stubInt.IsNull();
        [[maybe_unused]] bool isNullS      = stubSet.IsNull();
        [[maybe_unused]] bool isNullString = stubString.IsNull();

        isNullV = !stubVector.IsNull();
        isNullI = !stubInt.IsNull();

        isNullV      = stubStringVector[0].IsNull();
        isNullI      = stubVectorVector[0].IsNull();
        isNullS      = stubSetVector[0].IsNull();
        isNullString = stubIntVector[0].IsNull();

        // void swap(AutoPtr<Type>& p_LHS);
        AutoPtr<std::vector<int>> stubVector2(nullptr);
        AutoPtr<std::set<int>>    stubSet2(nullptr);
        AutoPtr<std::string>      stubString2(nullptr);
        AutoPtr<int>              stubInt2(nullptr);

        stubVector.swap(stubVector2);
        stubInt.swap(stubInt2);
        stubSet.swap(stubSet2);
        stubString.swap(stubString2);
    }
}

}  // namespace
