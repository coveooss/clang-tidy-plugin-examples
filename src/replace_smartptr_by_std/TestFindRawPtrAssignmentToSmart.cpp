#include <utility>  // std::move
#include <vector>

#include "CGLSmartPtrStub.h"

// SHOULD OUTPUT 84 WARNINGS, 3 NOLINT

namespace {

class TestArgList
{
public:
    TestArgList(int* i) : sp(i){};

private:
    CGL::SmartPtr<int> sp;
};

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

        TestArgList testInit(i);

        // Test SmartPtr(T* p_pObject = nullptr) and SmartPtr<T>& operator=(T* p_pObject)
        smartPtrTemplated = new D;
        smartPtrTemplated = new D();
        smartPtrTemplated = new D(x);
        smartPtrTemplated = new int;
        smartPtrTemplated = new int();
        smartPtrTemplated = new int(5);
        smartPtrTemplated = new D(testMethod(x, x));
        smartPtrVector    = new std::vector<int>(5);

        const D* toUndoConst = new D;
        smartPtrTemplated.reset(const_cast<D*>(toUndoConst));

        CGL::SmartPtr<D> templatedRawNoParens = new D;

        CGL::SmartPtr<D>       templatedRawNoParensMake = CGL::MakeSmartPtr<D>();  // Shouldn't find this
        const CGL::SmartPtr<D> templatedRawNoArgs       = new D();
        CGL::SmartPtr<D>       templatedRaw             = new D(x);

        D* d = new D();

        templatedRaw.reset(d);
        CGL::SmartPtr<D>       templatedRawLiteral = new D(5);
        CGL::SmartPtr<int>     rawLiteral          = new int(5);
        CGL::SmartPtr<D>       templatedRawMethod(new D(testMethod(x, x)));
        const CGL::SmartPtr<D> templatedExisting(i);

        templatedRaw = i;
        templatedRaw.reset(i);

        smartPtrTemplated = i;
        smartPtrTemplated.reset(i);

        ChildClass* c = new ChildClass;

        SPParent typedefClassSmartPtr  = new ParentClass;
        SPParent typedefClassSmartPtr2 = new ChildClass();
        typedefClassSmartPtr           = c;
        typedefClassSmartPtr2.reset(c);
    }

    // Test assignment and constructors with other SmartPtrs
    {
        // Set up variables for slicing
        CGL::SmartPtr<ParentClass> parent1 = new ParentClass;
        CGL::SmartPtr<ParentClass> parent2 = new ParentClass;

        const CGL::SmartPtr<ChildClass> child1 = new ChildClass;

        // Shouldn't find any of these
        parent1 = parent2;
        parent1 = child1;
        CGL::SmartPtr<ParentClass> parent3(parent1);
        CGL::SmartPtr<ParentClass> parent4 = parent1;
        CGL::SmartPtr<ParentClass> parent5 = child1;
        CGL::SmartPtr<ParentClass> parent6(child1);
        parent1                            = std::move(parent2);
        parent1                            = std::move(child1);
        CGL::SmartPtr<ParentClass> parent8 = std::move(child1);
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

        // Shouldn't find these
        smartPtrTemplated = CGL::MakeSmartPtr<char>();
        smartPtrVector    = CGL::MakeSmartPtr<std::vector<int>>(5);

        CGL::SmartPtr<char>       rawNoParens = new char;
        const CGL::SmartPtr<char> rawNoArgs   = new char();
        CGL::SmartPtr<char>       raw         = new char(x);
        char*                     c           = new char();
        raw.reset(c);
        CGL::SmartPtr<char> rawLiteral = new char(5);
        CGL::SmartPtr<char> templatedRawMethod(new char(testMethod(x, x)));
        CGL::SmartPtr<char> templatedExisting(i);
        templatedExisting = i;
        templatedExisting.reset(i);

        SPChar typedefSP  = new char;
        SPChar typedefSP2 = new char();
        typedefSP         = c;
        typedefSP2.reset(c);

        typedefSP.reset();

        SPChar cc(typedefSP2);  // Stays the same
    }

    using namespace CGL;
    using namespace std;
    {
        SmartPtr<char>* csp;
        csp  = new SmartPtr<char>;
        *csp = new char;

        SmartPtr<ParentClass>* psp;
        psp  = new SmartPtr<ParentClass>;
        *psp = new ParentClass;

        char* c = new char;
        *csp    = c;
        csp->reset(c);

        ParentClass* p = new ParentClass;
        *psp           = p;
        psp->reset(p);
    }

    OwnsSmart* a = new OwnsSmart;
    int*       i = new int(3);

    a->sp->reset(new int(5));
    a->sp->reset(i);
    *a->sp = i;
    (*a->sp).reset(i);

    a->sp->reset();
}

[[maybe_unused]] int TemplateInstantiation()
{
    TemplatedClassToForceDependentScope<int> instantiateTemplate;
    instantiateTemplate.TestDependentScopeMethod(3);
    TestCallsReplacements('A');

    return 0;
}

}  // namespace
