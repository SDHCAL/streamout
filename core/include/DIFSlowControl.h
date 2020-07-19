#pragma once

#include <map>
#include <string>
#include <bitset>

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
  DIFSlowControl(const unsigned int& version,const unsigned short& DIdi,unsigned char *buf);


  //! get DIF id
  inline unsigned short getDIFId();

  //! Get chips map
  /**
     @return a map of < Asic Id, map of <string (parameter name),int (parameter value) >
   */
  inline std::map<int,std::map<std::string,int>> getChipsMap();

  //! Get one chip map
  /**
     @param asicid ASIC ID
     @return a map of <string (parameter name),int (parameter value) >
   */
  inline std::map<std::string,int> getChipSlowControl(const int& asicid);

  //! Get one Chip value
  /**
     @param asicid ASic ID
     @param param Parameter name
   */
  inline int getChipSlowControl(const int& asicid,const std::string& param);

  //! print out full map
  void Dump();
private:
  //! Default Cosntructor
  DIFSlowControl()=delete;
  //! Fill hardROC 1 map
  void FillHR1(const int& header_shift,unsigned char *cbuf);
  //! Fill hardRoc 2 map
  void FillHR2(const int& header_shift,unsigned char *cbuf);
  //! read Asic HR1 type
  void FillAsicHR1(const std::bitset<72*8>& bs);
  //! read Asic HR2 Type
  void FillAsicHR2(const std::bitset<109*8>& bs);


  unsigned short m_DIFId{0}; //! DIF Id
  unsigned int m_Version{0}; //! version
  unsigned int m_AsicType{0};// asicType_
  unsigned int m_NbrAsic{0}; //! Number of Asic
  std::map<int,std::map<std::string,int>> m_MapSC; //! Storage map (asic,name,value)
};

