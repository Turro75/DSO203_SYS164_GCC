/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : LCD.c  
 Version   : DS203_SYS Ver 1.3x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "stm32f10x_lib.h"
#include "Config.h"
#include "BIOS.h"
#include "LCD.h"
#include "ASM.h"

u32 LCD_Type;
u16 Fill_Buffer;
uc8 DFU_Vers[]={"V3.13C"};

//================================ �ֿⶨ�� ====================================
uc16 Char_TAB_8x14[744] =
{
   0x0000,0x0000,0x0000,0x001C,0x0020,0x0040,0x0040,0x0040, // " ���Ͻ�
   0x0040,0x0040,0x0020,0x001C,0x0000,0x0000,0x0000,0x0000, // # ���Ͻ�
   0x0000,0x0000,0x0000,0xE000,0x1000,0x0800,0x0800,0x0800, // $ ���½�
   0x0C30,0x0C60,0x00C0,0x0180,0x0300,0x0630,0x0C30,0x0000, // %    
   0x0800,0x0800,0x1000,0xE000,0x0000,0x0000,0x0000,0x0000, // & ���½�
   0x2010,0x2010,0x2010,0x2010,0x2010,0x2010,0x3FF0,0x0000, // ' ���β��
   0x0000,0x0000,0x0FC0,0x1FE0,0x3030,0x2010,0x0000,0x0000, // (    
   0x0000,0x0000,0x2010,0x3030,0x1FE0,0x0FC0,0x0000,0x0000, // )    
   0x0000,0x18C0,0x0D80,0x0700,0x0700,0x0D80,0x18C0,0x0000, // *  ��
   0x0000,0x0100,0x0100,0x07C0,0x07C0,0x0100,0x0100,0x0000, // +    
   0x0000,0x0000,0x0000,0xFFFC,0x0000,0x0000,0x0000,0x0000, // , ����
   0x0000,0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x0000, // -    
   0x0000,0x0000,0x0000,0x0030,0x0030,0x0000,0x0000,0x0000, // .    
   0x0030,0x0060,0x00C0,0x0180,0x0300,0x0600,0x0C00,0x0000, // /    
   0x1FE0,0x3FF0,0x2090,0x2310,0x2410,0x3FF0,0x1FE0,0x0000, // 0    
   0x0000,0x0810,0x1810,0x3FF0,0x3FF0,0x0010,0x0010,0x0000, // 1    
   0x1070,0x30F0,0x2190,0x2310,0x2610,0x3830,0x1830,0x0000, // 2    
   0x1020,0x3030,0x2210,0x2210,0x2210,0x3FF0,0x1DE0,0x0000, // 3    
   0x0300,0x0700,0x0D00,0x1910,0x3FF0,0x3FF0,0x0110,0x0000, // 4    
   0x3E20,0x3E30,0x2210,0x2210,0x2310,0x23F0,0x21E0,0x0000, // 5    
   0x0FE0,0x1FF0,0x3210,0x2210,0x2210,0x03F0,0x01E0,0x0000, // 6    
   0x3000,0x3000,0x20F0,0x21F0,0x2300,0x3E00,0x3C00,0x0000, // 7    
   0x1DE0,0x3FF0,0x2210,0x2210,0x2210,0x3FF0,0x1DE0,0x0000, // 8    
   0x1C00,0x3E10,0x2210,0x2210,0x2230,0x3FE0,0x1FC0,0x0000, // 9    
   0x0000,0x0000,0x0000,0x0C60,0x0C60,0x0000,0x0000,0x0000, // :    
   0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x0040, // ; �Ϻ���
   0x0000,0x0100,0x0380,0x06C0,0x0C60,0x1830,0x1010,0x0000, // <    
   0x0000,0x0090,0x0090,0x0090,0x0090,0x0090,0x0090,0x0000, // =    
   0x0240,0x0240,0x0240,0x0240,0x0240,0x0240,0x0240,0x0000, // >    
   0x0000,0x1010,0x1830,0x0C60,0x06C0,0x0380,0x0100,0x0000, // ?    
   0x0FC0,0x1020,0x2790,0x2490,0x27D0,0x1050,0x0F80,0x0000, // @    
   0x07F0,0x0FF0,0x1900,0x3100,0x1900,0x0FF0,0x07F0,0x0000, // A    
   0x2010,0x3FF0,0x3FF0,0x2210,0x2210,0x3FF0,0x1DE0,0x0000, // B    
   0x0FC0,0x1FE0,0x3030,0x2010,0x2010,0x3030,0x1860,0x0000, // C    
   0x2010,0x3FF0,0x3FF0,0x2010,0x3030,0x1FE0,0x0FC0,0x0000, // D    
   0x2010,0x3FF0,0x3FF0,0x2210,0x2710,0x3030,0x3870,0x0000, // E    
   0x2010,0x3FF0,0x3FF0,0x2210,0x2700,0x3000,0x3800,0x0000, // F    
   0x0FC0,0x1FE0,0x3030,0x2110,0x2110,0x31E0,0x19F0,0x0000, // G    
   0x3FF0,0x3FF0,0x0200,0x0200,0x0200,0x3FF0,0x3FF0,0x0000, // H    
   0x0000,0x0000,0x2010,0x3FF0,0x3FF0,0x2010,0x0000,0x0000, // I    
   0x00E0,0x00F0,0x0010,0x2010,0x3FF0,0x3FE0,0x2000,0x0000, // J    
   0x2010,0x3FF0,0x3FF0,0x0300,0x0F00,0x3CF0,0x3070,0x0000, // K    
   0x2010,0x3FF0,0x3FF0,0x2010,0x0010,0x0030,0x0030,0x0000, // L    
   0x3FF0,0x3FF0,0x1C00,0x0E00,0x1C00,0x3FF0,0x3FF0,0x0000, // M    
   0x3FF0,0x3FF0,0x1C00,0x0E00,0x0700,0x3FF0,0x3FF0,0x0000, // N    
   0x0FC0,0x1FE0,0x3030,0x2010,0x3030,0x1FE0,0x0FC0,0x0000, // O    
   0x2010,0x3FF0,0x3FF0,0x2210,0x2200,0x3E00,0x1C00,0x0000, // P    
   0x1FE0,0x3FF0,0x2010,0x2070,0x2030,0x3FF8,0x1FE8,0x0000, // Q    
   0x2010,0x3FF0,0x3FF0,0x2200,0x2300,0x3FF0,0x1CF0,0x0000, // R    
   0x1860,0x3C70,0x2610,0x2210,0x2310,0x39F0,0x18E0,0x0000, // S    
   0x0000,0x3800,0x3010,0x3FF0,0x3FF0,0x3010,0x3800,0x0000, // T    
   0x3FE0,0x3FF0,0x0010,0x0010,0x0010,0x3FF0,0x3FE0,0x0000, // U    
   0x3F80,0x3FC0,0x0060,0x0030,0x0060,0x3FC0,0x3F80,0x0000, // V    
   0x3FC0,0x3FF0,0x0070,0x01C0,0x0070,0x3FF0,0x3FC0,0x0000, // W    
   0x3030,0x3CF0,0x0FC0,0x0300,0x0FC0,0x3CF0,0x3030,0x0000, // X    
   0x0000,0x3C00,0x3E10,0x03F0,0x03F0,0x3E10,0x3C00,0x0000, // Y    
   0x3870,0x30F0,0x2190,0x2310,0x2610,0x3C30,0x3870,0x0000, // Z    
   0x0010,0x0030,0x0050,0x0190,0x0310,0x0C10,0x3FF0,0x0000, // [ ����
   0x0800,0x0800,0x0800,0x0800,0x0800,0x0800,0x0800,0x0800, // \ �º���
   0x0000,0x0000,0x0000,0xFC7C,0x0280,0x0280,0x0100,0x0100, // ] ָ��
   0x0200,0x0400,0x0C00,0x1FF0,0x3FF0,0x0C00,0x0400,0x0200, // ^ ������
   0x0100,0x0080,0x00C0,0x3FE0,0x3FF0,0x00C0,0x0080,0x0100, // _ �½���
   0x2010,0x2010,0x2010,0x2010,0x2010,0x2010,0x2010,0x2010, // ` �������
   0x00E0,0x05F0,0x0510,0x0510,0x07E0,0x03F0,0x0010,0x0000, // a    
   0x2010,0x3FF0,0x3FE0,0x0210,0x0210,0x03F0,0x01E0,0x0000, // b    
   0x03E0,0x07F0,0x0410,0x0410,0x0410,0x0630,0x0220,0x0000, // c    
   0x01E0,0x03F0,0x0610,0x2410,0x3FF0,0x3FF0,0x0010,0x0000, // d    
   0x03E0,0x07F0,0x0510,0x0510,0x0510,0x0730,0x0320,0x0000, // e    
   0x0000,0x0210,0x1FF0,0x3FF0,0x2210,0x3000,0x1800,0x0000, // f    
   0x03C8,0x07EC,0x0424,0x0424,0x03FC,0x07F8,0x0400,0x0000, // g    
   0x2010,0x3FF0,0x3FF0,0x0220,0x0400,0x07F0,0x03F0,0x0000, // h    
   0x0000,0x0000,0x0410,0x37F0,0x37F0,0x0010,0x0000,0x0000, // i    
   0x0000,0x0018,0x001C,0x0404,0x37FC,0x37F8,0x0400,0x0000, // j    
   0x2010,0x3FF0,0x3FF0,0x2180,0x03C0,0x0670,0x0430,0x0000, // k    
   0x0000,0x0000,0x2010,0x3FF0,0x3FF0,0x0010,0x0000,0x0000, // l    
   0x07F0,0x07F0,0x0600,0x03F0,0x0600,0x07F0,0x07F0,0x0000, // m    
   0x0400,0x07F0,0x03F0,0x0400,0x0400,0x07F0,0x03F0,0x0000, // n    
   0x03E0,0x07F0,0x0410,0x0410,0x0410,0x07F0,0x03E0,0x0000, // o    
   0x0404,0x07FC,0x03FC,0x0444,0x0440,0x07C0,0x0380,0x0000, // p    
   0x0380,0x07C0,0x0440,0x0444,0x07FC,0x03FC,0x0404,0x0000, // q    
   0x0410,0x07F0,0x03F0,0x0610,0x0400,0x0600,0x0300,0x0000, // r    
   0x0000,0x0220,0x0730,0x0590,0x0490,0x06F0,0x0260,0x0000, // s    
   0x0400,0x0400,0x1FE0,0x3FF0,0x0410,0x0430,0x0020,0x0000, // t    
   0x07E0,0x07F0,0x0010,0x0010,0x07E0,0x07F0,0x0010,0x0000, // u    
   0x0000,0x07C0,0x07E0,0x0030,0x0030,0x07E0,0x07C0,0x0000, // v    
   0x07E0,0x07F0,0x0030,0x00E0,0x0030,0x07F0,0x07E0,0x0000, // w    
   0x0410,0x0630,0x03E0,0x01C0,0x03E0,0x0630,0x0410,0x0000, // x    
   0x0780,0x07C4,0x0044,0x0044,0x0044,0x07F8,0x07F0,0x0000, // y    
   0x0630,0x0670,0x04D0,0x0590,0x0710,0x0630,0x0430,0x0000, // z    
   0x0780,0x0480,0x3CF0,0x2010,0x2FD0,0x2FD0,0x2FD0,0x2FD0, // { ���ͷ
   0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2FD0, // | �����
   0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2FD0,0x2010,0x3FF0,0x0000, // } ���β
   0x0780,0x0480,0x3CF0,0x2010,0x2010,0x2010,0x2010,0x2010 // ~ �յ��ͷ
};

