#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fstream>

struct thread_data
{
  int is_running; // thread exit flag
  int period;     // pwm T[usec]
  int duty;       // pwm duty[%]
} data;

void* loopthread (void* param)
{
  struct thread_data *d = (struct thread_data *)param;

  struct timespec on_period  = {0, d->period * (d->duty / 100.0)};
  struct timespec off_period = {0, d->period * (1.0 - (d->duty / 100.0))};
  std::ofstream r_front("/sys/class/gpio/gpio67/value");
  std::ofstream r_back("/sys/class/gpio/gpio69/value");
  r_front << "0";
  r_back << "0";

  while(d->is_running)
    {
      r_front << "1" << std::endl;  
      nanosleep(&on_period, NULL);
      r_front << "0" << std::endl;
      nanosleep(&off_period, NULL);
    }

  r_front << "0";
  r_back << "0";
  printf("loop out\n");
  return NULL;
}

int main (int argc, char* argv[])
{
  pthread_t th;
  int i;

  data.is_running =    1;
  data.period     = 1000;
  data.duty       =  100;

  // over write init val period, duty
  if ((argc == 3))
    {
      i = atoi(argv[1]);
      if (0 < i && i <= 1000 * 1000 * 1000)
	data.period  = i;

      printf("%d\n",i);

      i = atoi(argv[2]);
      if (0 <= i && i <= 100)
	data.duty  = i;

      printf("%d\n",i);
    }

  signal(SIGTERM, [](int){
      data.is_running = 0;
    });

  pthread_create(&th, NULL, loopthread, &data);
  printf("join wait\n");
  pthread_join(th, NULL);
  printf("join\n");

}
