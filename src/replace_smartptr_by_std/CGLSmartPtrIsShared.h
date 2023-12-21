#pragma once

#include <memory>

namespace CGL {

template<class Type> using SmartPtr = std::shared_ptr<Type>;

template<typename T, typename... Args> auto MakeSmartPtr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace CGL
