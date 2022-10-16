//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Source File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                 PROPRIETARY AND CONFIDENTIAL                            ﬁ
//› Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "Example.hpp"          // corresponding header file
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
#include <chrono>
#include <sstream>
#include <Eigen/Core>
#include <Eigen/Geometry>

using Eigen::Quaterniond;
using Eigen::Vector3d;

// plugin information

extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return("RF2STABLECAM - 2022.10.16"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(7); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject*) new ExampleInternalsPlugin); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject * obj) { delete((ExampleInternalsPlugin*)obj); }

// ExampleInternalsPlugin class

ExampleInternalsPlugin::ExampleInternalsPlugin() {

}

void ExampleInternalsPlugin::WriteToAllExampleOutputFiles(const char* const openStr, const char* const msg) {
  return;
}

void ExampleInternalsPlugin::Startup(long version) {
  char temp[80];
  sprintf(temp, "-STARTUP- (version %.3f)", (float)version / 1000.0f);

  // Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
  // example output data files.
  WriteToAllExampleOutputFiles("w", temp);
}

void ExampleInternalsPlugin::Shutdown() {
  WriteToAllExampleOutputFiles("a", "-SHUTDOWN-");
}

void ExampleInternalsPlugin::StartSession() {
  WriteToAllExampleOutputFiles("a", "--STARTSESSION--");
}

void ExampleInternalsPlugin::EndSession() {
  WriteToAllExampleOutputFiles("a", "--ENDSESSION--");
}

void ExampleInternalsPlugin::EnterRealtime() {
  // start up timer every time we enter realtime
  WriteToAllExampleOutputFiles("a", "---ENTERREALTIME---");

  mTestPosInc = 500;

  //mCurrentPitch = 0.0;
  //mCurrentRoll = 0.0;
  //mCurrentPos = TelemVect3();
  mTestPos = TelemVect3();

  mCamPitch = 0.0;
  mCamRoll = 0.0;
  mCamYaw = 0.0;

  mCamX = 0.0;
  mCamY = 0.0;
  mCamZ = 0.0;

  mAveragePitch.reset();
  mAverageRoll.reset();
  mAverageX.reset();
  mAverageY.reset();
  mAverageX2.reset();
  mAverageY2.reset();

  mAverageGlobalPos1.reset();
  mAverageGlobalPos2.reset();
  mAverageGlobalPos3.reset();

  mDisableSequenceIndex = -1;

  mEnabled = true;
}

void ExampleInternalsPlugin::ExitRealtime() {
  WriteToAllExampleOutputFiles("a", "---EXITREALTIME---");

  mEnabled = false;
  //mShouldDisable = true;
  mDisableSequenceIndex = 0;
}

