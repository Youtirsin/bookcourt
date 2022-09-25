#include "task_timer.h"

std::chrono::milliseconds Timer::checkDuration(100);

Timer::time_point_t Timer::create_timepoint(Timer::Moment moment) {
  auto now = time(nullptr);
  auto tt = localtime(&now);
  tt->tm_hour = moment.hour;
  tt->tm_min = moment.min;
  tt->tm_sec = moment.sec;
  return std::chrono::system_clock::from_time_t(mktime(tt));
}

void Timer::sleep_util(time_point_t const& time) {
  using namespace std::chrono;
  while (isRunning && system_clock::now() < time) {
    // check state every checkDuration
    std::this_thread::sleep_for(checkDuration);
  }
}

void Timer::sleep_for(delay_t const& delay) {
  using namespace std::chrono;
  auto start = system_clock::now();
  // check state every 100 milliseconds
  while (isRunning && (system_clock::now() - start < delay)) {
    std::this_thread::sleep_for(checkDuration);
  }
}
