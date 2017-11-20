#include <mbed.h>

#include "Fan.h"

using namespace std;

Fan::Fan(
  PinName out_pin,
  PinName in_pin
)
:PwmOut(out_pin), PwmIn(in_pin)
{
  // Do Nothing
}
