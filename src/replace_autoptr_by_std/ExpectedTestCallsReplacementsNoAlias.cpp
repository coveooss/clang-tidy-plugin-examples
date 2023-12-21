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
    void testdef(std::unique_ptr<int[]> a)
    {
        return;
    }
    void testdef2(std::unique_ptr<int> a)
    {
        return;
    }

private:
    std::unique_ptr<D>   m_pDecryptor;
    std::unique_ptr<D[]> m_pDecryptor_arr;
};

template<class H1> class DualHashing
{
    typedef std::unique_ptr<H1>               HashingType1P;
    typedef std::unique_ptr<std::vector<int>> TestDefVectorInt;
    typedef std::unique_ptr<H1[]>             HashingType1PArr;
    typedef std::unique_ptr<char[]>           TestDefCharArr;

public:
    virtual void GetDigest() const;

private:
    // autoptr
    HashingType1P                m_pFirstHashing;
    mutable std::unique_ptr<int> atest;
    std::unique_ptr<int>         btest;
    mutable std::unique_ptr<int> atest2 = std::make_unique<int>(5);

    // arrayautoptr
    HashingType1PArr               m_pFirstHashingarr;
    mutable std::unique_ptr<int[]> atestarr;
    std::unique_ptr<int[]>         btestarr;
    mutable std::unique_ptr<int[]> atest2arr = std::make_unique<int[]>(5);
};

template<class H1> inline void DualHashing<H1>::GetDigest() const
{
    {
        HashingType1P                     nochange;
        std::unique_ptr<int>              a;
        std::unique_ptr<std::vector<int>> b;
        std::unique_ptr<H1>               c;
        std::unique_ptr<H1>               d;

        // this won't rewritten but it could be a case
        // if so, a rule needs to be build
        // CGL::AutoPtr<CGL::AutoPtr<int>> c;

        std::unique_ptr<int> cc(std::make_unique<int>(5));
        std::unique_ptr<int> cccc = std::make_unique<int>(5);

        const std::unique_ptr<int> ac;
        const std::unique_ptr<int> acc(std::make_unique<int>(5));
        const std::unique_ptr<int> acccc = std::make_unique<int>(5);

        TestDefVectorInt aa;
        TestDefVectorInt aaa(std::make_unique<std::vector<int>>(5, 5));
        TestDefVectorInt aaaa(std::make_unique<std::vector<int>>(5, 5));
        TestDefVectorInt aaaaa = std::make_unique<std::vector<int>>(5, 5);
    }
    {
        HashingType1PArr                    nochange;
        std::unique_ptr<int[]>              a;
        std::unique_ptr<std::vector<int>[]> b;
        std::unique_ptr<H1[]>               c;
        std::unique_ptr<H1[]>               d;

        // this won't rewritten but it could be a case
        // if so, a rule needs to be build
        // CGL::ArrayAutoPtr<CGL::ArrayAutoPtr<int>> c;

        std::unique_ptr<char[]> cc(std::make_unique<char[]>(5));
        std::unique_ptr<char[]> cccc = std::make_unique<char[]>(7);

        const std::unique_ptr<int[]>  ac;
        const std::unique_ptr<char[]> acc(std::make_unique<char[]>(5));
        const std::unique_ptr<char[]> acccc = std::make_unique<char[]>(7);

        TestDefCharArr aa;
        TestDefCharArr aaa(std::make_unique<char[]>(5));
        TestDefCharArr aaaaa = std::make_unique<char[]>(7);
    }
}
typedef std::unique_ptr<int>   TestDefInt;
typedef std::unique_ptr<int[]> TestDefIntArr;

[[maybe_unused]] void testArgReplacement(std::unique_ptr<int> tbr1, std::unique_ptr<int> tbr2)
{
    return;
}

