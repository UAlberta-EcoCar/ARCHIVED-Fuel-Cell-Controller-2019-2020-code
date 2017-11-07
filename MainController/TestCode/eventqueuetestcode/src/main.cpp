#include <mbed.h>
#include <rtos.h>
#include <mbed_events.h>

EventQueue test(32 * EVENTS_EVENT_SIZE);
Ticker add_event;
DigitalOut pin(PB_2);

void write_high_500ms(){
  pin = 1;
  Thread::wait(500);
  pin = 0;
}

void add_write(){
  test.call(write_high_500ms);
}

int main(int argc, char const *argv[]) {


  add_event.attach(&add_write, 10.0);
  Thread::wait(2000);

  test.dispatch();
  while(1){}

  return 0;
}
