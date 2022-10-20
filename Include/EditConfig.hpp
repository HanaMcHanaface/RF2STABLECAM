
#pragma once

#include <fstream>
#include "Log.hpp"

using namespace std;

bool EditConfigStr(string& source, bool& didChange, string regexStr, string numberNoChangeStr, string replaceStr) {
  regex rgx(regexStr);
  smatch result;

  const string& matchStr = source;

  if(!regex_search(matchStr.begin(), matchStr.end(), result, rgx)) {
    return(false);
  }

  const string& resultStr = result.str();
  const string& resultNoWhitespaceStr = regex_replace(resultStr, regex(R"(\s+)"), "");
  smatch numberMatch;

  if(!regex_search(resultNoWhitespaceStr.begin(), resultNoWhitespaceStr.end(), numberMatch, regex(R"([0-9\s]+$)"))) {
    return(false);
  }

  if(numberMatch.str() == numberNoChangeStr) {
    return(false);
  }

  WriteToLog("## Replacing config: \n" + resultStr + "\n## With: \n" + replaceStr + "\n## End of replace.");
  source = regex_replace(source, rgx, replaceStr);
  didChange = true;
  return(true);
}

class EditConfig {
public:
  EditConfig(string _fileName) : fileName(_fileName) {
    ostringstream text;
    ifstream in_file(fileName);

    text << in_file.rdbuf();

    str = text.str();
    readSuccess = true;
  }

  void write() {
    if(!readSuccess) {
      return;
    }

    WriteToLog("## Writing to " + fileName);
    ofstream out_file(fileName);
    out_file << str;
    WriteToLog("## Finished writing to " + fileName);
  }

  string str = "";

private:
  string fileName = "";
  bool readSuccess = false;
};

void EditPlayerConfig() {
  EditConfig ec("./UserData/player/player.JSON");

  bool didChange = false;

  EditConfigStr(ec.str, didChange, R"("Glance Rate"[:0-9\s\.]+)", "500", R"("Glance Rate":500)");
  EditConfigStr(ec.str, didChange, R"("Stabilize Horizon"[:0-9\s\.]+)", "0", R"("Stabilize Horizon":0)");
  EditConfigStr(ec.str, didChange, R"("Look Up/Down Angle"[:0-9\s\.]+)", "1", R"("Look Up/Down Angle":1)");
  EditConfigStr(ec.str, didChange, R"("Look Roll Angle"[:0-9\s\.]+)", "1", R"("Look Roll Angle":1)");

  if(!didChange) { return; }

  ec.write();
}

void EditPluginConfig() {
  EditConfig ec("./UserData/player/CustomPluginVariables.JSON");

  bool didChange = false;

  EditConfigStr(ec.str, didChange,
    R"("TrackIR_rF2_Plugin\.dll"[\s:\{]+" Enabled"[:0-9\s\.]+)",
    "0",
    R"("TrackIR_rF2_Plugin.dll":{
    " Enabled":0
  )"
  );

  if(!didChange) { return; }

  ec.write();
}
