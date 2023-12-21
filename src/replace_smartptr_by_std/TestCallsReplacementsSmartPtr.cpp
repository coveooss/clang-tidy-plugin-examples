#include <cstddef>
#include <utility>
#ifdef INCLUDE_SMARTPTR_ALIAS_STUBS
#    include "CGLSmartPtrStub.h"
#else
#    include "CGLSmartPtrIsShared.h"
#endif

#include <vector>

namespace {

class ParentClass
{
public:
    int foo;
};

class ChildClass : public ParentClass
{
public:
    int bar;
};

[[maybe_unused]] const inline int testMethod(int a, int b)
{
    return a + b;
}

[[maybe_unused]] const inline int* testMethodPtr(int* a)
{
    return a;
}

template<typename D> class TemplatedClassToForceDependentScope
{
public:
    void TestDependentScopeMethod(D x);

private:
    CGL::SmartPtr<D>                        smartPtrTemplated;
    const CGL::SmartPtr<ChildClass>         smartPtrClass;
    mutable CGL::SmartPtr<std::vector<int>> smartPtrVector;
};

template<typename D> void TemplatedClassToForceDependentScope<D>::TestDependentScopeMethod(D x)
{
    typedef CGL::SmartPtr<ParentClass> SPParent;

    // Test assignment and constructors with raw pointers and new objects
    {
        int* i = new int(3);

        // Test SmartPtr(T* p_pObject = nullptr) and SmartPtr<T>& operator=(T* p_pObject)
        smartPtrTemplated = new D;
        smartPtrTemplated = new D();
        smartPtrTemplated = new D(x);
        smartPtrTemplated = new int;
        smartPtrTemplated = new int();
        smartPtrTemplated = new int(5);
        smartPtrTemplated = new D(testMethod(x, x));
        smartPtrVector    = new std::vector<int>(5);

        // Just to check we don't break these
        smartPtrTemplated = CGL::MakeSmartPtr<D>();
        smartPtrVector    = CGL::MakeSmartPtr<std::vector<int>>(5);

        CGL::SmartPtr<D>       templatedRawNoParens     = new D;
        CGL::SmartPtr<D>       templatedRawNoParensMake = CGL::MakeSmartPtr<D>();
        const CGL::SmartPtr<D> templatedRawNoArgs       = new D();
        CGL::SmartPtr<D>       templatedRaw             = new D(x);
        D*                     d                        = new D();
        templatedRaw.reset(d);
        CGL::SmartPtr<D> templatedRawLiteral = new D(5);
        smartPtrTemplated.Swap(templatedRawLiteral);
        CGL::SmartPtr<int>     rawLiteral     = new int(5);
        CGL::SmartPtr<int>     rawLiteralMake = CGL::MakeSmartPtr<int>(5);
        CGL::SmartPtr<D>       templatedRawMethod(new D(testMethod(x, x)));
        const CGL::SmartPtr<D> templatedExisting(i);

        templatedRaw = i;
        templatedRaw.reset(i);

        smartPtrTemplated = i;
        smartPtrTemplated.reset(i);

        ChildClass* c                     = new ChildClass;
        SPParent    typedefClassSmartPtr  = new ParentClass;
        SPParent    typedefClassSmartPtr2 = new ChildClass();
        typedefClassSmartPtr              = c;
        typedefClassSmartPtr2.reset(c);

        templatedRaw         = nullptr;
        templatedRaw         = 0;
        smartPtrTemplated    = nullptr;
        smartPtrTemplated    = 0;
        typedefClassSmartPtr = nullptr;
        smartPtrTemplated    = 0;
    }

    // Test assignment and constructors with other SmartPtrs
    {
        // Set up variables for slicing
        CGL::SmartPtr<ParentClass> parent1 = new ParentClass;
        CGL::SmartPtr<ParentClass> parent2 = new ParentClass;

        const CGL::SmartPtr<ChildClass> child1 = new ChildClass;

        // Test SmartPtr<T>& operator=(const SmartPtr<T>& p_Object)
        parent1 = parent2;
        // Test template<typename U> SmartPtr<T>& operator=(const SmartPtr<U>& p_Object)
        parent1 = child1;

        // Test SmartPtr(const SmartPtr<T>& p_Object){};
        CGL::SmartPtr<ParentClass> parent3(parent1);
        CGL::SmartPtr<ParentClass> parent4 = parent1;

        // Test template<typename U> SmartPtr(const SmartPtr<U>& p_Object){};
        CGL::SmartPtr<ParentClass> parent5 = child1;
        CGL::SmartPtr<ParentClass> parent6(child1);

        // Test SmartPtr<T>& operator=(SmartPtr<T>&& p_Object)
        parent1 = std::move(parent2);
        // Test template<typename U> SmartPtr<T>& operator=(SmartPtr<U>&& p_Object)
        parent1 = std::move(child1);

        // Test SmartPtr(SmartPtr<T>&& p_Object){};
        CGL::SmartPtr<ParentClass> parent7 = std::move(parent1);
        // Test template<typename U> SmartPtr(SmartPtr<U>&& p_Object){};
        CGL::SmartPtr<ParentClass> parent8 = std::move(child1);

        // Test void Swap(SmartPtr<T>& p_rObject){}
        parent1.swap(parent2);
        parent2.Swap(parent1);
        std::swap(parent1, parent2);
    }
    using namespace CGL;
    using namespace std;
    // Test methods
    {
        vector<SmartPtr<ChildClass>> sv(2);
        SmartPtr<ParentClass> const  smartPtrParentClass;
        SPParent                     typedefSP;
        SPParent const               typedefSPConst;

        // -> operator
        sv[0]->foo               = 1;
        sv[1]->foo               = 2;
        smartPtrClass->foo       = 3;
        smartPtrParentClass->foo = 4;
        typedefSP->foo           = 5;
        typedefSPConst->foo      = 6;

        // * operator
        [[maybe_unused]] const int deref1 = (*sv[0]).foo;
        (void)(*sv[1]).foo;
        [[maybe_unused]] int deref2 = (*smartPtrClass).foo;
        [[maybe_unused]] int deref3 = (*smartPtrParentClass).foo;
        [[maybe_unused]] int deref4 = (*typedefSP).foo;
        [[maybe_unused]] int deref5 = (*typedefSPConst).foo;

        [[maybe_unused]] ParentClass noChange = *typedefSP;

        // getPtr
        [[maybe_unused]] const int getPtr1 = sv[0].GetPtr()->foo;
        (void)sv[1].GetPtr()->foo;
        [[maybe_unused]] int getPtr2         = smartPtrClass.GetPtr()->foo;
        [[maybe_unused]] int getPtr3         = smartPtrParentClass.GetPtr()->foo;
        [[maybe_unused]] int getPtr4         = typedefSP.GetPtr()->foo;
        [[maybe_unused]] int getPtr5         = typedefSPConst.GetPtr()->foo;
        [[maybe_unused]] D*  dependentGetPtr = smartPtrTemplated.GetPtr();

        [[maybe_unused]] int getPtrNoChange1   = smartPtrClass.get()->foo;
        [[maybe_unused]] int getPtrNoChange2   = typedefSPConst.get()->foo;
        [[maybe_unused]] D*  dependentNoChange = smartPtrTemplated.get();

        // Test Implicit conversion replacement
        [[maybe_unused]] D* convertAuto = smartPtrTemplated;

        [[maybe_unused]] ParentClass* ptrParent = smartPtrParentClass;
        ptrParent                               = typedefSP;
        ptrParent                               = typedefSPConst;
        ptrParent                               = sv[0];

        {
            SmartPtr<int>         smartPtrInt;
            [[maybe_unused]] int* ptrInt = smartPtrInt;
            testMethodPtr(smartPtrInt);
        }
        {
            typedef SmartPtr<int> IntSP;
            IntSP                 smartPtrInt;
            [[maybe_unused]] int* ptrInt = smartPtrInt;
            testMethodPtr(smartPtrInt);
        }
    }
    // Test Comparison Operators
    {
        const ParentClass* rawPC       = new ParentClass();
        ChildClass*        rawCC       = new ChildClass();
        CGL::SmartPtr<D>   templatedSP = new D();
        D*                 rawD        = new D();

        SmartPtr<ParentClass>       pcPtr;
        SmartPtr<ParentClass> const pcPtr2;
        SPParent const              typedefSPConst;

        // ==
        [[maybe_unused]] bool eqComp1 = pcPtr == pcPtr2;
        [[maybe_unused]] bool eqComp2 = pcPtr == rawPC;
        [[maybe_unused]] bool eqComp3 = pcPtr == rawCC;
        [[maybe_unused]] bool eqComp4 = pcPtr == smartPtrClass;
        [[maybe_unused]] bool eqComp5 = pcPtr == typedefSPConst;
        [[maybe_unused]] bool eqComp6 = smartPtrTemplated == templatedSP;
        [[maybe_unused]] bool eqComp7 = rawD == smartPtrTemplated;

        // !=
        [[maybe_unused]] bool neqComp1 = pcPtr != pcPtr2;
        [[maybe_unused]] bool neqComp2 = pcPtr != rawPC;
        [[maybe_unused]] bool neqComp3 = pcPtr != rawCC;
        [[maybe_unused]] bool neqComp4 = pcPtr != smartPtrClass;
        [[maybe_unused]] bool neqComp5 = pcPtr != typedefSPConst;
        [[maybe_unused]] bool neqComp6 = templatedSP != smartPtrTemplated;
        [[maybe_unused]] bool neqComp7 = rawD != smartPtrTemplated;

        // <
        [[maybe_unused]] bool ltComp1 = pcPtr < pcPtr2;
        [[maybe_unused]] bool ltComp2 = pcPtr < rawPC;
        [[maybe_unused]] bool ltComp3 = pcPtr < rawCC;
        [[maybe_unused]] bool ltComp4 = pcPtr < smartPtrClass;
        [[maybe_unused]] bool ltComp5 = pcPtr < typedefSPConst;
        [[maybe_unused]] bool ltComp6 = templatedSP < smartPtrTemplated;
        [[maybe_unused]] bool ltComp7 = rawD < smartPtrTemplated;

        // nullptr
        [[maybe_unused]] bool nullComp1    = pcPtr2 == nullptr;
        [[maybe_unused]] bool nullComp2    = smartPtrClass == nullptr;
        [[maybe_unused]] bool nullComp3    = pcPtr == nullptr;
        [[maybe_unused]] bool nullComp4    = typedefSPConst == nullptr;
        [[maybe_unused]] bool nullComp5    = smartPtrTemplated == nullptr;
        [[maybe_unused]] bool nullnotComp1 = pcPtr2 != nullptr;
        [[maybe_unused]] bool nullnotComp2 = smartPtrClass != nullptr;
        [[maybe_unused]] bool nullnotComp3 = pcPtr != nullptr;
        [[maybe_unused]] bool nullnotComp4 = typedefSPConst != nullptr;
        [[maybe_unused]] bool nullnotComp5 = smartPtrTemplated != nullptr;
    }
}

static CGL::SmartPtr<char> staticPtr;

struct OwnsSmart {
    CGL::SmartPtr<int>* sp;
};

// Same tests but in non-dependent scope
void TestCallsReplacements(char x)
{
    staticPtr = new char('A');

    typedef CGL::SmartPtr<char>     SPChar;
    CGL::SmartPtr<char>             smartPtrTemplated;
    const CGL::SmartPtr<ChildClass> smartPtrClass;
    CGL::SmartPtr<std::vector<int>> smartPtrVector;

    // Test assignment and constructors with raw pointers and new objects
    {
        char* i = new char(3);

        // Test SmartPtr(T* p_pObject = nullptr) and SmartPtr<T>& operator=(T* p_pObject)
        smartPtrTemplated = new char;
        smartPtrTemplated = new char();
        smartPtrTemplated = new char(x);
        smartPtrTemplated = new char(testMethod(x, x));
        smartPtrVector    = new std::vector<int>(5);

        // Just to check we don't break these
        smartPtrTemplated = CGL::MakeSmartPtr<char>();
        smartPtrVector    = CGL::MakeSmartPtr<std::vector<int>>(5);

        CGL::SmartPtr<char>       rawNoParens     = new char;
        CGL::SmartPtr<char>       rawNoParensMake = CGL::MakeSmartPtr<char>();
        const CGL::SmartPtr<char> rawNoArgs       = new char();
        CGL::SmartPtr<char>       raw             = new char(x);
        char*                     c               = new char();
        raw.reset(c);
        CGL::SmartPtr<char> rawLiteral     = new char(5);
        CGL::SmartPtr<int>  rawLiteralMake = CGL::MakeSmartPtr<int>(5);
        CGL::SmartPtr<char> templatedRawMethod(new char(testMethod(x, x)));
        CGL::SmartPtr<char> templatedExisting(i);
        templatedExisting = i;
        templatedExisting.reset(i);

        SPChar typedefSP  = new char;
        SPChar typedefSP2 = new char();
        typedefSP         = c;
        typedefSP2.reset(c);
        SPChar cc(typedefSP2);
    }

    using namespace CGL;
    using namespace std;
    // Test methods
    {
        SPChar       typedefSP;
        SPChar const typedefSPConst;

        // * operator
        *typedefSP      = 'A';
        *typedefSPConst = 'B';

        [[maybe_unused]] const char deref1 = *typedefSP;
        [[maybe_unused]] char       deref2 = *typedefSPConst;

        // getPtr
        [[maybe_unused]] const char* getptr1    = typedefSP.GetPtr();
        [[maybe_unused]] char*       getptr2    = typedefSPConst.GetPtr();
        [[maybe_unused]] const char  getptrval1 = *typedefSP.GetPtr();
        [[maybe_unused]] char        getptrval2 = *typedefSPConst.GetPtr();

        SmartPtr<char>*              weDoThis = new SmartPtr<char>;
        [[maybe_unused]] const char* getVal   = weDoThis->GetPtr();

        // Test Implicit conversion replacement
        [[maybe_unused]] char* rawptr = typedefSP;
        rawptr                        = typedefSPConst;

        OwnsSmart             spStruct;
        [[maybe_unused]] int* noChange = *spStruct.sp;
    }
    // Test Comparison Operators
    {
        const char* rawPC = new char();

        SmartPtr<char>       pcPtr;
        SmartPtr<char> const pcPtr2;
        SPChar const         typedefSPConst;

        // ==
        [[maybe_unused]] bool eqComp1 = pcPtr == pcPtr2;
        [[maybe_unused]] bool eqComp2 = pcPtr == rawPC;
        [[maybe_unused]] bool eqComp4 = pcPtr == staticPtr;
        [[maybe_unused]] bool eqComp5 = pcPtr == typedefSPConst;

        // !=
        [[maybe_unused]] bool neqComp1 = pcPtr != pcPtr2;
        [[maybe_unused]] bool neqComp2 = pcPtr != rawPC;
        [[maybe_unused]] bool neqComp4 = pcPtr != staticPtr;
        [[maybe_unused]] bool neqComp5 = pcPtr != typedefSPConst;

        // <
        [[maybe_unused]] bool ltComp1 = pcPtr < pcPtr2;
        [[maybe_unused]] bool ltComp2 = pcPtr < rawPC;
        [[maybe_unused]] bool ltComp4 = pcPtr < staticPtr;
        [[maybe_unused]] bool ltComp5 = pcPtr < typedefSPConst;

        // nullptr
        [[maybe_unused]] bool nullComp1    = pcPtr2 == nullptr;
        [[maybe_unused]] bool nullComp2    = staticPtr == nullptr;
        [[maybe_unused]] bool nullComp3    = pcPtr == nullptr;
        [[maybe_unused]] bool nullComp4    = typedefSPConst == nullptr;
        [[maybe_unused]] bool nullnotComp1 = pcPtr2 != nullptr;
        [[maybe_unused]] bool nullnotComp2 = staticPtr != nullptr;
        [[maybe_unused]] bool nullnotComp3 = pcPtr != nullptr;
        [[maybe_unused]] bool nullnotComp4 = typedefSPConst != nullptr;
    }
    {
        SmartPtr<char>* csp;
        csp  = new SmartPtr<char>;
        *csp = new char;

        SmartPtr<ParentClass>* psp;
        psp  = new SmartPtr<ParentClass>;
        *psp = new ParentClass;

        char* c = new char;
        *csp    = c;

        *csp = nullptr;
        *csp = 0;

        ParentClass* p = new ParentClass;
        *psp           = p;

        *psp = nullptr;
        *psp = 0;
    }
}

[[maybe_unused]] int TemplateInstantiation()
{
    TemplatedClassToForceDependentScope<int> instantiateTemplate;
    instantiateTemplate.TestDependentScopeMethod(3);
    TestCallsReplacements('A');

    return 0;
}

}  // namespace
