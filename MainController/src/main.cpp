#include <mbed.h>
#include <rtos.h>

#include "private_lib/constants.h"
#include "controller_event_queue.h"

Thread eventThread;

int main(int argc, char const *argv[]) {
  eventThread.start(contoller_event_queue_thread);

  while(1){Thread::wait(1000);};
  return 0;
}
