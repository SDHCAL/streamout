/**
*  \file ROOTtreeDest.h
*  \copyright 2022 G.Grenier F.Lagarde
*/

#pragma once

#include "Buffer.h"
#include "DIFPtr.h"
#include "Event.h"
#include "Interface.h"

#include <TFile.h>
#include <TTree.h>
#include <string>
#include <vector>

class ROOTWriter : public InterfaceWriter
{
public:
  ROOTWriter();

  void setFilename(const std::string&);

  void start();
  void processDIF(const DIFPtr&);
  void processFrame(const DIFPtr&, const std::uint32_t& frameIndex);
  void processPadInFrame(const DIFPtr&, const std::uint32_t& frameIndex, const std::uint32_t& channelIndex);
  void processSlowControl(const Buffer&) { ; }
  void end();

  virtual void startEvent();
  virtual void endEvent();
  virtual void startDIF();
  virtual void endDIF();
  virtual void startFrame();
  virtual void endFrame();
  virtual void startPad();
  virtual void endPad();

private:
  TFile*      m_File{nullptr};
  TTree*      m_Tree{nullptr};
  Event*      m_Event{nullptr};
  DIF*        m_DIF{nullptr};
  Hit*        m_Hit{nullptr};
  std::string m_Filename;
};
