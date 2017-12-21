#include <math.h>

#include "ExpScalable.h"

using namespace std;

ExpScalable::ExpScalable()
{
  this->set_params();
}

ExpScalable::ExpScalable(
  float a
)
{
  this->set_params(a);
}

ExpScalable::ExpScalable(
  float a,
  float b
)
{
  this->set_params(a, b);
}

float ExpScalable::scale(
  float value
)
{
  return (a*exp(value)) + b;
}

void ExpScalable::set_params(
  float a,
  float b
)
{
  this->a = a;
  this->b = b;
}
