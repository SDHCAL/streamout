#pragma once

#include "SDHCAL_buffer.h"
#include "SDHCAL_buffer_LoopCounter.h"
#include "SDHCAL_RawBuffer_Navigator.h"
#include <cassert>
#include <ostream>
//function to loop on buffers
//
// template class should implement
// bool SOURCE::next();
// SDHCAL_buffer SOURCE::getSDHCALBuffer();
//
// void DESTINATION::start();
// void DESTINATION::processDIF(DIFPtr*);
// void DESTINATION::processFrame(DIFPtr*,uint32_t frameIndex);
// void DESTINATION::processPadInFrame(DIFPtr*,uint32_t frameIndex, uint32_t channelIndex);
// void DESTINATION::processSlowControl(SDHCAL_buffer);
// void DESTINATION::end();
//

template <typename SOURCE, typename DESTINATION>
void SDHCAL_buffer_loop( SOURCE &source, DESTINATION &dest, bool debug=false, std::ostream& out=std::cout, bool verbose=false, std::ostream& verbose_out=std::cout)
{
  dest.start();
  SDHCAL_buffer_LoopCounter c;
  
  while ( source.next() )
  {
    SDHCAL_buffer buffer=source.getSDHCALBuffer();
    unsigned char* debug_variable_1=buffer.endOfBuffer();
    SDHCAL_RawBuffer_Navigator bufferNavigator(buffer);
    unsigned char* debug_variable_2=bufferNavigator.getDIFBuffer().endOfBuffer();
    if (verbose) verbose_out << "DIF BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_2 << std::endl;
    if (debug) assert (debug_variable_1 == debug_variable_2);
    uint32_t idstart=bufferNavigator.getStartOfDIF();
    if (debug && idstart==0) buffer.printBuffer();
    c.DIFStarter[idstart]++;
    if (!  bufferNavigator.validBuffer() ) continue;
    DIFPtr *d=bufferNavigator.getDIFPtr();
    if (debug) assert(d!=NULL);
    if (d!=NULL)
    {
      c.DIFPtrValueAtReturnedPos[bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()] ]++;
      if (debug) assert( bufferNavigator.getDIFBufferStart()[d->getGetFramePtrReturn()]==0xa0 );
    }
    c.SizeAfterDIFPtr[bufferNavigator.getSizeAfterDIFPtr()]++;
    dest.processDIF(d);
    for (uint32_t i=0;i<d->getNumberOfFrames();i++)
    {
      dest.processFrame(d,i);
      for (uint32_t j=0;j<64;j++) dest.processPadInFrame(d,i,j);
    }
    
    bool processSC=false;
    if (bufferNavigator.hasSlowControlData()) {c.hasSlowControl++;processSC=true;}
    if (bufferNavigator.badSCData()) {c.hasBadSlowControl++;processSC=false;}
    if (processSC) {dest.processSlowControl(bufferNavigator.getSCBuffer());}
    
    SDHCAL_buffer eod=bufferNavigator.getEndOfAllData();
    c.SizeAfterAllData[eod.getsize()]++;
    unsigned char* debug_variable_3=eod.endOfBuffer();
    if (verbose) verbose_out << "END DATA BUFFER END " << (unsigned int *) debug_variable_1 << " " << (unsigned int *) debug_variable_3 << std::endl;
    if (debug) assert (debug_variable_1 == debug_variable_3);
    if (verbose) {verbose_out << "End of Data remaining stuff : "; eod.printBuffer();}
    
    int nonzeroCount=0;
    for (unsigned char* it=eod.buffer(); it != eod.endOfBuffer(); it++)
      if (int(*it) !=0) nonzeroCount++;
      c.NonZeroValusAtEndOfData[nonzeroCount]++;
  } //end of while loop
  
  dest.end();
  c.printAllCounters(out);
}
