
#ifndef _TIMED_VECTOR3_AVERAGE_H
#define _TIMED_VECTOR3_AVERAGE_H

#include <chrono>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "TimedMovingAverage.hpp"

using Eigen::Vector3d;

class TimedVector3Average {
public:
  TimedVector3Average() {}

  Vector3d GetAverage(Vector3d newValue, double secondsToAverage) {
    return(Vector3d(
      mAverageX.GetAverage(newValue.x(), secondsToAverage),
      mAverageY.GetAverage(newValue.y(), secondsToAverage),
      mAverageZ.GetAverage(newValue.z(), secondsToAverage)
    ));
  }

  Vector3d GetAverage(Vector3d newValue, double secondsToAverageX, double secondsToAverageY, double secondsToAverageZ) {
    return(Vector3d(
      mAverageX.GetAverage(newValue.x(), secondsToAverageX),
      mAverageY.GetAverage(newValue.y(), secondsToAverageY),
      mAverageZ.GetAverage(newValue.z(), secondsToAverageZ)
    ));
  }

  void reset() {
    mAverageX.reset();
    mAverageY.reset();
    mAverageZ.reset();
  }

private:
  TimedMovingAverage mAverageX;
  TimedMovingAverage mAverageY;
  TimedMovingAverage mAverageZ;
};

#endif
