#ifdef INCLUDE_PTR_ALIAS_STUBS
#    include "CGLAutoPtrIsUniquePtr.h"
#    include "CGLArrayAutoPtrIsUniquePtr.h"
#else
#    include <memory>
#endif

#include <vector>
#include <string>
#include <set>

// let testMethod compile
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"

namespace {

[[maybe_unused]] const inline int testMethod(int a, int b)
{
    return a + b;
}

template<typename D> class PrivateDecryptionKeyImpl
{
public:
    void testdef(CGL::ArrayAutoPtr<int> a)
    {
        return;
    }
    void testdef2(CGL::AutoPtr<int> a)
    {
        return;
    }

private:
    CGL::AutoPtr<D>      m_pDecryptor;
    CGL::ArrayAutoPtr<D> m_pDecryptor_arr;
};

template<class H1> class DualHashing
{
    typedef CGL::AutoPtr<H1>               HashingType1P;
    typedef CGL::AutoPtr<std::vector<int>> TestDefVectorInt;
    typedef CGL::ArrayAutoPtr<H1>          HashingType1PArr;
    typedef CGL::ArrayAutoPtr<char>        TestDefCharArr;

public:
    virtual void GetDigest() const;

private:
    // autoptr
    HashingType1P             m_pFirstHashing;
    mutable CGL::AutoPtr<int> atest;
    CGL::AutoPtr<int>         btest;
    mutable CGL::AutoPtr<int> atest2 = std::make_unique<int>(5);

    // arrayautoptr
    HashingType1PArr               m_pFirstHashingarr;
    mutable CGL::ArrayAutoPtr<int> atestarr;
    CGL::ArrayAutoPtr<int>         btestarr;
    mutable CGL::ArrayAutoPtr<int> atest2arr = std::make_unique<int[]>(5);
};

template<class H1> inline void DualHashing<H1>::GetDigest() const
{
    {
        HashingType1P                  nochange;
        CGL::AutoPtr<int>              a;
        CGL::AutoPtr<std::vector<int>> b;
        std::unique_ptr<H1>            c;
        CGL::AutoPtr<H1>               d;

        // this won't rewritten but it could be a case
        // if so, a rule needs to be build
        // CGL::AutoPtr<CGL::AutoPtr<int>> c;

        CGL::AutoPtr<int> cc(std::make_unique<int>(5));
        CGL::AutoPtr<int> cccc = std::make_unique<int>(5);

        const CGL::AutoPtr<int> ac;
        const CGL::AutoPtr<int> acc(std::make_unique<int>(5));
        const CGL::AutoPtr<int> acccc = std::make_unique<int>(5);

        TestDefVectorInt aa;
        TestDefVectorInt aaa(std::make_unique<std::vector<int>>(5, 5));
        TestDefVectorInt aaaa(std::make_unique<std::vector<int>>(5, 5));
        TestDefVectorInt aaaaa = std::make_unique<std::vector<int>>(5, 5);
    }
    {
        HashingType1PArr                    nochange;
        CGL::ArrayAutoPtr<int>              a;
        CGL::ArrayAutoPtr<std::vector<int>> b;
        std::unique_ptr<H1[]>               c;
        CGL::ArrayAutoPtr<H1>               d;

        // this won't rewritten but it could be a case
        // if so, a rule needs to be build
        // CGL::ArrayAutoPtr<CGL::ArrayAutoPtr<int>> c;

        CGL::ArrayAutoPtr<char> cc(std::make_unique<char[]>(5));
        CGL::ArrayAutoPtr<char> cccc = std::make_unique<char[]>(7);

        const CGL::ArrayAutoPtr<int>  ac;
        const CGL::ArrayAutoPtr<char> acc(std::make_unique<char[]>(5));
        const CGL::ArrayAutoPtr<char> acccc = std::make_unique<char[]>(7);

        TestDefCharArr aa;
        TestDefCharArr aaa(std::make_unique<char[]>(5));
        TestDefCharArr aaaaa = std::make_unique<char[]>(7);
    }
}
typedef CGL::AutoPtr<int>      TestDefInt;
typedef CGL::ArrayAutoPtr<int> TestDefIntArr;

[[maybe_unused]] void testArgReplacement(CGL::AutoPtr<int> tbr1, CGL::AutoPtr<int> tbr2)
{
    return;
}

[[maybe_unused]] void testArgReplacement2(CGL::ArrayAutoPtr<int> tbr1, CGL::ArrayAutoPtr<int> tbr2)
{
    return;
}

[[maybe_unused]] void testArgReplacement3(TestDefInt tbr1, TestDefIntArr tbr2)
{
    return;
}

template<typename D> void TestCallsReplacements()
{
    {
        CGL::AutoPtr<int>              a;
        CGL::AutoPtr<D>                at;
        CGL::AutoPtr<std::vector<int>> b;
        std::unique_ptr<int>           c;
        std::unique_ptr<D>             ct;

        CGL::AutoPtr<int> cc(std::make_unique<int>(5));
        CGL::AutoPtr<int> cccc  = std::make_unique<int>(5);
        CGL::AutoPtr<D>   cccct = std::make_unique<D>(5);

        const CGL::AutoPtr<int> ac;
        const CGL::AutoPtr<int> acc(std::make_unique<int>(5));
        const CGL::AutoPtr<int> acccc  = std::make_unique<int>(5);
        const CGL::AutoPtr<D>   acccct = std::make_unique<D>(5);

        TestDefInt aa;
        TestDefInt aaa(std::make_unique<int>(5));
        TestDefInt aaaaa = std::make_unique<int>(5);
    }
    {
        CGL::ArrayAutoPtr<int> a;
        CGL::ArrayAutoPtr<D>   at;
        std::unique_ptr<int[]> c;
        std::unique_ptr<D[]>   ct;

        CGL::ArrayAutoPtr<int> cc(std::make_unique<int[]>(5));
        CGL::ArrayAutoPtr<int> cccc  = std::make_unique<int[]>(5);
        CGL::ArrayAutoPtr<D>   cccct = std::make_unique<D>(5);

        const CGL::ArrayAutoPtr<int> ac;
        const CGL::ArrayAutoPtr<int> acc(std::make_unique<int[]>(5));
        const CGL::ArrayAutoPtr<int> acccc  = std::make_unique<int[]>(5);
        const CGL::ArrayAutoPtr<D>   acccct = std::make_unique<D[]>(5);

        TestDefIntArr aa;
        TestDefIntArr aaa(std::make_unique<int[]>(5));
        TestDefIntArr aaaaa = std::make_unique<int[]>(5);
    }
#ifdef INCLUDE_PTR_ALIAS_STUBS
    using namespace CGL;
#endif
    {
        int*         test_pointer_int    = new int(5);
        std::string* test_pointer_string = new std::string("hello2");

        AutoPtr<std::vector<int>> a(std::make_unique<std::vector<int>>(5, 5));
        AutoPtr<std::string>      c(std::make_unique<std::string>("hello"));
        AutoPtr<int>              cc(std::make_unique<int>(5));
        AutoPtr<int>              ccc(std::make_unique<int>());
        AutoPtr<std::vector<int>> vv(std::make_unique<std::vector<int>>());
        AutoPtr<int>              d(nullptr);
        AutoPtr<int>              e(test_pointer_int);
        AutoPtr<std::string>      f(test_pointer_string);
        AutoPtr<int>              g(std::move(e));
        AutoPtr<std::string>      h(std::move(f));
        AutoPtr<std::vector<int>> i(std::move(a));
        AutoPtr<std::string>      j(std::move(h));
        AutoPtr<std::vector<int>> k(std::move(i));

        TestDefInt ii(std::make_unique<int>(5));
        TestDefInt iii(std::make_unique<int>());
        TestDefInt iiii(false ? std::make_unique<int>(5) : std::make_unique<int>());
    }
    {
        std::string test_array_s_vals[] = {"asdfa", "dsfas"};
        int         test_array_int[5];

        ArrayAutoPtr<std::set<std::string>> b(std::make_unique<std::set<std::string>[]>(3));
        ArrayAutoPtr<unsigned char>         d(std::make_unique<unsigned char[]>(testMethod(1, 2)));
        ArrayAutoPtr<int>                   f(test_array_int);
        ArrayAutoPtr<std::string>           i(test_array_s_vals);
        ArrayAutoPtr<std::string>           j(std::move(i));
        ArrayAutoPtr<std::set<std::string>> l(std::move(b));
        ArrayAutoPtr<std::string>           m(std::move(j));
        ArrayAutoPtr<std::set<std::string>> p(std::move(l));
        ArrayAutoPtr<int>                   y = std::make_unique<int[]>(5);
        ArrayAutoPtr<std::set<std::string>> z = std::make_unique<std::set<std::string>[]>(5);

        TestDefIntArr ii(std::make_unique<int[]>(5));
        TestDefIntArr iii(std::make_unique<int[]>(testMethod(1, 2)));
        TestDefIntArr iiii(false ? std::make_unique<int[]>(5) : std::make_unique<int[]>(2));
    }
}

}  // namespace