/*******************************************************************************
 LCD_Initial: 400x240 LCD��ʾģ���ʼ���� 
*******************************************************************************/
void LCD_Initial(void)
{  
  u8 *Prt; 
  //------------------------- Reset LCD Driver ---------------------//
  LCD_RS_HIGH();
  LCD_nRST_HIGH(); 
  Delayms(1);           //RST=1��Delay 1ms      
  LCD_nRST_LOW(); 
  Delayms(10);          //RST=0 Reset LCD��Delay 1ms      
  LCD_nRST_HIGH(); 
  Delayms(50);          //RST=1��Delay 5 ms  
  LCD_RD_Type();
  if(LCD_Type == 0x02049327){
    //==========SYS V1.61================//
    Prt =__DeviceVersion(DFU);
    if(memcmp(Prt, "V3.13C", 6) >= 0){
      //----------- ILI9327_HSD3.0 Internal Register Initial -----------//         
      LCD_WR_Ctrl(0xE9); 
      LCD_PORT =  0x20; 
      LCD_WR_Ctrl(0x11);   //Exit Sleep 
      
      Delayms(100);           
      LCD_WR_Ctrl(0xD1);//VCOM Control  
      LCD_PORT =  0x00; 
      LCD_PORT =  0x70;//0x5E; //79//70
      LCD_PORT =  0x1C;//0x17; //1E//1C
      
      LCD_WR_Ctrl(0xD0);//Power Setting 
      LCD_PORT =  0x07; 
      LCD_PORT =  0x01;//0x07; 
      LCD_PORT =  0x01;//0x0C; 
      
      LCD_WR_Ctrl(0x36);//set address mode 
      LCD_PORT =  0x40; // 48
      
      LCD_WR_Ctrl(0x0C);//get pixel format 
      LCD_PORT =  0x55; 
      
      LCD_WR_Ctrl(0x3A);//set pixel format 
      LCD_PORT =  0x55; 
   /*============1.62=====================
      LCD_WR_Ctrl(0x2A);  //Set column address
      LCD_PORT =  0x00; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x0; 
      LCD_PORT =  0x0EF;
      
      LCD_WR_Ctrl(0x2B);  //Set page address
      LCD_PORT =  0x00; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x01; 
      LCD_PORT =  0x8F;
   //==================================== */     
      LCD_WR_Ctrl(0xC1); 
      LCD_PORT =  0x10; 
      LCD_PORT =  0x10; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x02; 

      LCD_WR_Ctrl(0xC0);   //Set Default Gamma 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x35; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x01; 
      LCD_PORT =  0x02; 
      
      LCD_WR_Ctrl(0xC5);   //Set frame rate  
      LCD_PORT =  0x01; 
      
      LCD_WR_Ctrl(0xD2);   //power setting
      LCD_PORT =  0x01; 
      LCD_PORT =  0x22; 
      
      LCD_WR_Ctrl(0xC8);   //Set Gamma 
      LCD_PORT =  0x01; 
      LCD_PORT =  0x73; 
      LCD_PORT =  0x37; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x0F; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x04; 
      LCD_PORT =  0x40; 
      LCD_PORT =  0x67; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x0F; 
      LCD_PORT =  0x08; 
      LCD_PORT =  0x80; 
      LCD_PORT =  0x00; 
      
      LCD_WR_Ctrl(0x29);   //display on 
      LCD_WR_Ctrl(0x2C); 
    }else{
      //----------- ILI9327_HSD3.0 Internal Register Initial -----------//         
      LCD_WR_Ctrl(0xE9); 
      LCD_PORT =  0x20; 
      LCD_WR_Ctrl(0x11);   //Exit Sleep  
      Delayms(100);           
      LCD_WR_Ctrl(0xD1); 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x5E; 
      LCD_PORT =  0x17; //1F 
      
      LCD_WR_Ctrl(0xD0); 
      LCD_PORT =  0x07; 
      LCD_PORT =  0x07; 
      LCD_PORT =  0x0C; //80 
      
      LCD_WR_Ctrl(0x36); 
      LCD_PORT =  0x40; // 48
      
      LCD_WR_Ctrl(0x0C); 
      LCD_PORT =  0x55; 
      
      LCD_WR_Ctrl(0x3A); 
      LCD_PORT =  0x55; 
      
      LCD_WR_Ctrl(0xC1); 
      LCD_PORT =  0x10; 
      LCD_PORT =  0x10; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x02; 
      
      LCD_WR_Ctrl(0xC0);   //Set Default Gamma 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x35; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x01; 
      LCD_PORT =  0x02; 
      
      LCD_WR_Ctrl(0xC5);   //Set frame rate  
      LCD_PORT =  0x01; 
      
      LCD_WR_Ctrl(0xD2);   //power setting
      LCD_PORT =  0x01; 
      LCD_PORT =  0x22; 
      
      LCD_WR_Ctrl(0xC8);   //Set Gamma 
      LCD_PORT =  0x01; 
      LCD_PORT =  0x73; 
      LCD_PORT =  0x37; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x0F; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x04; 
      LCD_PORT =  0x40; 
      LCD_PORT =  0x67; 
      LCD_PORT =  0x00; 
      LCD_PORT =  0x02; 
      LCD_PORT =  0x0F; 
      LCD_PORT =  0x08; 
      LCD_PORT =  0x80; 
      LCD_PORT =  0x00; 
      
      LCD_WR_Ctrl(0x29);   //display on 
      LCD_WR_Ctrl(0x2C); 
    }
  } else {
    //----------------R61509V_CPT30 Internal Register Initial------------//         
    LCD_WR_REG(0x0000, 0x0000); 
    LCD_WR_REG(0x0000, 0x0000); 
    LCD_WR_REG(0x0000, 0x0000); 
    LCD_WR_REG(0x0000, 0x0000); 
    Delayms(10); 
    LCD_WR_REG(0x0600, 0x0001);
    Delayms(10);
    LCD_WR_REG(0x06f0, 0x0040);
    Delayms(10);
    LCD_WR_REG(0x0400, 0x6200); 
    LCD_WR_REG(0x0008, 0x0808); 
    LCD_WR_REG(0x0001, 0x0100); 
    LCD_WR_REG(0x0002, 0x0100); 
    LCD_WR_REG(0x0003, 0x0030);   //LCD_WR_REG(0x0003, 0x1030); 
    LCD_WR_REG(0x0009, 0x0001); 
    LCD_WR_REG(0x000C, 0x0000);
    LCD_WR_REG(0x0090, 0x8000);
    LCD_WR_REG(0x000F, 0x0000);
    LCD_WR_REG(0x0010, 0x0016);
    LCD_WR_REG(0x0011, 0x0101);
    LCD_WR_REG(0x0012, 0x0000);
    LCD_WR_REG(0x0013, 0x0001);
    LCD_WR_REG(0x0100, 0x0330);
    LCD_WR_REG(0x0101, 0x0237);
    LCD_WR_REG(0x0103, 0x0F00); 
    LCD_WR_REG(0x0280, 0x6B00); 
    LCD_WR_REG(0x0102, 0xC1B0);
    Delayms(100); 
    LCD_WR_REG(0x0300, 0x0C00); 
    LCD_WR_REG(0x0301, 0x5A0B); 
    LCD_WR_REG(0x0302, 0x0906); 
    LCD_WR_REG(0x0303, 0x1017); 
    LCD_WR_REG(0x0304, 0x2300); 
    LCD_WR_REG(0x0305, 0x1700); 
    LCD_WR_REG(0x0306, 0x6309);
    LCD_WR_REG(0x0307, 0x0C09);
    LCD_WR_REG(0x0308, 0x100C);
    LCD_WR_REG(0x0309, 0x2232);
    
    LCD_WR_REG(0x0210, 0x0000);
    LCD_WR_REG(0x0211, 0x00EF);
    LCD_WR_REG(0x0212, 0x0000);
    LCD_WR_REG(0x0213, 0x018F);
    LCD_WR_REG(0x0500, 0x0000);
    LCD_WR_REG(0x0501, 0x0000);
    LCD_WR_REG(0x0502, 0x0005);
    LCD_WR_REG(0x0401, 0x0001);
    LCD_WR_REG(0x0404, 0x0000);
    Delayms(100);
    LCD_WR_REG(0x0007, 0x0100);
    Delayms(100);
    LCD_WR_REG(0x0200, 0x0000);
    LCD_WR_REG(0x0201, 0x0000); 
    
    LCD_RS_LOW();
    LCD_PORT = 0x0202;     //Reg. Addr.
    LCD_RS_HIGH();
  } 
}
/*******************************************************************************
 LCD_WR_REG: ����LCDģ����ָ���Ĵ�����ֵ�� ����: �Ĵ�����ַ���Ĵ���ֵ��
*******************************************************************************/
void LCD_WR_REG(u16 Reg, u16 Data) 
{
  LCD_RS_LOW();
  LCD_PORT = Reg;        //Reg. Addr.
  LCD_RS_HIGH();
  LCD_PORT  = Data;       //Reg. Data 
}

