#include <math.h>

#include "LinearScalable.h"

using namespace std;

LinearScalable::LinearScalable()
{
  this->set_params();
}

LinearScalable::LinearScalable(
  float a
)
{
  this->set_params(a);
}

LinearScalable::LinearScalable(
  float a,
  float b
)
{
  this->set_params(a, b);
}

float LinearScalable::scale(
  float value
)
{
  return (a*value) + b;
}

void LinearScalable::set_params(
  float a,
  float b
)
{
  this->a = a;
  this->b = b;
}
