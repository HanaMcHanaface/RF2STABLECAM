//???????????????????????????????????????????????????????????????????????????
//?                                                                         ?
//? Module: Internals Example Source File                                   ?
//?                                                                         ?
//? Description: Declarations for the Internals Example Plugin              ?
//?                                                                         ?
//?                                                                         ?
//? This source code module, and all information, data, and algorithms      ?
//? associated with it, are part of CUBE technology (tm).                   ?
//?                 PROPRIETARY AND CONFIDENTIAL                            ?
//? Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. ?
//?                                                                         ?
//?                                                                         ?
//? Change history:                                                         ?
//?   tag.2005.11.30: created                                               ?
//?                                                                         ?
//???????????????????????????????????????????????????????????????????????????

#include "Example.hpp"          // corresponding header file
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include "EditConfig.hpp"

using namespace std;

// plugin information

extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return("RF2STABLECAM - 2022.10.20"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(7); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject*) new ExampleInternalsPlugin); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject * obj) {
  delete((ExampleInternalsPlugin*)obj);

  // Placing these here seems to work the best. 
  // EditPluginConfig() wouldn't work anywhere else.
  EditPlayerConfig();
  EditPluginConfig();
}

// ExampleInternalsPlugin class

ExampleInternalsPlugin::ExampleInternalsPlugin() {

}

void WriteToAllExampleOutputFiles(const char* const openStr, const char* const msg) {
  //FILE* fo;

  //fo = fopen("STABLECAM_Output.txt", openStr);
  //if(fo != NULL) {
  //  fprintf(fo, "%s\n", msg);
  //  fclose(fo);
  //}
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

  mCamPitch = 0.0;
  mCamRoll = 0.0;
  mCamYaw = 0.0;

  mCamX = 0.0;
  mCamY = 0.0;
  mCamZ = 0.0;

  mAveragePitch.reset();
  mAverageRoll.reset();

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
  TelemVect3 leftVector = { telemetry.mOri[0].x,  telemetry.mOri[1].x,  telemetry.mOri[2].x };

  const double carPitch = atan2(forwardVector.y, sqrt((forwardVector.x * forwardVector.x) + (forwardVector.z * forwardVector.z)));
  const double carRoll = atan2(leftVector.y, sqrt((leftVector.x * leftVector.x) + (leftVector.z * leftVector.z)));
  //const double carYaw = atan2(telemetry.mOri[2].x, telemetry.mOri[2].z);

  TelemVect3 carGlobalPos = telemetry.mPos;

  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  roll = -carRoll;
  pitch = -carPitch;
  //yaw = -carYaw;

  double averageRoll = mAverageRoll.GetAverage(carRoll, 0.2);
  double averagePitch = mAveragePitch.GetAverage(carPitch, 0.2);

  roll += averageRoll;
  pitch += averagePitch;

  y = mAverageYAccel.GetAverage(telemetry.mLocalAccel.y, 0.05) * 0.017;

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
    helperSetHWControl(controlName, fRetVal, "Camera_MoveDown", "Camera_MoveUp", 8, y) ||
    helperSetHWControl(controlName, fRetVal, "Camera_MoveBackward", "Camera_MoveForward", 10, z)
    );
}

bool ExampleInternalsPlugin::ForceFeedback(double& forceValue) {
  return(false);
}

void ExampleInternalsPlugin::UpdateScoring(const ScoringInfoV01& info) {
  return;
}

bool ExampleInternalsPlugin::RequestCommentary(CommentaryRequestInfoV01& info) {
  return(false);
}