void LCD_EnterSleep_ILI9327(void) 
{ 
  LCD_WR_Ctrl(0x10); 
} 

void LCD_ExitSleep_ILI9327(void) 
{ 
  LCD_WR_Ctrl(0x11); 
}

void LCD_WR_Ctrl(u16 Reg) 
{
  LCD_RS_LOW();
  LCD_PORT = Reg;        //Reg. Addr.
  LCD_RS_HIGH();
}
void LCD_RD_Type(void) 
{ 
  LCD_WR_Ctrl(0xEF);
  LCD_Type  = (LCD_PORT&0xFF);
  LCD_Type  = (LCD_PORT&0xFF)<<24;
  LCD_Type |= (LCD_PORT&0xFF)<<16;
  LCD_Type |= (LCD_PORT&0xFF)<<8;
  LCD_Type |= (LCD_PORT&0xFF);
}
/*
u16 LCD_RD_Data(void) 
{
  u16 Data;
  
  Data  = LCD_PORT; 
  Data  = LCD_PORT; 
  return Data;      
}
*/
u16 Get_Pixl(void)
{ 
  u16 Data;
  
  if(LCD_Type == 0x02049327){
    LCD_WR_Ctrl(0x2E);
//    Data = LCD_GetPixl();
    Data  = LCD_PORT; 
    Data  = LCD_PORT; 
    LCD_WR_Ctrl(0x2C);
  } else {
    Data  = LCD_PORT; 
    Data  = LCD_PORT; 
//    return LCD_GetPixl();
  }
  return Data;
}

