#ifndef TASK_TIMER
#define TASK_TIMER

#include <atomic>
#include <iostream>
#include <chrono>
#include <future>
#include <thread>

class Timer {
 public:
  using time_point_t = std::chrono::time_point<std::chrono::system_clock>;
  using period_t = std::chrono::milliseconds;
  using delay_t = std::chrono::milliseconds;

 public:
  struct Moment {
    int hour;
    int min;
    int sec;
  };

  Timer() : isRunning(true) {}
  Timer(Timer const& other) = delete;
  Timer(Timer&& other) = delete;
  Timer& operator=(Timer const& other) = delete;
  Timer& operator=(Timer&& other) = delete;

  ~Timer() = default;

  time_point_t create_timepoint(Moment moment);
  // 01
  // executes task as a timepoint
  template <typename TimerTask, typename... Args>
  std::future<void> schedule(time_point_t const& time, TimerTask task,
                             Args... args) {
    std::cout << "[task_timer] added timepoint task.\n";
    return std::async(std::launch::async, [=]() {
      sleep_util(time);
      if (isRunning) task(args...);
    });
  }

  // 02
  // executes task as a timepoint and periodly
  template <typename TimerTask, typename... Args>
  std::future<void> schedule(time_point_t const& time, period_t const& period,
                             TimerTask task, Args... args) {
    std::cout << "[task_timer] added periodly timepoint task.\n";
    return std::async(std::launch::async, [=]() {
      sleep_util(time);
      while (isRunning) {
        task(args...);
        std::this_thread::sleep_for(period);
      }
    });
  }

  // 03
  // execute task after a delay
  template <typename TimerTask, typename... Args>
  std::future<void> schedule(delay_t const& delay, TimerTask task,
                             Args... args) {
    std::cout << "[task_timer] added delay task.\n";
    return std::async(std::launch::async, [=]() {
      sleep_for(delay);
      if (isRunning) task(args...);
    });
  }

  // 04
  // execute task after a delay and periodly
  template <typename TimerTask, typename... Args>
  std::future<void> schedule(delay_t const& delay, period_t const& period,
                             TimerTask task, Args... args) {
    std::cout << "[task_timer] added periodly delay task.\n";
    return std::async(std::launch::async, [=]() {
      sleep_for(delay);
      while (isRunning) {
        task(args...);
        std::this_thread::sleep_for(period);
      }
    });
  }
  void cancel() {
    std::cout << "[task_timer] canceled task.\n";
    isRunning = false;
  }

  static void set_check_duration(const std::chrono::milliseconds &duration) {
    checkDuration = duration;
  }

  static std::chrono::milliseconds get_check_duration() {
    return checkDuration;
  }

 private:
  static std::chrono::milliseconds checkDuration;
  std::atomic<bool> isRunning;

  void sleep_util(time_point_t const& time);

  void sleep_for(delay_t const& delay);
};

Timer::time_point_t create_timepoint(int hour, int min, int sec);

#endif
