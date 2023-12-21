#pragma once

#include <memory>

namespace CGL {

template<class Type> using ArrayAutoPtr = std::unique_ptr<Type[]>;

}
