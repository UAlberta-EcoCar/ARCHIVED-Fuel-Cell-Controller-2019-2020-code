#include <mbed.h>
#include <rtos.h>

int fc_status = 0;

Mutex status_lock;


void set_fc_status(int val)
{
  status_lock.lock();
  fc_status = val;
  status_lock.unlock();
}

int get_fc_status(void)
{
  status_lock.lock();
  int val = fc_status;
  status_lock.unlock();
  return(val);
}
