/**
*  \file ROOTWriter.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "ROOTWriter.h"

void ROOTWriter::setFilename(const std::string& filename) { m_Filename = filename; }

ROOTWriter::ROOTWriter() : InterfaceWriter("ROOTWriter", "1.0.0") { addCompatibility("RawdataReader", ">=1.0.0"); }

void ROOTWriter::start(const VersionInfos& ver)
{
  m_File = TFile::Open(m_Filename.c_str(), "RECREATE", m_Filename.c_str(), ROOT::CompressionSettings(ROOT::kZLIB, 5));
  m_Tree = new TTree("RawData", "Raw SDHCAL data tree");
  m_Tree->Branch("Events", &m_Event, 512000, 99);
}

void ROOTWriter::end()
{
  if(m_Tree) m_Tree->Write();
  if(m_File)
  {
    m_File->Write();
    m_File->Close();
  }
  if(m_File) delete m_File;
}

void ROOTWriter::processDIF(const Payload& d)
{
  m_DIF->setDetectorID(d.getDetectorID());
  m_DIF->setID(d.getDIFid());
  m_DIF->setDTC(d.getDTC());
  m_DIF->setGTC(d.getGTC());
  m_DIF->setDIFBCID(d.getBCID());
  m_DIF->setAbsoluteBCID(d.getAbsoluteBCID());
}

void ROOTWriter::processFrame(const Payload& d, const std::uint32_t& frameIndex)
{
  m_Hit->setDIF(d.getDIFid());
  m_Hit->setASIC(d.getASICid(frameIndex));
  m_Hit->setDTC(d.getDTC());
  m_Hit->setGTC(d.getGTC());
  m_Hit->setDIFBCID(d.getBCID());
  m_Hit->setAbsoluteBCID(d.getAbsoluteBCID());
  m_Hit->setFrameBCID(d.getFrameBCID(frameIndex));
  m_Hit->setTimestamp(d.getFrameTimeToTrigger(frameIndex));
}

void ROOTWriter::processPadInFrame(const Payload& d, const std::uint32_t& frameIndex, const std::uint32_t& channelIndex)
{
  m_Hit->setChannel(channelIndex);
  m_Hit->setThreshold(static_cast<std::uint8_t>(d.getThresholdStatus(frameIndex, channelIndex)));
}

void ROOTWriter::startEvent()
{
  m_Event = new Event();
  m_Event->setEventNumber(getEventNumber());
  // m_Event->clear();
}

void ROOTWriter::endEvent()
{
  m_Tree->Fill();
  if(m_Event) delete m_Event;
}

void ROOTWriter::startDIF()
{
  m_DIF = new DIF();
  // m_DIF->clear();
}

void ROOTWriter::endDIF()
{
  m_Event->addDIF(*m_DIF);
  delete m_DIF;
}

void ROOTWriter::startFrame()
{
  m_Hit = new Hit();
  // m_Hit->clear();
}

void ROOTWriter::endFrame()
{
  m_DIF->addHit(*m_Hit);
  delete m_Hit;
}

void ROOTWriter::startPad() {}

void ROOTWriter::endPad() {}
