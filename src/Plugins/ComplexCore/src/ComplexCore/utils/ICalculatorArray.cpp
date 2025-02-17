#include "ComplexCore/utils/ICalculatorArray.hpp"

using namespace complex;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ICalculatorArray::ICalculatorArray() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ICalculatorArray::~ICalculatorArray() = default;

// -----------------------------------------------------------------------------
ICalculatorArray::Pointer ICalculatorArray::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}
