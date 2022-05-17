/**
*  \file ROOTtreeDest.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"
#include "Interface.h"
#include "TTree.h"

class ROOTtreeDest : public Interface
{
public:
  typedef struct
  {
    UInt_t    DIFid, ASICid, CHANNELid;
    UInt_t    Thresh;
    UInt_t    DTC, GTC, DIF_BCID, frame_BCID, timeStamp;
    ULong64_t AbsoluteBCID;
  } DATA;

  ROOTtreeDest();

  void start();
  void processDIF(DIFPtr*);
  void processFrame(DIFPtr*, std::uint32_t frameIndex);
  void processPadInFrame(DIFPtr*, std::uint32_t frameIndex, std::uint32_t channelIndex);
  void processSlowControl(const Buffer&) { ; }
  void end() { ; }

private:
  DATA   _data;
  TTree* _tree;
  void   dataReset();
};
