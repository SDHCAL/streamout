#include "DIFdataExample.h"


DIFdataExample::DIFdataExample() : _first(true)
{
  for (int i=0; i<60; ++i) _RAWbuffer[i]=0;
  int istuff=2;
  _RAWbuffer[istuff]=0xB0;  // start DIF data 
  _RAWbuffer[istuff+1]=133; // DIF ID 1 byte
  _RAWbuffer[istuff+5]=1;   // DTC    4 bytes
  _RAWbuffer[istuff+9]=1;   // Information Counter 4 bytes
  _RAWbuffer[istuff+13]=1;  // GTC    4 bytes
  _RAWbuffer[istuff+19]=1;  // Absolute BCID 6 bytes
  _RAWbuffer[istuff+22]=1;  // DIF BCID
  _RAWbuffer[istuff+23]=0;  //number of analog lines
  _RAWbuffer[istuff+24]=0xC4; //start analog RO info
  _RAWbuffer[istuff+25]=0xD4; //end analog RO info
  _RAWbuffer[istuff+26]=0xB4; //start digital RO info (frame header)
  // a frame is on 20 bytes
  _RAWbuffer[istuff+27]=3; //ASIC ID ? 1 byte
  _RAWbuffer[istuff+30]=1; //ASIC BCID (3 bytes)
  _RAWbuffer[istuff+35]=8; // put some threshold crossed among 128 bits (16 bytes)
  _RAWbuffer[istuff+47]=0xA3; // end  digital RO info
  _RAWbuffer[istuff+48]=0xA0; // end DIF data
}

bool DIFdataExample::next()
{
  if (_first) {_first=false; return true;}
  return false;
}

