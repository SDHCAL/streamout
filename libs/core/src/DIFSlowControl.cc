#include "DIFSlowControl.h"

#include <iostream>

DIFSlowControl::DIFSlowControl(const unsigned int& version, const unsigned short& DIfId, unsigned char* cbuf): m_Version(version), m_DIFId(DIfId), m_AsicType(2)
{
  if(cbuf[0] != 0xb1) return;
  int header_shift{6};
  if(m_Version < 8) m_NbrAsic = cbuf[5];
  else
  {
    m_DIFId      = cbuf[1];
    m_NbrAsic    = cbuf[2];
    header_shift = 3;
  }
  int size_hardroc1 = m_NbrAsic * 72 + header_shift + 1;
  if(cbuf[size_hardroc1 - 1] != 0xa1) size_hardroc1 = 0;

  int size_hardroc2 = m_NbrAsic * 109 + header_shift + 1;
  if(cbuf[size_hardroc2 - 1] != 0xa1) size_hardroc2 = 0;
  if(size_hardroc1 != 0)
  {
    FillHR1(header_shift, cbuf);
    m_AsicType = 1;
  }
  else if(size_hardroc2 != 0)
    FillHR2(header_shift, cbuf);
  else
    return;
}

inline unsigned short DIFSlowControl::getDIFId()
{
  return m_DIFId;
}

inline std::map<int, std::map<std::string, int>> DIFSlowControl::getChipsMap()
{
  return m_MapSC;
}

inline std::map<std::string, int> DIFSlowControl::getChipSlowControl(const int& asicid)
{
  return m_MapSC[asicid];
}

inline int DIFSlowControl::getChipSlowControl(const int& asicid, const std::string& param)
{
  return getChipSlowControl(asicid)[param];
}

void DIFSlowControl::Dump()
{
  for(std::map<int, std::map<std::string, int>>::iterator it = m_MapSC.begin(); it != m_MapSC.end(); it++)
  {
    std::cout << "ASIC " << it->first << std::endl;
    for(std::map<std::string, int>::iterator jt = (it->second).begin(); jt != (it->second).end(); jt++) std::cout << jt->first << " : " << jt->second << std::endl;
  }
}

void DIFSlowControl::FillHR1(const int& header_shift, unsigned char* cbuf)
{
  int nasic{cbuf[header_shift - 1]};
  int idx{header_shift};
  for(int k = 0; k < nasic; k++)
  {
    std::bitset<72 * 8> bs;
    //printf("%x %x \n",cbuf[idx+k*72+69],cbuf[idx+k*72+70]);
    for(int l = 71; l >= 0; l--)
    {
      //  printf("%d %x : %d -->",l,cbuf[idx+k*72+l],(71-l)*8);
      for(int m = 0; m < 8; m++)
      {
        if(((1 << m) & cbuf[idx + k * 72 + l]) != 0) bs.set((71 - l) * 8 + m, 1);
        else
          bs.set((71 - l) * 8 + m, 0);
        //printf("%d",(int) bs[(71-l)*8+m]);
      }
      //printf("\n");
    }
    FillAsicHR1(bs);
  }
}

void DIFSlowControl::FillHR2(const int& header_shift, unsigned char* cbuf)
{
  // int scsize1=cbuf[header_shift-1]*109+(header_shift-1)+2;
  int nasic{cbuf[header_shift - 1]};
  int idx{header_shift};
  //std::cout<<" DIFSlowControl::FillHR nasic "<<nasic<<std::endl;
  for(int k = 0; k < nasic; k++)
  {
    std::bitset<109 * 8> bs;
    //printf("%x %x \n",cbuf[idx+k*109+69],cbuf[idx+k*109+70]);
    for(int l = 108; l >= 0; l--)
    {
      //  printf("%d %x : %d -->",l,cbuf[idx+k*109+l],(71-l)*8);
      for(int m = 0; m < 8; m++)
      {
        if(((1 << m) & cbuf[idx + k * 109 + l]) != 0) bs.set((108 - l) * 8 + m, 1);
        else
          bs.set((108 - l) * 8 + m, 0);
        //printf("%d",(int) bs[(71-l)*8+m]);
      }
      //printf("\n");
    }
    FillAsicHR2(bs);
  }
}

