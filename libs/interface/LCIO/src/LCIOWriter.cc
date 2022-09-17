/** \file LCIOWriter.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/
/**
*  \file ROOTWriter.cc
*  \copyright 2022 G.Grenier F.Lagarde
*/

#include "LCIOWriter.h"

#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCParametersImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IOIMPL/LCFactory.h"
#include "LCIOSTLTypes.h"

void LCIOWriter::setFilename(const std::string& filename) { m_Filename = filename; }

LCIOWriter::LCIOWriter() : InterfaceWriter("LCIOWriter", "1.0.0"), m_LCWriter(IOIMPL::LCFactory::getInstance()->createLCWriter()) { addCompatibility("RawdataReader", ">=1.0.0"); }

void LCIOWriter::start()
{
  m_LCWriter->open(m_Filename, EVENT::LCIO::WRITE_NEW);
  std::unique_ptr<IMPL::LCRunHeaderImpl> runHdr(new IMPL::LCRunHeaderImpl);
  runHdr->setRunNumber(50);  // FIXME : provide run number
  runHdr->setDetectorName(m_DetectorName);
  std::string description("data collected with SDHCAL prototype");
  runHdr->setDescription(description);
  m_LCWriter->writeRunHeader(runHdr.get());
}

void LCIOWriter::end() { m_LCWriter->close(); }

void LCIOWriter::processDIF(const Payload& d)
{
  std::string   parameter_name = "DIF" + std::to_string(d.getDIFid()) + "_Triggers";
  EVENT::IntVec parameters;
  parameters.push_back(d.getDTC());
  parameters.push_back(d.getGTC());
  parameters.push_back(d.getBCID());
  parameters.push_back(d.getAbsoluteBCID() & 0xFFFFFF);
  parameters.push_back((d.getAbsoluteBCID() >> 24) & 0xFFFFFF);
  parameters.push_back(0);
  parameters.push_back(0);
  parameters.push_back(0);
  m_CollectionVec->parameters().setValues("DIF" + std::to_string(d.getDIFid()) + "_Triggers", parameters);
}

void LCIOWriter::processFrame(const Payload& d, const std::uint32_t& frameIndex) {}

void LCIOWriter::processPadInFrame(const Payload& d, const std::uint32_t& frameIndex, const std::uint32_t& channelIndex)
{
  IMPL::RawCalorimeterHitImpl* hit = new IMPL::RawCalorimeterHitImpl;
  int                          ID0 = channelIndex;
  ID0                              = ID0 << 8;
  ID0 += d.getASICid(frameIndex);
  ID0 = ID0 << 8;
  ID0 += d.getDIFid();
  hit->setCellID0(ID0);
  hit->setAmplitude(d.getThresholdStatus(frameIndex, channelIndex));
  hit->setTimeStamp(d.getFrameTimeToTrigger(frameIndex));
  m_CollectionVec->addElement(hit);
}

void LCIOWriter::startEvent()
{
  m_LCEvent = std::make_unique<IMPL::LCEventImpl>();
  m_LCEvent->setEventNumber(0);
  m_LCEvent->setDetectorName(m_DetectorName);
  m_LCEvent->setTimeStamp(0);
  m_LCEvent->setWeight(1);
  m_CollectionVec = new IMPL::LCCollectionVec(EVENT::LCIO::RAWCALORIMETERHIT);
  IMPL::LCFlagImpl flag(0);
  // flag.setBit(EVENT::LCIO::RCHBIT_ID1);
  flag.setBit(EVENT::LCIO::RCHBIT_TIME);
  m_CollectionVec->setFlag(flag.getFlag());
  m_CollectionVec->parameters().setValue(EVENT::LCIO::CellIDEncoding, "dif:8,asic:8,channel:6");
}

void LCIOWriter::endEvent()
{
  m_LCEvent->addCollection(m_CollectionVec, "DHCALRawHits");
  m_LCWriter->writeEvent(m_LCEvent.get());
}

void LCIOWriter::startDIF() {}

void LCIOWriter::endDIF() {}

void LCIOWriter::startFrame() {}

void LCIOWriter::endFrame() {}

void LCIOWriter::startPad() {}

void LCIOWriter::endPad() {}