[[maybe_unused]] void testArgReplacement2(std::unique_ptr<int[]> tbr1, std::unique_ptr<int[]> tbr2)
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
        std::unique_ptr<int>              a;
        std::unique_ptr<D>                at;
        std::unique_ptr<std::vector<int>> b;
        std::unique_ptr<int>              c;
        std::unique_ptr<D>                ct;

        std::unique_ptr<int> cc(std::make_unique<int>(5));
        std::unique_ptr<int> cccc  = std::make_unique<int>(5);
        std::unique_ptr<D>   cccct = std::make_unique<D>(5);

        const std::unique_ptr<int> ac;
        const std::unique_ptr<int> acc(std::make_unique<int>(5));
        const std::unique_ptr<int> acccc  = std::make_unique<int>(5);
        const std::unique_ptr<D>   acccct = std::make_unique<D>(5);

        TestDefInt aa;
        TestDefInt aaa(std::make_unique<int>(5));
        TestDefInt aaaaa = std::make_unique<int>(5);
    }
    {
        std::unique_ptr<int[]> a;
        std::unique_ptr<D[]>   at;
        std::unique_ptr<int[]> c;
        std::unique_ptr<D[]>   ct;

        std::unique_ptr<int[]> cc(std::make_unique<int[]>(5));
        std::unique_ptr<int[]> cccc  = std::make_unique<int[]>(5);
        std::unique_ptr<D[]>   cccct = std::make_unique<D>(5);

        const std::unique_ptr<int[]> ac;
        const std::unique_ptr<int[]> acc(std::make_unique<int[]>(5));
        const std::unique_ptr<int[]> acccc  = std::make_unique<int[]>(5);
        const std::unique_ptr<D[]>   acccct = std::make_unique<D[]>(5);

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

        std::unique_ptr<std::vector<int>> a(std::make_unique<std::vector<int>>(5, 5));
        std::unique_ptr<std::string>      c(std::make_unique<std::string>("hello"));
        std::unique_ptr<int>              cc(std::make_unique<int>(5));
        std::unique_ptr<int>              ccc(std::make_unique<int>());
        std::unique_ptr<std::vector<int>> vv(std::make_unique<std::vector<int>>());
        std::unique_ptr<int>              d(nullptr);
        std::unique_ptr<int>              e(test_pointer_int);
        std::unique_ptr<std::string>      f(test_pointer_string);
        std::unique_ptr<int>              g(std::move(e));
        std::unique_ptr<std::string>      h(std::move(f));
        std::unique_ptr<std::vector<int>> i(std::move(a));
        std::unique_ptr<std::string>      j(std::move(h));
        std::unique_ptr<std::vector<int>> k(std::move(i));

        TestDefInt ii(std::make_unique<int>(5));
        TestDefInt iii(std::make_unique<int>());
        TestDefInt iiii(false ? std::make_unique<int>(5) : std::make_unique<int>());
    }
    {
        std::string test_array_s_vals[] = {"asdfa", "dsfas"};
        int         test_array_int[5];

        std::unique_ptr<std::set<std::string>[]> b(std::make_unique<std::set<std::string>[]>(3));
        std::unique_ptr<unsigned char[]>         d(std::make_unique<unsigned char[]>(testMethod(1, 2)));
        std::unique_ptr<int[]>                   f(test_array_int);
        std::unique_ptr<std::string[]>           i(test_array_s_vals);
        std::unique_ptr<std::string[]>           j(std::move(i));
        std::unique_ptr<std::set<std::string>[]> l(std::move(b));
        std::unique_ptr<std::string[]>           m(std::move(j));
        std::unique_ptr<std::set<std::string>[]> p(std::move(l));
        std::unique_ptr<int[]>                   y = std::make_unique<int[]>(5);
        std::unique_ptr<std::set<std::string>[]> z = std::make_unique<std::set<std::string>[]>(5);

        TestDefIntArr ii(std::make_unique<int[]>(5));
        TestDefIntArr iii(std::make_unique<int[]>(testMethod(1, 2)));
        TestDefIntArr iiii(false ? std::make_unique<int[]>(5) : std::make_unique<int[]>(2));
    }
}

}  // namespace