/*******************************************************************************
 delayms: ���루mS����ʱ���� ����: ��ʱ�ȴ��ĺ�����ֵ����72MHz��Ƶ����£�
*******************************************************************************/
void Delayms(u16 mS) 
{ 
  Delay_Cnt = mS;
  while (Delay_Cnt > 0){}
}
/*******************************************************************************
 Point_SCR : ָ����ʾλ�� for R61509V LCD   ����: X��Y����  
*******************************************************************************/
void Point_SCR(u16 x0, u16 y0)						
{ 
  __LCD_DMA_Ready();
  if(LCD_Type == 0x02049327){
    LCD_WR_Ctrl(0x2A);      // Block End X Address
    LCD_PORT = (0x00); 
    LCD_PORT = (y0 & 0xFF); 
    LCD_PORT = (0x00); 
    LCD_PORT = (0xEF); 
    LCD_WR_Ctrl(0x2B);      // Block End Y Address
    LCD_PORT = (x0 >> 8); 
    LCD_PORT = (x0 & 0xFF); 
    LCD_PORT = (399 >> 8); 
    LCD_PORT = (399 & 0xFF); 
    LCD_WR_Ctrl(0x2C); 
  } else {
    LCD_WR_REG(0x0200,y0);             
    LCD_WR_REG(0x0201,x0);             
    LCD_RS_LOW();
    LCD_PORT = 0x0202;     //Reg. Addr.
    LCD_RS_HIGH();
  }
}
/*******************************************************************************
 Clear_Screen: ��LCD��ʾ����Ļ�� 
*******************************************************************************/
void Clear_Screen(u16 Color)						
{ 
  Fill_Buffer = Color;
  Point_SCR(0, 0); 
  __LCD_Fill(&Fill_Buffer,240*200);//Clear Half Screen
  __LCD_DMA_Ready();
  __LCD_Fill(&Fill_Buffer,240*200);//Clear Half Screen
}
/*******************************************************************************
 Display_Str: ��ָ��λ����ʾ�ַ���   ����: X��Y���꣬��ɫֵ����ʾģʽ���ַ��� 
*******************************************************************************/
u16 Get_TAB_8x14(u8 Code, u16 Row)
{
  return Char_TAB_8x14[((Code-0x22)*8)+Row];
}

