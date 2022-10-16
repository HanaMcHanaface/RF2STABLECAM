
#ifndef _TIME_DIFFER
#define _TIME_DIFFER

#include <chrono>
#include <iostream>
#include <chrono>

class TimeDiffer {
public:
  TimeDiffer() : previousTime(clock_::now()) {}

  double diff() {
    std::chrono::time_point<clock_> timeNow = clock_::now();

    double d = std::chrono::duration_cast<second_>(timeNow - previousTime).count();

    previousTime = timeNow;

    return(d);
  }

private:
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::ratio<1> > second_;
  std::chrono::time_point<clock_> previousTime;
};

#endif