void ExampleInternalsPlugin::UpdateTelemetry(const TelemInfoV01& telemetry) {
  if(!IsEnabled()) {
    return;
  }

  TelemVect3 forwardVector = { -telemetry.mOri[0].z, -telemetry.mOri[1].z, -telemetry.mOri[2].z };
  TelemVect3    leftVector = { telemetry.mOri[0].x,  telemetry.mOri[1].x,  telemetry.mOri[2].x };

  //var rawYaw = (float)Math.Atan2(playerVeh.mOri[RowZ].x, playerVeh.mOri[RowZ].z);
  const double carYaw = atan2(telemetry.mOri[2].x, telemetry.mOri[2].z);
  const double carPitch = atan2(forwardVector.y, sqrt((forwardVector.x * forwardVector.x) + (forwardVector.z * forwardVector.z)));
  const double carRoll = atan2(leftVector.y, sqrt((leftVector.x * leftVector.x) + (leftVector.z * leftVector.z)));

  TelemVect3 carGlobalPos = telemetry.mPos;

  //mCurrentPitch = pitch;
  //mCurrentRoll = roll;
  //mCurrentYaw = yaw;

  //TelemQuat quat;
  //quat.ConvertMatToQuat(info.mOri);

  //TelemVect3 vec;

  //vec.Set(info.mPos.x, info.mPos.y, info.mPos.z);

  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  roll = -carRoll;
  pitch = -carPitch;
  //yaw = -carYaw;

  //pitch += atan2(-carY, 500.0);

  double averageRoll = mAverageRoll.GetAverage(carRoll, 0.2);
  double averagePitch = mAveragePitch.GetAverage(carPitch, 0.2);

  roll += averageRoll;
  pitch += averagePitch;

  //double diff = mTelemetryDiff.diff();
  //double carX = diff * telemetry.mLocalVel.x;
  //double carY = diff * telemetry.mLocalVel.y;

  //double averageX = mAverageX.GetAverage(carX, 1.0);
  //double averageY = mAverageY.GetAverage(carY, 1.0);

  //x = mAverageX2.GetAverage(25.0 * (averageX - carX), 0.05);
  //y = mAverageY2.GetAverage(25.0 * (averageY - carY), 0.05);

  //if(mTestPosInc >= 499) {
  //  mTestPos = carGlobalPos;
  //}
  //mTestPosInc = (mTestPosInc + 1) % 500;

  TelemQuat quat;
  quat.ConvertMatToQuat(telemetry.mOri);

  Quaterniond q = Quaterniond(quat.w, quat.x, quat.y, quat.z);

  //Vector3d globalTranslation = mTestPos.ToEigen() - carGlobalPos.ToEigen();
  //Vector3d resultY = (q.conjugate() * globalTranslation) * 6.595;
  //x = result.x();
  //y = -result.y();
  //z = result.z();

  Vector3d carGlobalPosE = carGlobalPos.ToEigen();

  //Vector3d smoothGlobalPos1 = mAverageGlobalPos1.GetAverage(carGlobalPosE, 0.015);
  //Vector3d globalTranslation1 = smoothGlobalPos1 - carGlobalPosE;
  //double resultX = (q.conjugate() * globalTranslation1).x();

  Vector3d smoothGlobalPos2 = mAverageGlobalPos2.GetAverage(carGlobalPosE, 0.08);
  Vector3d globalTranslation2 = smoothGlobalPos2 - carGlobalPosE;
  double resultY = (q.conjugate() * globalTranslation2).y();

  //Vector3d smoothGlobalPos3 = mAverageGlobalPos3.GetAverage(carGlobalPosE, 0.05);
  //Vector3d globalTranslation3 = smoothGlobalPos3 - carGlobalPosE;
  //double resultZ = (q.conjugate() * globalTranslation3).z();

  // Magic value to make the translation into global meters.
  double multTranslation = 6.595;

  //x = resultX * multTranslation;
  y = -resultY * multTranslation;
  //z = resultVec.z();

  //x = -(mCurrentPos.x - averageX);
  //y = -(mCurrentPos.y - averageY);

  //mCurrentPos = telemetry.mPos;

  mCamX = x;
  mCamY = y;
  mCamZ = z;

  mCamRoll = roll;
  mCamPitch = pitch;
  mCamYaw = yaw;
}

void ExampleInternalsPlugin::UpdateGraphics(const GraphicsInfoV02& info) {
  mCameraType = info.mCameraType;
}

bool ExampleInternalsPlugin::helperSetHWControl(
  const char* const controlName, double& fRetVal,
  const char* const negativeCompareName, const char* const positiveCompareName,
  int index, double targetVal
) {
  if(_stricmp(controlName, positiveCompareName) == 0) {
    if(mDisableSequenceIndex == index) {
      fRetVal = 0.0;
      mDisableSequenceIndex++;
      return(true);
    }

    if(mDisableSequenceIndex >= 0) {
      return(false);
    }

    if(targetVal > 0) {
      fRetVal = targetVal;
    } else {
      fRetVal = 0.0;
    }

    return(true);
  } else if(_stricmp(controlName, negativeCompareName) == 0) {
    if(mDisableSequenceIndex == index + 1) {
      fRetVal = 0.0;
      mDisableSequenceIndex++;
      return(true);
    }

    if(mDisableSequenceIndex >= 0) {
      return(false);
    }

    if(targetVal < 0) {
      fRetVal = -targetVal;
    } else {
      fRetVal = 0.0;
    }

    return(true);
  }

  return(false);
}

bool ExampleInternalsPlugin::CheckHWControl(const char* const controlName, double& fRetVal) {
  // only if enabled, of course
  if(!IsEnabled() && mDisableSequenceIndex < 0) {
    return(false);
  }

  // there are 12 items index 0-11. When #11 finished the index will then be 12
  if(mDisableSequenceIndex == 12) {
    mDisableSequenceIndex = -1;
    return(false);
  }

  double x = mCamX;
  double y = mCamY;
  double z = mCamZ;

  double roll = mCamRoll;
  double pitch = mCamPitch;
  double yaw = mCamYaw;

  return (
    helperSetHWControl(controlName, fRetVal, "Camera_RollLeft", "Camera_RollRight", 0, roll) ||
    helperSetHWControl(controlName, fRetVal, "Camera_PitchDown", "Camera_PitchUp", 2, pitch) ||
    helperSetHWControl(controlName, fRetVal, "Camera_YawLeft", "Camera_YawRight", 4, yaw) ||

    helperSetHWControl(controlName, fRetVal, "Camera_MoveLeft", "Camera_MoveRight", 6, x) ||
    helperSetHWControl(controlName, fRetVal, "Camera_MoveUp", "Camera_MoveDown", 8, y) ||
    helperSetHWControl(controlName, fRetVal, "Camera_MoveBackward", "Camera_MoveForward", 10, z)
    );
}

