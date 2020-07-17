#include "DIFSlowControl.h"
#include <iostream>
#include <stdio.h>
void DIFSlowControl::FillHR1(int header_shift,unsigned char *cbuf)
{
  //  int scsize1=cbuf[header_shift-1]*72+(header_shift-1)+2;
  int nasic = cbuf[header_shift-1]; int idx=header_shift;
  for (int k=0;k<nasic;k++)
    {
      bitset<72*8> bs;
      //printf("%x %x \n",cbuf[idx+k*72+69],cbuf[idx+k*72+70]);
      for (int l =71;l>=0;l--)
	{
	  //  printf("%d %x : %d -->",l,cbuf[idx+k*72+l],(71-l)*8);
	  for (int m=0;m<8;m++)
	    {
	      if ( ( (1<<m) & cbuf[idx+k*72+l]) !=0 )
		bs.set((71-l)*8+m,1);
	      else
		bs.set((71-l)*8+m,0);
	      //printf("%d",(int) bs[(71-l)*8+m]);
	    }
	  //printf("\n");
	}

      FillAsicHR1(bs);
    }
}


void DIFSlowControl::FillHR2(int header_shift,unsigned char *cbuf)
{
  // int scsize1=cbuf[header_shift-1]*109+(header_shift-1)+2;
  int nasic = cbuf[header_shift-1]; int idx=header_shift;
  //std::cout<<" DIFSlowControl::FillHR nasic "<<nasic<<std::endl;
  for (int k=0;k<nasic;k++)
    {
      bitset<109*8> bs;
      //printf("%x %x \n",cbuf[idx+k*109+69],cbuf[idx+k*109+70]);
      for (int l =108;l>=0;l--)
	{
	  //  printf("%d %x : %d -->",l,cbuf[idx+k*109+l],(71-l)*8);
	  for (int m=0;m<8;m++)
	    {
	      if ( ( (1<<m) & cbuf[idx+k*109+l]) !=0 )
		bs.set((108-l)*8+m,1);
	      else
		bs.set((108-l)*8+m,0);
	      //printf("%d",(int) bs[(71-l)*8+m]);
	    }
	  //printf("\n");
	}

      FillAsicHR2(bs);
    }
}


