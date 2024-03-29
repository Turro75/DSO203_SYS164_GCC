/********************* (C) COPYRIGHT 2011 e-Design Co.,Ltd. ********************
 File Name : FAT12.c  
 Version   : DS203_SYS Ver 1.5x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "FAT12.h"
#include "BIOS.h"
#include "ASM.h"
#include "Function.h"
#include "Ext_Flash.h"

typedef struct
{
  u32 FAT1_BASE;          // FAT1区开始地址
  u32 FAT2_BASE;          // FAT2区开始地址
  u32 ROOT_BASE;          // 根目录开始地址
  u32 FILE_BASE;          // 文件区开始地址 
  u32 FAT_LEN;
  u32 SEC_LEN;            // 扇区长度 
  u32 FAT_END;            // 链接结束
  u8  FAT1_SEC;           // FAT1扇区数
  u8  FAT2_SEC;
}FAT_InitTypeDef;
//-------FLASH-----W25Q64BV--------------------------------------------------//
#define FILE_BASE_8M    0x7000     // 文件区开始地址  
#define ROOT_BASE_8M    0x3000     // 根目录开始地址
#define FAT_LEN_8M      0x1000 
#define FAT1_BASE_8M    0x1000   // FAT1区开始地址
#define FAT2_BASE_8M    0x2000     // FAT2区开始地址
#define SEC_LEN_8M      0x1000     // 扇区长度 
#define FAT1_SEC_8M     0x1         // FAT1扇区数
#define FAT2_SEC_8M     0x1         // FAT2扇区数
#define FAT_END_8M      0x7FF        // 链接结束
//----------------------------------------------------------------------------//
#define FILE_BASE_2M    0x8000   // 文件区开始地址  
#define ROOT_BASE_2M    0x4000     // 根目录开始地址
#define FAT_LEN_2M      0x1800 
#define FAT1_BASE_2M    0x1000     // FAT1区开始地址
#define FAT2_BASE_2M    0x2800   // FAT2区开始地址
#define SEC_LEN_2M      0x200        // 扇区长度 
#define FAT1_SEC_2M     0x0C         // FAT1扇区数
#define FAT2_SEC_2M     0x0C         // FAT2扇区数
#define FAT_END_2M      0xFFF        // 链接结束
//----------------------------------------------------------------------------//
#define OK           0            // 操作完成
#define SEC_ERR      1            // 扇区读写错误
#define FAT_ERR      2            // FAT表读写错误
#define OVER         3            // 操作溢出
#define NEW          4            // 新目录项

#define OW           0            // 或写(数据从0变1改写)
#define RW           1            // 重写

//#define DISK          7
uc8 DiskDevInfo_8M[]={"8MB Internal"};

FAT_InitTypeDef FAT_V;
void Init_Fat_Value(void)
{
  u32 ptr;
  u8 Ver[8];

  ptr=Get(DFUVER, 0);
  memcpy(Ver,(u8*)ptr,5);
  ptr=(Ver[1]-'0')*100 +(Ver[3]-'0')*10 +(Ver[4]-'0'); 
  if(ptr<=311){
    flash_mode=FLASH_2M;
  }
  else{
    ptr=Get(DEVICEINFO, 0);
    if(memcmp((u8*)ptr,DiskDevInfo_8M,3)==0)flash_mode=FLASH_8M;
    else flash_mode=FLASH_2M;
  }
  if(flash_mode==FLASH_8M){
    FAT_V.FAT1_BASE=FAT1_BASE_8M; 
    FAT_V.FAT2_BASE=FAT2_BASE_8M; 
    FAT_V.ROOT_BASE=ROOT_BASE_8M;  
    FAT_V.FILE_BASE=FILE_BASE_8M;      
    FAT_V.FAT1_SEC=FAT1_SEC_8M;
    FAT_V.FAT2_SEC=FAT1_SEC_8M;
    FAT_V.SEC_LEN = SEC_LEN_8M;    
    FAT_V.FAT_LEN = FAT_LEN_8M; 
    FAT_V.FAT_END = FAT_END_8M;
  }
  else{
    FAT_V.FAT1_BASE=FAT1_BASE_2M; 
    FAT_V.FAT2_BASE=FAT2_BASE_2M; 
    FAT_V.ROOT_BASE=ROOT_BASE_2M;  
    FAT_V.FILE_BASE=FILE_BASE_2M;      
    FAT_V.FAT1_SEC=FAT1_SEC_2M;
    FAT_V.FAT2_SEC=FAT1_SEC_2M;
    FAT_V.SEC_LEN = SEC_LEN_2M;    
    FAT_V.FAT_LEN = FAT_LEN_2M; 
    FAT_V.FAT_END = FAT_END_2M;
  }
}
/*******************************************************************************
 读磁盘页面(256 Bytes)  包含USB读写冲突后重读
*******************************************************************************/
u8 ReadDiskData(u8* pBuffer, u32 ReadAddr, u16 Lenght)
{
  u8 n = 0;
  
  while(1){
    Clash = 0;
    __ExtFlash_PageRD(pBuffer, ReadAddr, Lenght);
    if(n++ > 6) return SEC_ERR;     // 超时出错返回
    if(Clash == 0) return OK;       // 无冲突产生则返回
  }
}
/*******************************************************************************
 写磁盘页面(256 Bytes)  包含USB读写冲突后重写
*******************************************************************************/
u8 ProgDiskPage(u8* pBuffer, u32 ProgAddr)
{                         
  u8   n = 0; 
  
  while(1){
    Clash = 0;
    __ExtFlash_PageWR(pBuffer, ProgAddr);
    if(n++ > 6) return SEC_ERR;     // 超时出错返回
    if(Clash == 0) return OK;       // 无冲突产生则返回
  }
} 
/*******************************************************************************
 查找下一个链接簇号后返回，当前簇号保存在指针+1的位置
*******************************************************************************/
u8 NextCluster(u16* pCluster)
{
  u16 FatNum;
  u32 Addr ;
  
  Addr=FAT_V.FAT1_BASE +(*pCluster + *pCluster/2);
  
  *(pCluster+1)= *pCluster;                                   // 保存前一个簇号
  *pCluster = 0;
  if((*(pCluster+1) >=FAT_V.FAT_END)||(*(pCluster+1)< 2)) return SEC_ERR;
  if(__ReadDiskData((u8*)&FatNum, Addr, 2)!= OK) return SEC_ERR;
  *pCluster= (*(pCluster+1) & 1)?(FatNum >>4):(FatNum & 0xFFF); // 指向下一个簇号
  return OK; 
}
/*******************************************************************************
 读文件扇区(512 Bytes), 返回时指针指向下一个簇号，当前簇号保存在指针+1的位置
*******************************************************************************/
u8 ReadFileSec(u8* pBuffer, u16* pCluster)
{
  u32 ReadAddr =FAT_V.FILE_BASE + FAT_V.SEC_LEN*(*pCluster-2);

  if(__ReadDiskData(pBuffer, ReadAddr, FAT_V.SEC_LEN)!=OK) return SEC_ERR;  //(SEC_LEN=4096/512)
  if(NextCluster(pCluster)!=0) return FAT_ERR;                 // 取下一个簇号
  return OK;
} 
/*******************************************************************************
 写文件扇区(512/4096 Bytes)，填写当前FAT表及返回查找到的下一个簇号
*******************************************************************************/
u8 ProgFileSec(u8* pBuffer, u16* pCluster)
{
  u16 Tmp;
  u32 ProgAddr = FAT_V.FILE_BASE + FAT_V.SEC_LEN*(*pCluster-2);
  if(__ProgDiskPage(pBuffer, ProgAddr)!= OK) return SEC_ERR; // 写扇区 4096 Byte 或 写前半扇区
  if(flash_mode==FLASH_2M){
    pBuffer += 256;  ProgAddr += 256;
    if(__ProgDiskPage(pBuffer, ProgAddr)!= OK) return SEC_ERR; // 写后半扇区
  }
  
  if(NextCluster(pCluster)!=0) return FAT_ERR;                 // 取下一个簇号
  Tmp = *(pCluster+1);
  if(*pCluster == 0){
    *pCluster = Tmp;
    if(SeekBlank (pBuffer, pCluster )!= OK) return OVER;
    if(SetCluster(pBuffer, pCluster )!= OK) return SEC_ERR;
  }
  return OK;
}
/*******************************************************************************
 查找空闲簇号，返回时指针指向下一个空闲簇号，当前簇号保存在指针+1的位置
*******************************************************************************/
u8 SeekBlank(u8* pBuffer, u16* pCluster)
{
  u16  Tmp;
  u8   Buffer[2];
  u8   Tmp_Flag = 1;

  *(pCluster+1)= *pCluster;                                    // 保存当前簇号
  /*
  for(*pCluster=0; (*pCluster)<4095; (*pCluster)++){
    if(__ReadDiskData(Buffer, FAT_V.FAT1_BASE +(*pCluster)+(*pCluster)/2, 2)!= 0) return SEC_ERR;
    Tmp = ((*pCluster)& 1)?((*(u16*)Buffer)>>4):((*(u16*)Buffer)& 0xFFF);
    if((Tmp == 0)&&((*pCluster)!= *(pCluster+1))) return OK;
  }
  */
  //-----------------------
  for(*pCluster=0; (*pCluster)<4095; (*pCluster)++){
    if(__ReadDiskData(Buffer, FAT_V.FAT1_BASE +(*pCluster)+(*pCluster)/2, 2)!= 0) return SEC_ERR;
    Tmp = ((*pCluster)& 1)?((*(u16*)Buffer)>>4):((*(u16*)Buffer)& 0xFFF);
    
    if((Tmp == 0)&&(Tmp_Flag == 0)&&(((*pCluster))!= *(pCluster+1))) {
      Tmp_Flag = 1;
      return OK;
    }
    if((Tmp == 0)&&(Tmp_Flag == 1))  {
      *(pCluster+2) = *pCluster;
      Tmp_Flag = 0;
    } 
  }
  //-----------------------
  return OK;
}         
/*******************************************************************************
 将下一簇号写入FAT表当前簇链接位，返回时指针指向下一簇号，指针+1为当前簇号
*******************************************************************************/
u8 SetCluster(u8* pBuffer, u16* pCluster)
{
  u16  Offset, i, k;
  u32  SecAddr;

  i = *(pCluster+1);                    // 提取原当前簇号
  k = *pCluster;                        // 提取下一簇号
  Offset = i+ i/2;
  SecAddr = FAT_V.FAT1_BASE +(Offset & 0xF000 );
  Offset &= 0x0FFF;
  if(__ReadDiskData(pBuffer, SecAddr, FAT_V.SEC_LEN)!= 0) return SEC_ERR; //???????????? old=258
  if(i & 1){
    pBuffer[Offset  ]=(pBuffer[Offset]& 0x0F)+((k <<4)& 0xF0);
    pBuffer[Offset+1]= k >>4;
  } else {
    pBuffer[Offset  ]= k & 0xFF;
    pBuffer[Offset+1]=(pBuffer[Offset+1]& 0xF0)+((k>>8)& 0x0F);
  }
  
  if(__ProgDiskPage(pBuffer, SecAddr)!= 0) return SEC_ERR;
  if(flash_mode==FLASH_2M)
    if(__ProgDiskPage(pBuffer+256, SecAddr+256)!= 0) return SEC_ERR;
  return OK;
}
/*******************************************************************************
 读模式打开文件：返回文件第一个簇号及目录项地址或 0簇号及第一个空白目录项地址
*******************************************************************************/
u8 OpenFileRd(u8* pBuffer, u8* pFileName, u16* pCluster, u32* pDirAddr)
{
  u16 i, n;

  *pCluster = 0;
  for(*pDirAddr=FAT_V.ROOT_BASE; *pDirAddr<FAT_V.FILE_BASE; ){
    if(__ReadDiskData(pBuffer, *pDirAddr,FAT_V.SEC_LEN)!= OK) return SEC_ERR;
    for(n=0; n<FAT_V.SEC_LEN; n+=32){   //?????????????????????256
      for(i=0; i<11; i++){
        if(pBuffer[n + i]!= 0){
          if(pBuffer[n + i]!= pFileName[i]) break;
          if(i == 10){                             // 找到文件名
            *pCluster = *(u16*)(pBuffer + n + 0x1A); // 文件第一个簇号
            return OK;         
          }
        } else return NEW;               // 遇到第一个空白目录项后返回
      }
      *pDirAddr += 32;
    }
  }
  return OVER;
}
/*******************************************************************************
 写模式打开文件：返回文件第一个簇号及目录项地址
*******************************************************************************/
u8 OpenFileWr(u8* pBuffer, u8* pFileName, u16* pCluster, u32* pDirAddr)
{
  u32 i, n,offset;
  
  i = OpenFileRd(pBuffer, pFileName, pCluster, pDirAddr);
  if(i != NEW) return i;
  else{                                                    // 当前项为空白目录项
    if(SeekBlank(pBuffer, pCluster)!= OK) return OVER;     // 若FAT表满返回
    if(flash_mode==FLASH_8M)
      n =*pDirAddr & 0xFFF;
    else
      n =*pDirAddr & 0xFF;                              // n为当前页目录号 OLD=0XFF
    offset=*pDirAddr-n;
//    if(__ReadDiskData(pBuffer,(*pDirAddr)-n, 4096)!= OK) return SEC_ERR;    //256
    if(__ReadDiskData(pBuffer,offset, FAT_V.SEC_LEN)!= OK) return SEC_ERR; 
    for(i=0; i<11; i++) pBuffer[n+i]= pFileName[i];      // 创建新目录项offset +
    *(u16*)(pBuffer + n + 0x1A)= *pCluster;

//    if(__ProgDiskPage(pBuffer,(*pDirAddr)-n)!= OK) return SEC_ERR;
    if(__ProgDiskPage(pBuffer,offset)!= OK) return SEC_ERR;
    return OK;
  }
}                
/*******************************************************************************
 关闭文件，将结束符写入FAT表，将文件长度写入目录项，复制FAT1到FAT2
*******************************************************************************/
u8 CloseFile(u8* pBuffer, u32 Lenght, u16* pCluster, u32* pDirAddr)
{
  u32 n;
  if(flash_mode==FLASH_8M)n=0xFFF;
  else  n=0xFF;
  *pCluster = 0xFFF;
  SetCluster(pBuffer, pCluster); 

  if(__ReadDiskData(pBuffer, (*pDirAddr &(~n)), FAT_V.SEC_LEN)!= OK) return SEC_ERR;
  *(u8* )(pBuffer +(*pDirAddr & n)+ 0x0B)= 0x20;
  *(u32*)(pBuffer +(*pDirAddr & n)+ 0x1C)= Lenght;
  if(__ProgDiskPage(pBuffer, (*pDirAddr &(~n)))!= OK) return SEC_ERR;
  if(flash_mode==FLASH_8M){
    if(__ReadDiskData(pBuffer, FAT_V.FAT1_BASE, FAT_V.SEC_LEN)!= OK) return SEC_ERR;
    if(__ProgDiskPage(pBuffer, FAT_V.FAT2_BASE     )!= OK) return SEC_ERR; 
  }
  else{
    for(n=0; n<FAT_V.FAT1_SEC; n++){
      if(__ReadDiskData(pBuffer, FAT_V.FAT1_BASE+n*256, 256)!= OK) return SEC_ERR;
      if(__ProgDiskPage(pBuffer, FAT_V.FAT2_BASE+n*256     )!= OK) return SEC_ERR;
    } 
  }
  return OK;
}
