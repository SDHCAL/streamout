#ifndef _DIFSlowControl_H
#define _DIFSlowControl_H
#include <map>
#include <string>
#include <bitset>
#include <iostream>
#include <stdio.h>
#include "DIFPtr.h"
/**
\class DIFSlowControl
  \author  L.Mirabito
  \date March 2010
  \version 1.0

 \brief Handler of DIF Slow Control  info

*/
class DIFSlowControl
{
public:
  //! Constructor
  /**
     @param version Data format version
     @param DIdi DIF id
     @param buf Pointer to the Raw data buffer
   */
  DIFSlowControl(unsigned int version,unsigned short DIdi,unsigned char *buf);

  //! Default Cosntructor
  DIFSlowControl(){}
  //! get DIF id
  unsigned short getDIFId() { return _DIFId;}

  //! Get chips map
  /**
     @return a map of < Asic Id, map of <string (parameter name),int (parameter value) >
   */
  std::map < int,std::map <std::string,int> > getChipsMap(){ return _mapSC;}

  //! Get one chip map
  /**
     @param asicid ASIC ID
     @return a map of <string (parameter name),int (parameter value) >
   */
  std::map<std::string,int> getChipSlowControl(int asicid){ return _mapSC[asicid];}

  //! Get one Chip value
  /**
     @param asicid ASic ID
     @param param Parameter name
   */
  int getChipSlowControl(int asicid,std::string param) { return getChipSlowControl(asicid)[param];}

  //! print out full map
  void Dump();
private:
  //! Fill hardROC 1 map
  void FillHR1(int header_shift,unsigned char *cbuf);
  //! Fill hardRoc 2 map
  void FillHR2(int header_shift,unsigned char *cbuf);
  //! read Asic HR1 type
  void FillAsicHR1(std::bitset<72*8> &bs);

  //! read Asic HR2 Type
  void FillAsicHR2(std::bitset<109*8> &bs);


  unsigned short			_DIFId; //! DIF Id
  unsigned int _version; //! version
  unsigned int _asicType;// asicType_
  unsigned int _nAsic; //! Number of Asic
  std::map< int,std::map < std::string,int > >        _mapSC; //! Storage map (asic,name,value)
};
#endif