void DIFSlowControl::FillAsicHR1(const std::bitset<72 * 8>& bs)
{
  // Asic Id
  int asicid{0};
  for(int j = 0; j < 8; j++)
    if(bs[j + 9] != 0) asicid += (1 << (7 - j));
  std::map<std::string, int> mAsic;
  // Slow Control
  mAsic["SSC0"]          = static_cast<int>(bs[575]);
  mAsic["SSC1"]          = static_cast<int>(bs[574]);
  mAsic["SSC2"]          = static_cast<int>(bs[573]);
  mAsic["Choix_caisson"] = static_cast<int>(bs[572]);
  mAsic["SW_50k"]        = static_cast<int>(bs[571]);
  mAsic["SW_100k"]       = static_cast<int>(bs[570]);
  mAsic["SW_100f"]       = static_cast<int>(bs[569]);
  mAsic["SW_50f"]        = static_cast<int>(bs[568]);

  mAsic["Valid_DC"]  = static_cast<int>(bs[567]);
  mAsic["ON_Discri"] = static_cast<int>(bs[566]);
  mAsic["ON_Fsb"]    = static_cast<int>(bs[565]);
  mAsic["ON_Otaq"]   = static_cast<int>(bs[564]);
  mAsic["ON_W"]      = static_cast<int>(bs[563]);
  mAsic["ON_Ss"]     = static_cast<int>(bs[562]);
  mAsic["ON_Buf"]    = static_cast<int>(bs[561]);
  mAsic["ON_Paf"]    = static_cast<int>(bs[560]);
  // Gain
  for(int i = 0; i < 64; i++)
  {
    int gain{0};
    for(int j = 0; j < 6; j++)
      if(bs[176 + i * 6 + j] != 0) gain += (1 << j);
    mAsic["Channel_" + std::to_string(i) + "_" + "Gain"]       = gain;
    mAsic["Channel_" + std::to_string(i) + "_" + "cTest"]      = bs[112 + i];
    mAsic["Channel_" + std::to_string(i) + "_" + "Valid_trig"] = static_cast<int>(bs[25 + i]);
  }

  mAsic["ON_Otabg"]  = static_cast<int>(bs[111]);
  mAsic["ON_Dac"]    = static_cast<int>(bs[110]);
  mAsic["ON_Otadac"] = static_cast<int>(bs[109]);
  // DAC
  int dac1{0};
  for(int j = 0; j < 10; j++)
    if(bs[j + 99] != 0) dac1 += (1 << j);
  mAsic["DAC1"] = dac1;
  int dac0{0};
  for(int j = 0; j < 10; j++)
    if(bs[j + 89] != 0) dac0 += (1 << j);
  mAsic["DAC0"]            = dac0;
  mAsic["EN_Raz_Ext"]      = static_cast<int>(bs[23]);
  mAsic["EN_Raz_Int"]      = static_cast<int>(bs[22]);
  mAsic["EN_Out_Raz_Int"]  = static_cast<int>(bs[21]);
  mAsic["EN_Trig_Ext"]     = static_cast<int>(bs[20]);
  mAsic["EN_Trig_Int"]     = static_cast<int>(bs[19]);
  mAsic["EN_Out_Trig_Int"] = static_cast<int>(bs[18]);
  mAsic["Bypass_Chip"]     = static_cast<int>(bs[17]);
  mAsic["HardrocHeader"]   = static_cast<int>(asicid);
  mAsic["EN_Out_Discri"]   = static_cast<int>(bs[8]);
  mAsic["EN_Transmit_On"]  = static_cast<int>(bs[7]);
  mAsic["EN_Dout"]         = static_cast<int>(bs[6]);
  mAsic["EN_RamFull"]      = static_cast<int>(bs[5]);
  m_MapSC[asicid]          = mAsic;
}

