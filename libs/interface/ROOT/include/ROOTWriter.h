/**
*  \file ROOTtreeDest.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"
#include "Interface.h"
#include "Event.h"

#include <TTree.h>
#include <TFile.h>

class ROOTWritter : public Interface
{
public:
  typedef struct
  {
    UInt_t    DIFid, ASICid, CHANNELid;
    UInt_t    Thresh;
    UInt_t    DTC, GTC, DIF_BCID, frame_BCID, timeStamp;
    ULong64_t AbsoluteBCID;
  } DATA;

  ROOTWritter();

  void start();
  void processDIF(const DIFPtr&);
  void processFrame(const DIFPtr&, const std::uint32_t& frameIndex);
  void processPadInFrame(const DIFPtr&, const std::uint32_t& frameIndex, const std::uint32_t& channelIndex);
  void processSlowControl(const Buffer&) { ; }
  void end() { ; }

private:
  TFile* m_File{nullptr};
  TTree* _tree{nullptr};
  DATA   _data;
  void   dataReset();
};
