
auto matchFunctionDecl(const std::string innerTypeBind, const std::string fullTypeBind, const std::string fullDeclBind, const std::string classNameToMatch){
    return functionDecl(

    hasReturnTypeLoc(elaboratedTypeLoc(hasNamedTypeLoc(templateSpecializationTypeLoc(hasTemplateArgumentLoc(0, hasTypeLoc(typeLoc().bind(innerTypeBind)))))).bind(fullTypeBind))
).bind(fullDeclBind);
}


auto replaceFunctionDecl()
{
    const std::string fullDef = "fulldef";
    const std::string fullType = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource, 
        matchFunctionDecl(innerType, fullType, fullDef, className)
            ),
        changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))), "std::unique_ptr<", node(innerType), ">", enclose(after(node(fullType)), after(node(fullDef))))),
        cat("Replace a ", className, " functiondef with std::unique_ptr"));
}

auto replaceFunctionDeclArr()
{
    const std::string fullDef = "fulldef";
    const std::string fullType = "fulltype";
    const std::string innerType = "innertype";
    return makeRule(
        traverse(clang::TK_IgnoreUnlessSpelledInSource, 
        matchFunctionDecl(innerType, fullType, fullDef, classNameArr)
            ),
        changeTo(cat(enclose(before(node(fullDef)), before(node(fullType))), "std::unique_ptr<", node(innerType), "[]>", enclose(after(node(fullType)), after(node(fullDef))))),
        cat("Replace a ", className, " functiondef with std::unique_ptr"));
}