void DIFSlowControl::FillAsicHR2(const std::bitset<109 * 8>& bs)
{
  int asicid{0};
  for(int j = 0; j < 8; j++)
    if(bs[j + (108 - 7) * 8 + 2] != 0) asicid += (1 << (7 - j));
  std::map<std::string, int> mAsic;
  for(int i = 0; i < 64; i++)
  {
    int gain{0};
    int mask{0};
    mAsic["Channel_" + std::to_string(i) + "_" + "cTest"] = bs[i];
    for(int j = 0; j < 8; j++)
      if(bs[64 + i * 8 + j] != 0) gain += (1 << j);
    mAsic["Channel_" + std::to_string(i) + "_" + "Gain"] = gain;
    for(int j = 0; j < 3; j++)
      if(bs[8 * 77 + 2 + i * 3 + j] != 0) mask += (1 << j);
    mAsic["Channel_" + std::to_string(i) + "_" + "Mask"] = mask;
  }
  mAsic["PwrOnPA"] = static_cast<int>(bs[8 * 72]);
  mAsic["Cmdb3SS"] = static_cast<int>(bs[8 * 72 + 1]);
  mAsic["Cmdb2SS"] = static_cast<int>(bs[8 * 72 + 2]);
  mAsic["Cmdb1SS"] = static_cast<int>(bs[8 * 72 + 3]);
  mAsic["Cmdb0SS"] = static_cast<int>(bs[8 * 72 + 4]);
  mAsic["SwSsc0"]  = static_cast<int>(bs[8 * 72 + 5]);
  mAsic["SwSsc1"]  = static_cast<int>(bs[8 * 72 + 6]);
  mAsic["SwSsc2"]  = static_cast<int>(bs[8 * 72 + 7]);

  mAsic["PwrOnBuff"] = static_cast<int>(bs[8 * 73]);
  mAsic["PwrOnSS"]   = static_cast<int>(bs[8 * 73 + 1]);
  mAsic["PwrOnW"]    = static_cast<int>(bs[8 * 73 + 2]);
  mAsic["Cmdb3Fsb2"] = static_cast<int>(bs[8 * 73 + 3]);
  mAsic["Cmdb2Fsb2"] = static_cast<int>(bs[8 * 73 + 4]);
  mAsic["Cmdb1Fsb2"] = static_cast<int>(bs[8 * 73 + 5]);
  mAsic["Cmdb0Fsb2"] = static_cast<int>(bs[8 * 73 + 6]);
  mAsic["Sw50k2"]    = static_cast<int>(bs[8 * 73 + 7]);

  mAsic["Sw100k2"]   = static_cast<int>(bs[8 * 74]);
  mAsic["Sw100f2"]   = static_cast<int>(bs[8 * 74 + 1]);
  mAsic["Sw50f2"]    = static_cast<int>(bs[8 * 74 + 2]);
  mAsic["Cmdb3Fsb1"] = static_cast<int>(bs[8 * 74 + 3]);
  mAsic["Cmdb2Fsb1"] = static_cast<int>(bs[8 * 74 + 4]);
  mAsic["Cmdb1Fsb1"] = static_cast<int>(bs[8 * 74 + 5]);
  mAsic["Cmdb0Fsb1"] = static_cast<int>(bs[8 * 74 + 6]);
  mAsic["Sw50k1"]    = static_cast<int>(bs[8 * 74 + 7]);

  mAsic["Sw100k1"]   = static_cast<int>(bs[8 * 75]);
  mAsic["Sw100f1"]   = static_cast<int>(bs[8 * 75 + 1]);
  mAsic["Sw50f1"]    = static_cast<int>(bs[8 * 75 + 2]);
  mAsic["Sel0"]      = static_cast<int>(bs[8 * 75 + 3]);
  mAsic["Sel11"]     = static_cast<int>(bs[8 * 75 + 4]);
  mAsic["PwrOnFsb"]  = static_cast<int>(bs[8 * 75 + 5]);
  mAsic["PwrOnFsb1"] = static_cast<int>(bs[8 * 75 + 6]);
  mAsic["PwrOnFsb2"] = static_cast<int>(bs[8 * 75 + 7]);

  mAsic["Sw50k0"]      = static_cast<int>(bs[8 * 76]);
  mAsic["Sw100k0"]     = static_cast<int>(bs[8 * 76 + 1]);
  mAsic["Sw100f0"]     = static_cast<int>(bs[8 * 76 + 2]);
  mAsic["Sw50f0"]      = static_cast<int>(bs[8 * 76 + 3]);
  mAsic["EnOtaQ"]      = static_cast<int>(bs[8 * 76 + 4]);
  mAsic["OtaQ_PwrADC"] = static_cast<int>(bs[8 * 76 + 5]);
  mAsic["Discri_PwrA"] = static_cast<int>(bs[8 * 76 + 6]);
  mAsic["Discri2"]     = static_cast<int>(bs[8 * 76 + 7]);

  mAsic["Discri1"]      = static_cast<int>(bs[8 * 77]);
  mAsic["RS_or_Discri"] = static_cast<int>(bs[8 * 77 + 1]);

  mAsic["Header"] = asicid;
  for(int i = 0; i < 3; i++)
  {
    int B = 0;
    for(int j = 0; j < 10; j++)
      if(bs[8 * 102 + 2 + i * 10 + j] != 0) B += (1 << j);
    mAsic["B" + std::to_string(i)] = B;
  }

  mAsic["Smalldac"]  = static_cast<int>(bs[8 * 106]);
  mAsic["DacSw"]     = static_cast<int>(bs[8 * 106 + 1]);
  mAsic["OtagBgSw"]  = static_cast<int>(bs[8 * 106 + 2]);
  mAsic["Trig2b"]    = static_cast<int>(bs[8 * 106 + 3]);
  mAsic["Trig1b"]    = static_cast<int>(bs[8 * 106 + 4]);
  mAsic["Trig0b"]    = static_cast<int>(bs[8 * 106 + 5]);
  mAsic["EnTrigOut"] = static_cast<int>(bs[8 * 106 + 6]);
  mAsic["DiscrOrOr"] = static_cast<int>(bs[8 * 106 + 7]);

  mAsic["TrigExtVal"]   = static_cast<int>(bs[8 * 107]);
  mAsic["RazChnIntVal"] = static_cast<int>(bs[8 * 107 + 1]);
  mAsic["RazChnExtVal"] = static_cast<int>(bs[8 * 107 + 2]);
  mAsic["ScOn"]         = static_cast<int>(bs[8 * 107 + 3]);
  mAsic["CLKMux"]       = static_cast<int>(bs[8 * 107 + 4]);

  // EnOCDout1b 	EnOCDout2b 	EnOCTransmitOn1b 	EnOCTransmitOn2b 	EnOCChipsatb 	SelStartReadout 	SelEndReadout
  mAsic["SelEndReadout"]    = static_cast<int>(bs[8 * 108 + 1]);
  mAsic["SelStartReadout"]  = static_cast<int>(bs[8 * 108 + 2]);
  mAsic["EnOCChipsatb"]     = static_cast<int>(bs[8 * 108 + 3]);
  mAsic["EnOCTransmitOn2b"] = static_cast<int>(bs[8 * 108 + 4]);
  mAsic["EnOCTransmitOn1b"] = static_cast<int>(bs[8 * 108 + 5]);
  mAsic["EnOCDout2b"]       = static_cast<int>(bs[8 * 108 + 6]);
  mAsic["EnOCDout1b"]       = static_cast<int>(bs[8 * 108 + 7]);
  m_MapSC[asicid]           = mAsic;
}