//
//bool ExampleInternalsPlugin::CheckHWControl(const char* const controlName, double& fRetVal) {
//  // only if enabled, of course
//  if(!IsEnabled()) {
//    return(false);
//  }
//
//  double x = mCamX;
//  double y = mCamY;
//  double z = mCamZ;
//
//  double roll = mCamRoll;
//  double pitch = mCamPitch;
//  double yaw = mCamYaw;
//
//  if(_stricmp(controlName, "Camera_RollRight") == 0) {
//    if(mDisableSequenceIndex == 0) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(roll > 0) {
//      fRetVal = roll;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_RollLeft") == 0) {
//    if(mDisableSequenceIndex == 1) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(roll < 0) {
//      fRetVal = -roll;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_PitchUp") == 0) {
//    if(mDisableSequenceIndex == 2) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(pitch > 0) {
//      fRetVal = pitch;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_PitchDown") == 0) {
//    if(mDisableSequenceIndex == 3) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(pitch < 0) {
//      fRetVal = -pitch;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveUp") == 0) {
//    if(mDisableSequenceIndex == 4) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(y > 0) {
//      fRetVal = y;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveDown") == 0) {
//    if(mDisableSequenceIndex == 5) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(y < 0) {
//      fRetVal = -y;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveRight") == 0) {
//    if(mDisableSequenceIndex == 6) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(x > 0) {
//      fRetVal = x;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveLeft") == 0) {
//    if(mDisableSequenceIndex == 7) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(x < 0) {
//      fRetVal = -x;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveForward") == 0) {
//    if(mDisableSequenceIndex == 8) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(z > 0) {
//      fRetVal = z;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_MoveBackward") == 0) {
//    if(mDisableSequenceIndex == 9) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(z < 0) {
//      fRetVal = -z;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_YawRight") == 0) {
//    if(mDisableSequenceIndex == 10) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(yaw > 0) {
//      fRetVal = yaw;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  } else if(_stricmp(controlName, "Camera_YawLeft") == 0) {
//    if(mDisableSequenceIndex == 11) {
//      fRetVal = 0.0;
//      mDisableSequenceIndex++;
//      return(true);
//    }
//
//    if(yaw < 0) {
//      fRetVal = -yaw;
//    } else {
//      fRetVal = 0.0;
//    }
//    return(true);
//  }
//
//  return(false);
//
//}

bool ExampleInternalsPlugin::ForceFeedback(double& forceValue) {
  return(false);
  // Note that incoming value is the game's computation, in case you're interested.
#if 0 // enable to log it out (note that this is a very very slow implementation)
  FILE* fo = fopen("FFB.txt", "a");
  if(fo != NULL) {
    fprintf(fo, "\nFFB=%.4f", forceValue);
    fclose(fo);
  }
#endif

  // CHANGE COMMENTS TO ENABLE FORCE EXAMPLE
  return(false);

  // I think the bounds are -11500 to 11500 ...
//  forceValue = 11500.0 * sinf( mET );
//  return( true );
}


