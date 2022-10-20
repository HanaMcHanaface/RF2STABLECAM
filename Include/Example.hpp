//ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
//Ý                                                                         Þ
//Ý Module: Internals Example Header File                                   Þ
//Ý                                                                         Þ
//Ý Description: Declarations for the Internals Example Plugin              Þ
//Ý                                                                         Þ
//Ý                                                                         Þ
//Ý This source code module, and all information, data, and algorithms      Þ
//Ý associated with it, are part of CUBE technology (tm).                   Þ
//Ý                 PROPRIETARY AND CONFIDENTIAL                            Þ
//Ý Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. Þ
//Ý                                                                         Þ
//Ý                                                                         Þ
//Ý Change history:                                                         Þ
//Ý   tag.2005.11.30: created                                               Þ
//Ý                                                                         Þ
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

#ifndef _INTERNALS_EXAMPLE_H
#define _INTERNALS_EXAMPLE_H

#include "InternalsPlugin.hpp"
#include "TimedMovingAverage.hpp"
#include "TimeDiffer.hpp"

// This is used for the app to use the plugin for its intended purpose
class ExampleInternalsPlugin : public InternalsPluginV07  // REMINDER: exported function GetPluginVersion() should return 1 if you are deriving from this InternalsPluginV01, 2 for InternalsPluginV02, etc.
{

public:

  // Constructor/destructor
  ExampleInternalsPlugin();
  ~ExampleInternalsPlugin() {}

  // These are the functions derived from base class InternalsPlugin
  // that can be implemented.
  void Startup(long version);  // game startup
  void Shutdown();               // game shutdown

  void EnterRealtime();          // entering realtime
  void ExitRealtime();           // exiting realtime

  void StartSession();           // session has started
  void EndSession();             // session has ended

  // GAME OUTPUT
  long WantsTelemetryUpdates() { return(1); } // CHANGE TO 1 TO ENABLE TELEMETRY EXAMPLE!
  void UpdateTelemetry(const TelemInfoV01& info);

  bool WantsGraphicsUpdates() { return(true); } // CHANGE TO TRUE TO ENABLE GRAPHICS EXAMPLE!
  void UpdateGraphics(const GraphicsInfoV02& info);

  // GAME INPUT
  bool HasHardwareInputs() { return(true); } // CHANGE TO TRUE TO ENABLE HARDWARE EXAMPLE!
  void UpdateHardware(const double fDT) {} // update the hardware with the time between frames
  void EnableHardware() {/* mHardwareEnabled = true;*/ }             // message from game to enable hardware
  void DisableHardware() { /*mHardwareEnabled = false;*/ }           // message from game to disable hardware

  // See if the plugin wants to take over a hardware control.  If the plugin takes over the
  // control, this method returns true and sets the value of the double pointed to by the
  // second arg.  Otherwise, it returns false and leaves the double unmodified.
  bool CheckHWControl(const char* const controlName, double& fRetVal);

  bool ForceFeedback(double& forceValue);  // SEE FUNCTION BODY TO ENABLE FORCE EXAMPLE

  // SCORING OUTPUT
  bool WantsScoringUpdates() { return(false); } // CHANGE TO TRUE TO ENABLE SCORING EXAMPLE!
  void UpdateScoring(const ScoringInfoV01& info);

  // COMMENTARY INPUT
  bool RequestCommentary(CommentaryRequestInfoV01& info);  // SEE FUNCTION BODY TO ENABLE COMMENTARY EXAMPLE

private:
  bool helperSetHWControl(
    const char* const controlName, double& fRetVal,
    const char* const negativeCompareName, const char* const positiveCompareName,
    int index, double targetVal
  );
  bool IsEnabled() { return(mEnabled && mHardwareEnabled && mCameraType >= 0 && mCameraType <= 2); }

  int mDisableSequenceIndex = -1;

  bool mEnabled = false;
  bool mHardwareEnabled = true;
  long mCameraType = -1;

  double mCamPitch = 0.0;
  double mCamRoll = 0.0;
  double mCamYaw = 0.0;

  double mCamX = 0.0;
  double mCamY = 0.0;
  double mCamZ = 0.0;

  TimedMovingAverage mAveragePitch;
  TimedMovingAverage mAverageRoll;

  TimedMovingAverage mAverageYAccel;
};

#endif // _INTERNALS_EXAMPLE_H
