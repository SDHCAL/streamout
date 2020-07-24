#ifndef ROOTtreeDest_h
#define ROOTtreeDest_h

#include "DIFPtr.h"
#include "SDHCAL_buffer.h"
#include "TTree.h"

class ROOTtreeDest
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
  void processFrame(DIFPtr*, uint32_t frameIndex);
  void processPadInFrame(DIFPtr*, uint32_t frameIndex, uint32_t channelIndex);
  void processSlowControl(SDHCAL_buffer) { ; }
  void end() { ; }

private:
  DATA   _data;
  TTree* _tree;
  void   dataReset();
};

#endif
