#include "ROOTtreeDest.h"

ROOTtreeDest::ROOTtreeDest()
{
  dataReset();
  _tree = new TTree("RawData", "Raw SDHCAL data tree");
  _tree->Branch("data", &_data, "DIFid/i:ASICid:CHANNELid:Thresh:DTC:GTC:DIF_BCID:frame_BCID:timeStamp:AbsoluteBCID/l");
}

void ROOTtreeDest::dataReset()
{
  _data.DIFid = _data.ASICid = _data.CHANNELid = 0;
  _data.Thresh                                 = 0;
  _data.DTC = _data.GTC = _data.DIF_BCID = _data.frame_BCID = _data.timeStamp = 0;
  _data.AbsoluteBCID                                                          = 0;
}

void ROOTtreeDest::start() { dataReset(); }

void ROOTtreeDest::processDIF(DIFPtr* d)
{
  _data.DIFid        = d->getDIFid();
  _data.DTC          = d->getDTC();
  _data.GTC          = d->getGTC();
  _data.DIF_BCID     = d->getBCID();
  _data.AbsoluteBCID = d->getAbsoluteBCID();
}

void ROOTtreeDest::processFrame(DIFPtr* d, uint32_t frameIndex)
{
  _data.ASICid     = d->getASICid(frameIndex);
  _data.frame_BCID = d->getFrameBCID(frameIndex);
  _data.timeStamp  = d->getFrameTimeToTrigger(frameIndex);
}

void ROOTtreeDest::processPadInFrame(DIFPtr* d, uint32_t frameIndex, uint32_t channelIndex)
{
  _data.CHANNELid = channelIndex;
  _data.Thresh    = d->getThresholdStatus(frameIndex, channelIndex);
  if(_data.Thresh != 0) _tree->Fill();
}