void Display_Str(u16 x0, u16 y0, u16 Color, u8 Mode, const unsigned char *s)
{ 
  signed short i, j, b; 
  Point_SCR(x0, y0);
  for (j=0; j<14;++j){ 
    if(Mode==0) __LCD_SetPixl(BLACK);   //Normal replace Display
    if(Mode==1) __LCD_SetPixl(Color);   //Inverse replace Display
  }
  x0++;                            // ÿһ���ַ���ǰ����һ�հ��� 
  while (*s!=0) {
    for(i=0;i<8;++i){
      Point_SCR(x0+i, y0);
      if((*s==0x20)||(*s==0x21)) b=0x0000;
      else                       b=Get_TAB_8x14(*s, i);
      if((*s==0x21)&&(i==4)) break;
      for(j=0;j<14;++j){
        if(b&0x04) {
          if(Mode==0) __LCD_SetPixl(Color);
          if(Mode==1) __LCD_SetPixl(BLACK);
        } else {
          if(Mode==0) __LCD_SetPixl(BLACK);
          if(Mode==1) __LCD_SetPixl(Color);
        }
        b>>=1;
      }
    }
    if(*s==0x21) x0 +=4;           //��ʾλ��ˮƽ����+4
    else  x0 += 8;                 //��ʾλ��ˮƽ����+8
    ++s;                           //�ַ���ָ��+1
  }
}
/*******************************************************************************
 Display_Logo: ��ָ��λ����ʾ�̱�ͼ��   ����: X��Y���� 
*******************************************************************************/
void Display_Logo(u16 x0, u16 y0)
{ 
  u8 *Ptr;
  u16 i, x, y, Color[16];
//  u32 b;
  
  Ptr = (u8*)INF_BASE;
  x = x0;
  y = y0;
  for(i=0; i<16; i++){        // 0x36~0x75: ��ɫ��(ÿɫ4B,˳��ΪB,G,R,Alpha)
    Color[i]=((Ptr[0x36 + i*4]>>3)<<11)+
      ((Ptr[0x37 + i*4]>>2)<<5)+(Ptr[0x38 + i*4]>>3);
  }      
  for(i=0; i<256*64/2; ++i){
    Point_SCR(x, y);
    LCD_PORT = Color[(Ptr[0x76 + i]&0xF0)>>4]; // �߰��ֽ�   
    x++;
    if(x > x0+255) { x = x0; y++;}
    Point_SCR(x, y);
    LCD_PORT = Color[Ptr[0x76 + i]&0x0F];      // �Ͱ��ֽ�
    x++;
    if(x > x0+255) { x = x0; y++;}
  }
}
/*******************************************************************************
 LCD_Set_Block: 
*******************************************************************************/
void LCD_Set_Block(u16 x1, u16 x2, u16 y1, u16 y2) 
{
  if(LCD_Type == 0x02049327){
    LCD_WR_Ctrl(0x2A);      // Block End X Address
    LCD_PORT = (y1 >> 8); 
    LCD_PORT = (y1 & 0xFF); 
    LCD_PORT = (y2 >> 8); 
    LCD_PORT = (y2 & 0xFF); 
    LCD_WR_Ctrl(0x2B);      // Block End Y Address
    LCD_PORT = (x1 >> 8); 
    LCD_PORT = (x1 & 0xFF); 
    LCD_PORT = (x2 >> 8); 
    LCD_PORT = (x2 & 0xFF); 
    LCD_WR_Ctrl(0x2C); 
  } else {
    LCD_WR_REG(0x0211, y2);      // Block End X Address
    LCD_WR_REG(0x0213, x2);      // Block End Y Address

    LCD_WR_REG(0x0210, y1);      // Block Start X Address
    LCD_WR_REG(0x0212, x1);      // Block Start Y Address
  
    LCD_WR_REG(0x0200, y1);
    LCD_WR_REG(0x0201, x1);
  
    LCD_RS_LOW();
    LCD_PORT = 0x0202;             //GRAM Reg. Addr.
    LCD_RS_HIGH();
  }
}
/******************************** END OF FILE *********************************/
