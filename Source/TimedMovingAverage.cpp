
#include <chrono>
#include "TimedMovingAverage.hpp"

TimedMovingAverage::TimedMovingAverage() : beg_(clock_::now()), mInit(false), mAverage(0.0) {

}

void TimedMovingAverage::reset() {
  mInit = false;
}

double TimedMovingAverage::GetAverage(double newValue, double secondsToAverage) {
  if(secondsToAverage <= 0.00001) {
    return newValue;
  }

  if(!mInit) {
    mAverage = newValue;
    mInit = true;
    resetTimer();
    return mAverage;
  }

  double diff = GetDiffSeconds();
  double influence = (diff / secondsToAverage);

  if(influence >= 1.0) {
    mAverage = newValue;
    return mAverage;
  }

  mAverage = (mAverage * (1.0 - influence)) + (newValue * influence);

  return mAverage;
}

double TimedMovingAverage::GetDiffSeconds() {
  double diff = elapsedTimer();
  resetTimer();
  return(diff);
}

// Timer implementation

void TimedMovingAverage::resetTimer() {
  beg_ = clock_::now();
}

double TimedMovingAverage::elapsedTimer() const {
  return(std::chrono::duration_cast<second_>(clock_::now() - beg_).count());
}
