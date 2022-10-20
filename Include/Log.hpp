#pragma once

using namespace std;

void WriteToLog(string msg) {
  ofstream out("./RF2STABLECAM_LOG.txt", std::ios::app);

  out << msg << "\n";
}
