#pragma once

#include <memory>

namespace CGL {

template<class Type> using AutoPtr = std::unique_ptr<Type>;

}