void ExampleInternalsPlugin::UpdateScoring(const ScoringInfoV01& info) {
  return;
  // Note: function is called twice per second now (instead of once per second in previous versions)
  FILE* fo = fopen("ExampleInternalsScoringOutput.txt", "a");
  if(fo != NULL) {
    // Print general scoring info
    fprintf(fo, "TrackName=%s\n", info.mTrackName);
    fprintf(fo, "Session=%d NumVehicles=%d CurET=%.3f\n", info.mSession, info.mNumVehicles, info.mCurrentET);
    fprintf(fo, "EndET=%.3f MaxLaps=%d LapDist=%.1f\n", info.mEndET, info.mMaxLaps, info.mLapDist);

    // Note that only one plugin can use the stream (by enabling scoring updates) ... sorry if any clashes result
    fprintf(fo, "START STREAM\n");
    const char* ptr = info.mResultsStream;
    while(*ptr != NULL)
      fputc(*ptr++, fo);
    fprintf(fo, "END STREAM\n");

    // New version 2 stuff
    fprintf(fo, "GamePhase=%d YellowFlagState=%d SectorFlags=(%d,%d,%d)\n", info.mGamePhase, info.mYellowFlagState,
      info.mSectorFlag[0], info.mSectorFlag[1], info.mSectorFlag[2]);
    fprintf(fo, "InRealtime=%d StartLight=%d NumRedLights=%d\n", info.mInRealtime, info.mStartLight, info.mNumRedLights);
    fprintf(fo, "PlayerName=%s PlrFileName=%s\n", info.mPlayerName, info.mPlrFileName);
    fprintf(fo, "DarkCloud=%.2f Raining=%.2f AmbientTemp=%.1f TrackTemp=%.1f\n", info.mDarkCloud, info.mRaining, info.mAmbientTemp, info.mTrackTemp);
    fprintf(fo, "Wind=(%.1f,%.1f,%.1f) MinPathWetness=%.2f MaxPathWetness=%.2f\n", info.mWind.x, info.mWind.y, info.mWind.z, info.mMinPathWetness, info.mMaxPathWetness);

    // Print vehicle info
    for(long i = 0; i < info.mNumVehicles; ++i) {
      VehicleScoringInfoV01& vinfo = info.mVehicle[i];
      fprintf(fo, "Driver %d: %s\n", i, vinfo.mDriverName);
      fprintf(fo, " ID=%d Vehicle=%s\n", vinfo.mID, vinfo.mVehicleName);
      fprintf(fo, " Laps=%d Sector=%d FinishStatus=%d\n", vinfo.mTotalLaps, vinfo.mSector, vinfo.mFinishStatus);
      fprintf(fo, " LapDist=%.1f PathLat=%.2f RelevantTrackEdge=%.2f\n", vinfo.mLapDist, vinfo.mPathLateral, vinfo.mTrackEdge);
      fprintf(fo, " Best=(%.3f, %.3f, %.3f)\n", vinfo.mBestSector1, vinfo.mBestSector2, vinfo.mBestLapTime);
      fprintf(fo, " Last=(%.3f, %.3f, %.3f)\n", vinfo.mLastSector1, vinfo.mLastSector2, vinfo.mLastLapTime);
      fprintf(fo, " Current Sector 1 = %.3f, Current Sector 2 = %.3f\n", vinfo.mCurSector1, vinfo.mCurSector2);
      fprintf(fo, " Pitstops=%d, Penalties=%d\n", vinfo.mNumPitstops, vinfo.mNumPenalties);

      // New version 2 stuff
      fprintf(fo, " IsPlayer=%d Control=%d InPits=%d LapStartET=%.3f\n", vinfo.mIsPlayer, vinfo.mControl, vinfo.mInPits, vinfo.mLapStartET);
      fprintf(fo, " Place=%d VehicleClass=%s\n", vinfo.mPlace, vinfo.mVehicleClass);
      fprintf(fo, " TimeBehindNext=%.3f LapsBehindNext=%d\n", vinfo.mTimeBehindNext, vinfo.mLapsBehindNext);
      fprintf(fo, " TimeBehindLeader=%.3f LapsBehindLeader=%d\n", vinfo.mTimeBehindLeader, vinfo.mLapsBehindLeader);
      fprintf(fo, " Pos=(%.3f,%.3f,%.3f)\n", vinfo.mPos.x, vinfo.mPos.y, vinfo.mPos.z);

      // Forward is roughly in the -z direction (although current pitch of car may cause some y-direction velocity)
      fprintf(fo, " LocalVel=(%.2f,%.2f,%.2f)\n", vinfo.mLocalVel.x, vinfo.mLocalVel.y, vinfo.mLocalVel.z);
      fprintf(fo, " LocalAccel=(%.1f,%.1f,%.1f)\n", vinfo.mLocalAccel.x, vinfo.mLocalAccel.y, vinfo.mLocalAccel.z);

      // Orientation matrix is left-handed
      fprintf(fo, " [%6.3f,%6.3f,%6.3f]\n", vinfo.mOri[0].x, vinfo.mOri[0].y, vinfo.mOri[0].z);
      fprintf(fo, " [%6.3f,%6.3f,%6.3f]\n", vinfo.mOri[1].x, vinfo.mOri[1].y, vinfo.mOri[1].z);
      fprintf(fo, " [%6.3f,%6.3f,%6.3f]\n", vinfo.mOri[2].x, vinfo.mOri[2].y, vinfo.mOri[2].z);
      fprintf(fo, " LocalRot=(%.3f,%.3f,%.3f)\n", vinfo.mLocalRot.x, vinfo.mLocalRot.y, vinfo.mLocalRot.z);
      fprintf(fo, " LocalRotAccel=(%.2f,%.2f,%.2f)\n", vinfo.mLocalRotAccel.x, vinfo.mLocalRotAccel.y, vinfo.mLocalRotAccel.z);
    }

    // Delimit sections
    fprintf(fo, "\n");

    // Close file
    fclose(fo);
  }
}


bool ExampleInternalsPlugin::RequestCommentary(CommentaryRequestInfoV01& info) {
  // COMMENT OUT TO ENABLE EXAMPLE
  return(false);
}

