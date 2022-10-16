
#ifndef _TIMED_MOVING_AVERAGE_H
#define _TIMED_MOVING_AVERAGE_H

#include <chrono>

class TimedMovingAverage {
public:
  TimedMovingAverage();

  double GetAverage(double newValue, double secondsToAverage);
  void reset();

private:
  bool mInit;
  double mAverage;

  double GetDiffSeconds();

  // Timer implementation

  void resetTimer();
  double elapsedTimer() const;
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::ratio<1>> second_;
  std::chrono::time_point<clock_> beg_;
};

#endif