DIFSlowControl::DIFSlowControl(unsigned int vers,unsigned short DIdi,unsigned char *cbuf)
{
  _DIFId=DIdi;
  _version=vers;
  _asicType=2;
  //
  //std::cout<<"DIFSlowControl :: "<<std::hex<<(int)cbuf[0]<<std::dec<<std::endl;
  if (cbuf[0]!=0xb1) return;
  int header_shift=6;
  if (_version<8)
    _nAsic=cbuf[5];
  else
    {
      _DIFId=cbuf[1];
      _nAsic= cbuf[2];
      header_shift=3;
    }

  //std::cout<<" Id version header shift "<<_DIFId<<" "<<_version<<" "<<_nAsic<<" "<<header_shift<<std::endl;
  int size_hardroc1 = _nAsic*72+header_shift+1;
  if (cbuf[size_hardroc1-1]!=0xa1) size_hardroc1=0;

  int size_hardroc2 = _nAsic*109+header_shift+1;
  if (cbuf[size_hardroc2-1]!=0xa1) size_hardroc2=0;

  //std::cout<<" Hardroc 1 "<<size_hardroc1<<" "<<(int)cbuf[size_hardroc1-1]<<" "<<(int) cbuf[size_hardroc1]<<std::endl;
  //std::cout<<" Hardroc 2 "<<size_hardroc2<<" "<<(int)cbuf[size_hardroc2-1]<<" "<<(int) cbuf[size_hardroc2]<<std::endl;

  if (size_hardroc1!=0)
    {
    FillHR1(header_shift,cbuf);
    _asicType=1;
    }
  else
    if (size_hardroc2!=0)
      FillHR2(header_shift,cbuf);
    else
      return;





}
void DIFSlowControl::FillAsicHR1(bitset<72*8> &bs)
{
  // Asic Id
  int asicid=0;
  for (int j=0;j<8;j++) if (bs[j+9]!=0) asicid += (1<<(7-j));
  char Name[256];

  map<string,int> mAsic;

  // Slow Control
  mAsic["SSC0"]=(int)  bs[575];
  mAsic["SSC1"]=(int) bs[574];
  mAsic["SSC2"]=(int) bs[573];
  mAsic["Choix_caisson"]=(int) bs[572];
  mAsic["SW_50k"]=(int) 	bs[571];
  mAsic["SW_100k"]=(int) bs[570];
  mAsic["SW_100f"]=(int) bs[569];
  mAsic["SW_50f"]=(int) 	bs[568];

  mAsic["Valid_DC"]=(int)  bs[567];
  mAsic["ON_Discri"]=(int) bs[566];
  mAsic["ON_Fsb"]=(int) 	bs[565];
  mAsic["ON_Otaq"]=(int) bs[564];
  mAsic["ON_W"]=(int) bs[563];
  mAsic["ON_Ss"]=(int) bs[562];
  mAsic["ON_Buf"]=(int) bs[561];
  mAsic["ON_Paf"]=(int) bs[560];
  // Gain
  int Gain[64];
  for(int i=0;i<64;i++)
    {
      Gain[i]=0;
      for (int j=0;j<6;j++) if (bs[176+i*6+j]!=0) Gain[i]+= (1<<j);
      sprintf(Name,"Channel_%i_",i);
      string name(Name);
      mAsic[(name+"Gain")]=Gain[i];
    }

  // cTest
  int cTest[64];
  for(int i=0;i<64;i++)
    {
      cTest[i]=bs[112+i];
      sprintf(Name,"Channel_%i_",i);
      string name(Name);
      mAsic[(name+"cTest")]=cTest[i];

    }

  mAsic["ON_Otabg"]=(int)  bs[111];
  mAsic["ON_Dac"]=(int) 	bs[110];
  mAsic["ON_Otadac"]=(int)  bs[109];
  // DAC
  int dac1=0;
  for (int j=0;j<10;j++) if (bs[j+99]!=0) dac1+= (1<<j);
  mAsic["DAC1"]=dac1;
  int dac0=0;
  for (int j=0;j<10;j++) if (bs[j+89]!=0) dac0+= (1<<j);
  mAsic["DAC0"]=dac0;
  // Valid
  int Valid_trig[64];
  for (int j=0;j<64;j++)
    {
      Valid_trig[j]=(int) bs[25+j];
      sprintf(Name,"Channel_%i_",j);
      string name(Name);
      mAsic[(name+"Valid_trig")]=Valid_trig[j];

    }



  mAsic["EN_Raz_Ext"]=(int)  bs[23];
  mAsic["EN_Raz_Int"]=(int) bs[22];
  mAsic["EN_Out_Raz_Int"]=(int) bs[21];
  mAsic["EN_Trig_Ext"]=(int) bs[20];
  mAsic["EN_Trig_Int"]=(int) bs[19];
  mAsic["EN_Out_Trig_Int"]=(int)  bs[18];
  mAsic["Bypass_Chip"]=(int) bs[17];

  mAsic["HardrocHeader"]=(int) asicid;

  mAsic["EN_Out_Discri"]=(int)  bs[8];

  mAsic["EN_Transmit_On"]=(int)  bs[7];
  mAsic["EN_Dout"]=(int)  bs[6];
  mAsic["EN_RamFull"]=(int)  bs[5];

  _mapSC[asicid] =mAsic;
  return;

}
void DIFSlowControl::FillAsicHR2(bitset<109*8> &bs)
{
  int asicid=0;
  for (int j=0;j<8;j++) if (bs[j+(108-7)*8+2]!=0) asicid += (1<<(7-j));

  //std::cout<<"DIFSlowControl::FillAsicHR2 "<<asicid<<std::endl;
  char Name[256];
  map<string,int> mAsic;

  int cTest[64],gain[64];
  for(int i=0;i<64;i++)
    {
      cTest[i]=bs[i];
      sprintf(Name,"Channel_%i_",i);
      string name(Name);
      mAsic[(name+"cTest")]=cTest[i];

      gain[i]=0;
      for (int j=0;j<8;j++) if (bs[64+i*8+j]!=0) gain[i] +=(1<<j);
      mAsic[(name+"Gain")]=gain[i];

    }
  mAsic["PwrOnPA"] = (int) bs[8*72];
  mAsic["Cmdb3SS"] = (int) bs[8*72+1];
  mAsic["Cmdb2SS"] = (int) bs[8*72+2];
  mAsic["Cmdb1SS"] = (int) bs[8*72+3];
  mAsic["Cmdb0SS"] = (int) bs[8*72+4];
  mAsic["SwSsc0"] = (int) bs[8*72+5];
  mAsic["SwSsc1"] = (int) bs[8*72+6];
  mAsic["SwSsc2"] = (int) bs[8*72+7];

  mAsic["PwrOnBuff"] = (int) bs[8*73];
  mAsic["PwrOnSS"] = (int) bs[8*73+1];
  mAsic["PwrOnW"] = (int) bs[8*73+2];
  mAsic["Cmdb3Fsb2"] = (int) bs[8*73+3];
  mAsic["Cmdb2Fsb2"] = (int) bs[8*73+4];
  mAsic["Cmdb1Fsb2"] = (int) bs[8*73+5];
  mAsic["Cmdb0Fsb2"] = (int) bs[8*73+6];
  mAsic["Sw50k2"] = (int) bs[8*73+7];

  mAsic["Sw100k2"] = (int) bs[8*74];
  mAsic["Sw100f2"] = (int) bs[8*74+1];
  mAsic["Sw50f2"] = (int) bs[8*74+2];
  mAsic["Cmdb3Fsb1"] = (int) bs[8*74+3];
  mAsic["Cmdb2Fsb1"] = (int) bs[8*74+4];
  mAsic["Cmdb1Fsb1"] = (int) bs[8*74+5];
  mAsic["Cmdb0Fsb1"] = (int) bs[8*74+6];
  mAsic["Sw50k1"] = (int) bs[8*74+7];

  mAsic["Sw100k1"] = (int) bs[8*75];
  mAsic["Sw100f1"] = (int) bs[8*75+1];
  mAsic["Sw50f1"] = (int) bs[8*75+2];
  mAsic["Sel0"] = (int) bs[8*75+3];
  mAsic["Sel11"] = (int) bs[8*75+4];
  mAsic["PwrOnFsb"] = (int) bs[8*75+5];
  mAsic["PwrOnFsb1"] = (int) bs[8*75+6];
  mAsic["PwrOnFsb2"] = (int) bs[8*75+7];


  mAsic["Sw50k0"] = (int) bs[8*76];
  mAsic["Sw100k0"] = (int) bs[8*76+1];
  mAsic["Sw100f0"] = (int) bs[8*76+2];
  mAsic["Sw50f0"] = (int) bs[8*76+3];
  mAsic["EnOtaQ"] = (int) bs[8*76+4];
  mAsic["OtaQ_PwrADC"] = (int) bs[8*76+5];
  mAsic["Discri_PwrA"] = (int) bs[8*76+6];
  mAsic["Discri2"] = (int) bs[8*76+7];

  mAsic["Discri1"] = (int) bs[8*77];
  mAsic["RS_or_Discri"] = (int) bs[8*77+1];

  int mask[64];
  for (int i=0;i<64;i++)
    {
      mask[i]=0;
      for (int j=0;j<3;j++) if (bs[8*77+2+i*3+j]!=0) mask[i]+=(1<<j);
      sprintf(Name,"Channel_%i_",i);
      string name(Name);
      mAsic[(name+"Mask")]=mask[i];
    }

  mAsic["Header"]=asicid;
  int B[3];
  for (int i=0;i<3;i++)
    {
      B[i]=0;
      for (int j=0;j<10;j++) if (bs[8*102+2+i*10+j]!=0) B[i]+=(1<<j);
      sprintf(Name,"B%i",i);
      string name(Name);
      mAsic[name]=B[i];
    }

  mAsic["Smalldac"] = (int) bs[8*106];
  mAsic["DacSw"] = (int) bs[8*106+1];
  mAsic["OtagBgSw"] = (int) bs[8*106+2];
  mAsic["Trig2b"] = (int) bs[8*106+3];
  mAsic["Trig1b"] = (int) bs[8*106+4];
  mAsic["Trig0b"] = (int) bs[8*106+5];
  mAsic["EnTrigOut"] = (int) bs[8*106+6];
  mAsic["DiscrOrOr"] = (int) bs[8*106+7];

  mAsic["TrigExtVal"] = (int) bs[8*107];
  mAsic["RazChnIntVal"] = (int) bs[8*107+1];
  mAsic["RazChnExtVal"] = (int) bs[8*107+2];
  mAsic["ScOn"] = (int) bs[8*107+3];
  mAsic["CLKMux"] = (int) bs[8*107+4];

  // EnOCDout1b 	EnOCDout2b 	EnOCTransmitOn1b 	EnOCTransmitOn2b 	EnOCChipsatb 	SelStartReadout 	SelEndReadout
  mAsic["SelEndReadout"] = (int) bs[8*108+1];
  mAsic["SelStartReadout"] = (int) bs[8*108+2];
  mAsic["EnOCChipsatb"] = (int) bs[8*108+3];
  mAsic["EnOCTransmitOn2b"] = (int) bs[8*108+4];
  mAsic["EnOCTransmitOn1b"] = (int) bs[8*108+5];
  mAsic["EnOCDout2b"] = (int) bs[8*108+6];
  mAsic["EnOCDout1b"] = (int) bs[8*108+7];

 _mapSC[asicid] =mAsic;


}
void DIFSlowControl::Dump()
{

  for (map< int,map < string,int > >::iterator it=_mapSC.begin();it!=_mapSC.end();it++)
    {
      std::cout<<"ASIC " <<it->first<<std::endl;
      map < string,int >::iterator jt =it->second.begin();
      for (map < string,int >::iterator jt =(it->second).begin(); jt!=(it->second).end();jt++)
	std::cout<<jt->first<<" : "<<jt->second<<std::endl;
    }
}
