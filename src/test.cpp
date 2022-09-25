#include <iostream>
#include <boost/json.hpp>

#include "task_timer/task_timer.h"

using namespace std;

template<class Task, class... Args>
void rerange_task(Timer::time_point_t timepoint, Task task, Args... args) {
  static Timer *timer = nullptr;
  static std::future<void> future;
  if (timer) {
    timer->cancel();
    std::this_thread::sleep_for(Timer::get_check_duration() * 2);
    delete timer;
  }
  timer = new Timer;
  future = timer->schedule(timepoint, task, args...);
}

int main() {

  return 0;
}
