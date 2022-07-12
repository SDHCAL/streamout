/** \file DIFSlowControl.h
 *  \copyright 2022 G.Grenier F.Lagarde
 */

#pragma once

#include <bitset>
#include <cstdint>
#include <iosfwd>
#include <map>
#include <string>

class DIFSlowControl
{
public:
  //! Constructor
  /**
    @param version Data format version
    @param DIFid DIF id
    @param buf Pointer to the Raw data buffer
   */
  DIFSlowControl(const std::uint8_t& version, const std::uint8_t& DIFid, unsigned char* buf);

  //! get DIF id
  std::uint8_t getDIFId();

  //! Get chips map
  /**
    @return a map of < Asic Id, map of <string (parameter name),int (parameter value) >
   */
  std::map<int, std::map<std::string, int>> getChipsMap();

  //! Get one chip map
  /**
    @param asicid ASIC ID
    @return a map of <string (parameter name),int (parameter value) >
   */
  std::map<std::string, int> getChipSlowControl(const int& asicid);

  //! Get one Chip value
  /**
    @param asicid ASic ID
    @param param Parameter name
   */
  int getChipSlowControl(const std::int8_t& asicid, const std::string& param);

  std::map<int, std::map<std::string, int>>::const_iterator cbegin() const { return m_MapSC.cbegin(); }

  std::map<int, std::map<std::string, int>>::const_iterator cend() const { return m_MapSC.cend(); }

private:
  //! Default Constructor
  DIFSlowControl() = delete;
  //! Fill hardROC 1 map
  void FillHR1(const int& header_shift, unsigned char* cbuf);
  //! Fill hardRoc 2 map
  void FillHR2(const int& header_shift, unsigned char* cbuf);
  //! read Asic HR1 type
  void FillAsicHR1(const std::bitset<72 * 8>& bs);
  //! read Asic HR2 Type
  void FillAsicHR2(const std::bitset<109 * 8>& bs);

  unsigned int                              m_DIFId{0};     //! DIF Id
  unsigned int                              m_Version{0};   //! version
  unsigned int                              m_AsicType{0};  // asicType_
  unsigned int                              m_NbrAsic{0};   //! Number of Asic
  std::map<int, std::map<std::string, int>> m_MapSC;        //! Storage map (asic,name,value)
};

std::string to_string(const DIFSlowControl& c);
/* void setSCBuffer()
  {
    if(!hasSlowControl()) return;
    if(m_SCbuffer.size() != 0) return;  // deja fait
    if(m_BadSlowControl) return;
    m_SCbuffer.set(&(begin()[getEndOfDIFData()]));
    // compute Slow Control size
    std::size_t maxsize{size() - getEndOfDIFData() + 1};  // should I +1 here ?
    uint32_t    k{1};                                     // SC Header
    uint32_t    dif_ID{m_SCbuffer[1]};
    uint32_t    chipSize{m_SCbuffer[3]};
    while((dif_ID != 0xa1 && m_SCbuffer[k] != 0xa1 && k < maxsize) || (dif_ID == 0xa1 && m_SCbuffer[k + 2] == chipSize && k < maxsize))
    {
      k += 2;  // DIF ID + ASIC Header
      uint32_t scsize = m_SCbuffer[k];
      if(scsize != 74 && scsize != 109)
      {
        k                = 0;
        m_BadSlowControl = true;
        throw Exception(fmt::format("PROBLEM WITH SC SIZE {}", scsize));
      }
      k++;          // skip size bit
      k += scsize;  // skip the data
    }
    if(m_SCbuffer[k] == 0xa1 && !m_BadSlowControl) m_SCbuffer.setSize(k + 1);  // add the trailer
    else
    {
      m_BadSlowControl = true;
      throw Exception(fmt::format("PROBLEM SC TRAILER NOT FOUND "));
    }
  }*/
