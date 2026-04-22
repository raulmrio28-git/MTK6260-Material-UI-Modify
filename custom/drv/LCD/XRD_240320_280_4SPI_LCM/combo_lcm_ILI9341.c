/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   lcd.c
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *   This Module defines the LCD driver.
 *
 * Author:
 	Hesong Li
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "drv_comm.h"
#include "reg_base.h"
#include "lcd_sw_inc.h"
#include "lcd_sw.h"
#include "lcd_hw.h"
/*Serial interface*/
#include "lcd_if_hw.h"
#include "lcd_if.h"
#include "eint.h"

#ifdef LQT_SUPPORT/*Do not remove LQT code segment*/
#include "lcd_lqt.h"
extern kal_uint8 lcd_at_mode;
extern kal_bool lcd_update_permission;
#endif/*LQT_SUPPORT*/
//#endif

extern kal_bool  lcd_assert_fail;

extern kal_uint32 drv_get_current_time(void);
extern kal_uint32 drv_get_duration_ms(kal_uint32 previous_time);
extern void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada);

volatile kal_uint32 ranesis_delay;


void LCDDelayms(kal_uint16 data)
{

    kal_uint32 time1;
    
    time1 = drv_get_current_time();
    while (drv_get_duration_ms(time1) <= data);
}

void init_lcd_interface_ILI9341(void)
{
	
	//Serial Clock:
	SET_LCD_SERIAL_CLOCK(0, LCD_SERIAL_CLOCK_132MHZ);
	//SET_LCD_SERIAL_CLOCK(0, LCD_SERIAL_CLOCK_26MHZ);

	/*
	#if defined(SERIAL_LCM_2_DATA_LINE_PROTOCOL)
	ENABLE_LCD_SERIAL0_2PIN;
	#if defined(MAIN_LCD_16BIT_MODE)
		SET_LCD_SERIAL0_IF_2PIN_SIZE(LCD_SCNF_IF_2PIN_WIDTH_16);
	#elif defined(MAIN_LCD_18BIT_MODE)
		SET_LCD_SERIAL0_IF_2PIN_SIZE(LCD_SCNF_IF_2PIN_WIDTH_18);
	#endif
	#endif
	*/

	#if defined(SLCD_FEATURE_SINGLE_A0_MODE)
	ENABLE_LCD_SERIAL0_SINGLE_A0;
	#else
	DISABLE_LCD_SERIAL0_SINGLE_A0;
	#endif

	#if defined(SLCD_FEATURE_CS_STAY_LOW_MODE)
	ENABLE_LCD_SERIAL0_CS_STAY_LOW;
	#else
	DISABLE_LCD_SERIAL0_CS_STAY_LOW;
	#endif


	//Define serial LCM work mode here
	#if (defined(SERIAL_LCM_4WIRE_1_DATA_LANE))
	DISABLE_LCD_SERIAL0_3WIRE;
	#elif (defined(SERIAL_LCM_3WIRE_1_DATA_LANE)||defined(SERIAL_LCM_3WIRE_2_DATA_LANE))
	ENABLE_LCD_SERIAL0_3WIRE;
	DISABLE_LCD_SERIAL0_SDI;
	#endif

	ENABLE_LCD_SERIAL_IF_HW_CS;//HW CS

	#if defined(MAIN_LCD_16BIT_MODE)
	SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_16);
	#elif defined(MAIN_LCD_18BIT_MODE)
	SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_18);
	#elif defined(MAIN_LCD_8BIT_MODE)
	SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_8);
	#endif

	//Timing need tuning
	SET_LCD_SERIAL_IF_CSS(0, 6);  //
	SET_LCD_SERIAL_IF_CSH(0, 6);

	SET_LCD_SERIAL_IF_WR_1ST(0,6);//
	SET_LCD_SERIAL_IF_WR_2ND(0,6);
	SET_LCD_SERIAL_IF_RD_1ST(0,8);//
	SET_LCD_SERIAL_IF_RD_2ND(0,8);
	SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_8);//CMD use 8 bit width

	#if defined(MAIN_LCD_16BIT_MODE_RGB888)
	SET_LCD_SEND_RES_MODE(1);
	#endif

	//Set Driving current
	set_lcd_driving_current(LCD_DRIVING_8MA);

}
	


#ifdef LQT_SUPPORT/*Do not remove LQT code segment*/
/*************************************************************************
* FUNCTION
*   LCD_gamma_test() and LCD_flicker_test()
*
* DESCRIPTION
*   Generating test pattern by AT commands.
*
* PARAMETERS
*   level, color
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void LCD_gamma_test_ILI9341(kal_uint8 level, kal_uint8 color)
{
	kal_uint16 LCD_x;
	kal_uint16 LCD_y;
	kal_uint16 r_color,g_color,b_color,w_color;
    kal_uint32 lcd_layer0_buffer;	

  		  		
	lcd_layer0_buffer=REG_LCD_LAYER0_BUFF_ADDR;
	r_color=(level>>1)<<11;	/* transfer to RGB565 */
	g_color=level<<5;
	b_color=level>>1;
	w_color=(r_color|g_color|b_color);
	for(LCD_y=0;LCD_y<LCD_HEIGHT;LCD_y++)
	{
		for(LCD_x=0;LCD_x<LCD_WIDTH/2;LCD_x++)
		{
			switch(color)
			{
	      case 0:
					*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((w_color<<16)|w_color);
					break;					
				case 1:
					*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((r_color<<16)|r_color);
					break;
	      case 2:
					*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((g_color<<16)|g_color);
					break;
	      case 3:
					*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((b_color<<16)|b_color);
					break;

	      default:
	         ASSERT(0);
			}
		}
	}		
}

void LCD_flicker_test_ILI9341(kal_uint8 level)
{
		kal_uint16 LCD_x;
		kal_uint16 LCD_y;
		kal_uint16 r_color,g_color,b_color,w_color;
    kal_uint32 lcd_layer0_buffer;	
    	
	  lcd_layer0_buffer=REG_LCD_LAYER0_BUFF_ADDR;
	  r_color=(level>>1)<<11;	/* transfer to RGB565 */
	  g_color=level<<5;
	  b_color=level>>1;
	  w_color=(r_color|g_color|b_color);
		
		for(LCD_y=0;LCD_y<LCD_HEIGHT;LCD_y++)
		{
			if(LCD_y&0x1)
			{
				for(LCD_x=0;LCD_x<LCD_WIDTH/2;LCD_x++)
						*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((w_color<<16)|w_color);
			}
			else
			{
				for(LCD_x=0;LCD_x<LCD_WIDTH/2;LCD_x++)
					*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)=0x0;
			}
		}	
}
#endif/*LQT_SUPPORT*/


/***********************ILI9341********************************/
/*ILI, 262K Color LCD, Parallel Interface, Firefly development board*/

void LCD_Partial_On_ILI9341(kal_uint16 start_page,kal_uint16 end_page)
{

}

void LCD_Partial_Off_ILI9341(void)
{

}

kal_uint8 LCD_Partial_line_ILI9341(void)
{
	return 1;		/* partial display in 1 line alignment */
}

void LCD_Set_Y_Addr_ILI9341(kal_uint16 start_row, kal_uint16 end_row)
{

}

void LCD_Set_X_Addr_ILI9341(kal_uint16 start_column, kal_uint16 end_column)
{

}

void LCD_blockClear_ILI9341(kal_uint16 x1,kal_uint16 y1,kal_uint16 x2,kal_uint16 y2,kal_uint16 data)
{
	kal_uint16 LCD_x;
	kal_uint16 LCD_y;
	kal_uint8 r_color,g_color,b_color;
	kal_uint8 x1_high,x1_low,y1_high,y1_low,x2_high,x2_low,y2_high,y2_low;


	x1_high=(x1&0xFF00)>>8;
	x1_low=x1&0xFF;
	y1_high=(y1&0xFF00)>>8;
	y1_low=y1&0xFF;
	x2_high=(x2&0xFF00)>>8;
	x2_low=x2&0xFF;
	y2_high=((y2)&0xFF00)>>8;
	y2_low=(y2)&0xFF;

#ifdef MAIN_LCD_9BIT_MODE
	r_color=(data&0xF800)>>10;	/* transfer to RGB666 */
	g_color=(data>>5)&0x3F;
	b_color=((data)&0x1F)<<1;
#endif

    LCD_CtrlWrite_ILI9341(0x2A);
	LCD_DataWrite_ILI9341(((x1&0xFF00)>>8));
	LCD_DataWrite_ILI9341((x1&0xFF));
	LCD_DataWrite_ILI9341(((x2&0xFF00)>>8));
	LCD_DataWrite_ILI9341((x2&0xFF));
    LCD_CtrlWrite_ILI9341(0x2B);
	LCD_DataWrite_ILI9341(((y1&0xFF00)>>8));
	LCD_DataWrite_ILI9341((y1&0xFF));
	LCD_DataWrite_ILI9341(((y2&0xFF00)>>8));
	LCD_DataWrite_ILI9341((y2&0xFF));
	
	LCD_CtrlWrite_ILI9341(0x2C);

	//data = 0xF800;
	for(LCD_y=y1;LCD_y<=y2;LCD_y++)
	{
		for(LCD_x=x1;LCD_x<=x2;LCD_x++)
		{
		#if (defined(MAIN_LCD_8BIT_MODE))
			*((volatile unsigned char *) MAIN_LCD_DATA_ADDR)=(kal_uint8)((data&0xFF00)>>8);
			*((volatile unsigned char *) MAIN_LCD_DATA_ADDR)=(kal_uint8)(data&0xFF);			
		#elif (defined(MAIN_LCD_9BIT_MODE))
			*((volatile unsigned short *) MAIN_LCD_DATA_ADDR)=((r_color<<6)|(g_color>>3)<<9);
			*((volatile unsigned short *) MAIN_LCD_DATA_ADDR)=((((g_color&0x07)<<6)|b_color)<<9);		
		#elif (defined(MAIN_LCD_16BIT_MODE) || defined(MAIN_LCD_18BIT_MODE))
			*((volatile unsigned int *) MAIN_LCD_DATA_ADDR)=(r_color<<12)|(g_color<<6)|b_color;
		#endif
		}
	}

}

void LCD_ClearAll_ILI9341(kal_uint16 data)
{
   LCD_blockClear_ILI9341(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,data);
}


void LCD_Init_ILI9341(kal_uint32 bkground, void **buf_addr)
{
	GPIO_ModeSetup(46, 2);      //LSRSTB
	SET_LCD_CTRL_RESET_PIN;
	LCDDelayms(10);
	CLEAR_LCD_CTRL_RESET_PIN;
	LCDDelayms(10); 
	SET_LCD_CTRL_RESET_PIN;
	LCDDelayms(120);

	LCD_CtrlWrite_ILI9341(0x01); //software reset
	LCDDelayms(5);

	LCD_CtrlWrite_ILI9341(0x11);
	LCDDelayms(120);

 	LCD_CtrlWrite_ILI9341(0xCF);   
	LCD_DataWrite_ILI9341(0x00); 
	LCD_DataWrite_ILI9341(0x83); 
	LCD_DataWrite_ILI9341(0X30); 	 
      
 	LCD_CtrlWrite_ILI9341(0xED);   
	LCD_DataWrite_ILI9341(0x64); 
	LCD_DataWrite_ILI9341(0x03); 
	LCD_DataWrite_ILI9341(0X12); 
	LCD_DataWrite_ILI9341(0X81); 
      
 	LCD_CtrlWrite_ILI9341(0xE8);   
	LCD_DataWrite_ILI9341(0x85); 
	LCD_DataWrite_ILI9341(0x01); 
	LCD_DataWrite_ILI9341(0x79); 
      
 	LCD_CtrlWrite_ILI9341(0xCB);   
	LCD_DataWrite_ILI9341(0x39); 
	LCD_DataWrite_ILI9341(0x2C); 
	LCD_DataWrite_ILI9341(0x00); 
	LCD_DataWrite_ILI9341(0x34); 
	LCD_DataWrite_ILI9341(0x02); 
      
 	LCD_CtrlWrite_ILI9341(0xF7);   
	LCD_DataWrite_ILI9341(0x20); 
      
 	LCD_CtrlWrite_ILI9341(0xEA);   
	LCD_DataWrite_ILI9341(0x00); 
	LCD_DataWrite_ILI9341(0x00); 
      
      
 	LCD_CtrlWrite_ILI9341(0xC1);    //Power control 
	LCD_DataWrite_ILI9341(0x11);   //SAP[2:0];BT[3:0] 
      
 	LCD_CtrlWrite_ILI9341(0xC5);    //VCM control 1 
	LCD_DataWrite_ILI9341(0x34); 
	LCD_DataWrite_ILI9341(0x3D); 
      
 	LCD_CtrlWrite_ILI9341(0xC7);    //VCM control 2 
	LCD_DataWrite_ILI9341(0xC0); 
    
 	LCD_CtrlWrite_ILI9341(0x36);    // Memory Access Control 
	LCD_DataWrite_ILI9341(0x08); 
      
 	LCD_CtrlWrite_ILI9341(0x3A);   	 // Pixel format 
	LCD_DataWrite_ILI9341(0x55);  //16bit 
    
 	LCD_CtrlWrite_ILI9341(0xB1);   	   // Frame rate
	LCD_DataWrite_ILI9341(0x00);   
	LCD_DataWrite_ILI9341(0x1D);  //65Hz
      
 	LCD_CtrlWrite_ILI9341(0xB6);    // Display Function Control 
	LCD_DataWrite_ILI9341(0x0A); 
	LCD_DataWrite_ILI9341(0xA2); 
	LCD_DataWrite_ILI9341(0x27);
	LCD_DataWrite_ILI9341(0x00);

	LCD_CtrlWrite_ILI9341(0xb7); //Entry mode 
	LCD_DataWrite_ILI9341(0x07);
    
      
 	LCD_CtrlWrite_ILI9341(0xF2);    // 3Gamma Function Disable 
	LCD_DataWrite_ILI9341(0x08); 
      
 	LCD_CtrlWrite_ILI9341(0x26);    //Gamma curve selected 
	LCD_DataWrite_ILI9341(0x01); 
      

	LCD_CtrlWrite_ILI9341(0xE0); //positive gamma correction
	LCD_DataWrite_ILI9341(0x1f);
	LCD_DataWrite_ILI9341(0x1a);
	LCD_DataWrite_ILI9341(0x18);
	LCD_DataWrite_ILI9341(0x0a);
	LCD_DataWrite_ILI9341(0x0f);
	LCD_DataWrite_ILI9341(0x06);
	LCD_DataWrite_ILI9341(0x45);
	LCD_DataWrite_ILI9341(0x87);
	LCD_DataWrite_ILI9341(0x32);
	LCD_DataWrite_ILI9341(0x0a);
	LCD_DataWrite_ILI9341(0x07);
	LCD_DataWrite_ILI9341(0x02);
	LCD_DataWrite_ILI9341(0x07);
	LCD_DataWrite_ILI9341(0x05);
	LCD_DataWrite_ILI9341(0x00);
	
	LCD_CtrlWrite_ILI9341(0xE1); //negamma correction
	LCD_DataWrite_ILI9341(0x00);
	LCD_DataWrite_ILI9341(0x25);
	LCD_DataWrite_ILI9341(0x27);
	LCD_DataWrite_ILI9341(0x05);
	LCD_DataWrite_ILI9341(0x10);
	LCD_DataWrite_ILI9341(0x09);
	LCD_DataWrite_ILI9341(0x3a);
	LCD_DataWrite_ILI9341(0x78);
	LCD_DataWrite_ILI9341(0x4d);
	LCD_DataWrite_ILI9341(0x05);
	LCD_DataWrite_ILI9341(0x18);
	LCD_DataWrite_ILI9341(0x0d);
	LCD_DataWrite_ILI9341(0x38);
	LCD_DataWrite_ILI9341(0x3a);
	LCD_DataWrite_ILI9341(0x1f);
      
 	LCD_CtrlWrite_ILI9341(0x11);    //Exit Sleep 
	LCDDelayms(120);
 	LCD_CtrlWrite_ILI9341(0x29);    //Display on 
	LCDDelayms(50);

}



void LCD_EnterSleep_ILI9341(void)
{
#if defined(LQT_SUPPORT)/*Do not remove LQT code segment*/
 if(!(lcd_at_mode==LCD_AT_RELEASE_MODE))
 {
  return;
 }	
#endif /*defined(LQT_SUPPORT))*/	

	LCD_CtrlWrite_ILI9341(0x28);
	LCDDelayms(10);  
	LCD_CtrlWrite_ILI9341(0x10);
	LCDDelayms(120); 
}

void LCD_ExitSleep_ILI9341(void)
{
#if defined(LQT_SUPPORT)/*Do not remove LQT code segment*/
 if(!(lcd_at_mode==LCD_AT_RELEASE_MODE))
 {
  return;
 }	
#endif /*defined(LQT_SUPPORT))*/

	LCD_CtrlWrite_ILI9341(0x11);
	LCDDelayms(120); 
	LCD_CtrlWrite_ILI9341(0x29);
	LCDDelayms(150);

	//LCD_Init_HX8352B(0,NULL);
}


void LCD_PWRON_ILI9341(kal_bool on)
{
   if(on)
   {
      LCD_ExitSleep_ILI9341();
   }
   else
   {
      LCD_EnterSleep_ILI9341();
   }
}

void LCD_SetContrast_ILI9341(kal_uint8 level)
{
}

void LCD_ON_ILI9341(kal_bool on)
{
   if (on)
   {
      LCD_ExitSleep_ILI9341();
   }
   else
   {
      LCD_EnterSleep_ILI9341();
   }
}

void LCD_BlockWrite_ILI9341(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy)
{

#ifdef LQT_SUPPORT/*Do not remove LQT code segment*/
    if(!lcd_update_permission&&!(lcd_at_mode==LCD_AT_RELEASE_MODE))
    {
    	lcd_assert_fail = KAL_FALSE;
    	return;//in LQT mode but not update permitted
    }
    if(lcd_update_permission&&!(lcd_at_mode==LCD_AT_RELEASE_MODE))
    {
    	startx = 0;
    	starty = 0;
    	endx = LCD_WIDTH -1;
    	endy = LCD_HEIGHT -1; //in LQT mode and update permitted
    }
#endif /*LQT_SUPPORT*/

#if defined(__RF_DESENSE_TEST__)&& defined(__FM_DESENSE_COPY_NO_LCM_UPDATE_)
   // under this test, LCD will do nothing ==> just return this function call
   return;
#endif

	ASSERT(lcd_assert_fail==KAL_FALSE);
	lcd_assert_fail = KAL_TRUE;

	LCD_CtrlWrite_ILI9341(0x2A);
	LCD_DataWrite_ILI9341(((startx&0xFF00)>>8));
	LCD_DataWrite_ILI9341((startx&0xFF));
	LCD_DataWrite_ILI9341(((endx&0xFF00)>>8));
	LCD_DataWrite_ILI9341((endx&0xFF));
    LCD_CtrlWrite_ILI9341(0x2B);
	LCD_DataWrite_ILI9341(((starty&0xFF00)>>8));
	LCD_DataWrite_ILI9341((starty&0xFF));
	LCD_DataWrite_ILI9341(((endy&0xFF00)>>8));
	LCD_DataWrite_ILI9341((endy&0xFF));
	
	LCD_CtrlWrite_ILI9341(0x2C);

	ENABLE_LCD_TRANSFER_COMPLETE_INT;//Kate
	DISABLE_LCD_ROI_CTRL_CMD_FIRST;
	START_LCD_TRANSFER;

	lcd_assert_fail = KAL_FALSE;
}


void LCD_Size_ILI9341(kal_uint16 *out_LCD_width,kal_uint16 *out_LCD_height)
{
   *out_LCD_width = LCD_WIDTH;
   *out_LCD_height = LCD_HEIGHT;
}

/*Engineering mode*/
kal_uint8 LCD_GetParm_ILI9341(lcd_func_type type)
{
   switch(type)
   {
      case lcd_Bais_func:
         return 1;
      case lcd_Contrast_func:
         return 1;
      case lcd_LineRate_func:
         return 1;
      case lcd_Temperature_Compensation_func:
         return 1;
      default:
         ASSERT(0);
      return 100;
   }
}

void LCD_SetBias_ILI9341(kal_uint8 *bias)
{
}

void LCD_Contrast_ILI9341(kal_uint8 *contrast)
{
}

void LCD_LineRate_ILI9341(kal_uint8 *linerate)
{
}

void LCD_Temp_Compensate_ILI9341(kal_uint8 *compensate)
{
}

void LCD_Set_Scan_Direction_ILI9341(kal_uint8 rotate_value)
{
}

LCM_IOCTRL_STATUS_ENUM
LCD_IOCTRL_ILI9341(LCM_IOCTRL_ID_ENUM ID, void* Parameters)
{
  
  
   switch (ID)
   {
       case LCM_IOCTRL_QUERY__FRAME_RATE:
       {
             *(kal_uint32*) (Parameters) =  90;
             
            return LCM_IOCTRL_OK;
             
       }

       case LCM_IOCTRL_SET__FRAME_RATE:
       {
	   		 return LCM_IOCTRL_NOT_SUPPORT;
            
       }


       case LCM_IOCTRL_QUERY__FRAME_MARKER:
	   	 	return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_SET__FRAME_MARKER:
            return LCM_IOCTRL_NOT_SUPPORT;


       case LCM_IOCTRL_QUERY__SUPPORT_H_V_SIGNAL_FUNC:
       {
	   		 return LCM_IOCTRL_NOT_SUPPORT;
       }

       case LCM_IOCTRL_QUERY__SUPPORT_V_PULSE_WIDTH:
       {
			 return LCM_IOCTRL_NOT_SUPPORT;
			
       }

       case LCM_IOCTRL_QUERY__SUPPORT_H_PULSE_WIDTH:
                return LCM_IOCTRL_NOT_SUPPORT;


       case LCM_IOCTRL_QUERY__BACK_PORCH:
       {
            *(kal_uint32*) (Parameters) = 2; // for example
            return LCM_IOCTRL_OK;
       }

       case LCM_IOCTRL_QUERY__FRONT_PORCH:
       {
            *(kal_uint32*) (Parameters) = 2; // for example
            return LCM_IOCTRL_OK;
       }

       case LCM_IOCTRL_SET__BACK_PORCH:
       {
	   	 return LCM_IOCTRL_NOT_SUPPORT;
       }

       case LCM_IOCTRL_SET__FRONT_PORCH:
       {
	   	 return LCM_IOCTRL_NOT_SUPPORT;
       }

       case LCM_IOCTRL_QUERY__TE_EDGE_ATTRIB:
       {
	   		return LCM_IOCTRL_NOT_SUPPORT;
       }

       case LCM_IOCTRL_QUERY__SUPPORT_READBACK_FUNC:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__SCANLINE_REG:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__IF_CS_NUMBER:
       {
            *(kal_uint32*) (Parameters) =  LCD_IF_SERIAL_0;
            return LCM_IOCTRL_OK;
       }

       case LCM_IOCTRL_QUERY__LCM_WIDTH:
       {
            *(kal_uint32*) (Parameters) =  240;
            return LCM_IOCTRL_OK;
       }

       case LCM_IOCTRL_QUERY__LCM_HEIGHT:
       {
            *(kal_uint32*) (Parameters) =  320;
            return LCM_IOCTRL_OK;
       }
       
     case LCM_IOCTRL_QUERY__SYNC_MODE:

          *(kal_uint32*) (Parameters) = LCM_TE_VSYNC_MODE;
          return LCM_IOCTRL_OK;

     case LCM_IOCTRL_QUERY__FLIP_MIRROR:
          return LCM_IOCTRL_NOT_SUPPORT;

     case LCM_IOCTRL_QUERY__ROTATION:
          return LCM_IOCTRL_OK;
          
     case LCM_IOCTRL_QUERY__LCD_PPI: 
     	  *((kal_uint32*)Parameters) = LCM_DPI;
      	  return LCM_IOCTRL_OK;  
     default:
          return LCM_IOCTRL_NOT_SUPPORT;
          
   }
}


#ifdef COMBO_LCM_ID_AUTO_DETECT
extern void rmmi_write_to_uart(kal_uint8 * buffer, kal_uint16 length, kal_bool stuff); 

static kal_uint32 ili9341_lcm_id;

kal_uint32 LCM_Read_Cmd_ILI9341(int cmd, unsigned int n)
{
	char buffer[128];
	ili9341_lcm_id = 0;
	LCD_CtrlWrite_ILI9341(cmd);
	while (n--)
	{
		ili9341_lcm_id <<= 8;
		ili9341_lcm_id |= LCD_DataRead_ILI9341;
	}
	kal_prompt_trace(MOD_ENG,"ILI9341 read_cmd------>%d",ili9341_lcm_id);
	sprintf(buffer, "ILI9341 read_cmd------>%d\n",ili9341_lcm_id);
    rmmi_write_to_uart((kal_uint8*)buffer, strlen(buffer), KAL_TRUE);
}
kal_bool LCM_ID_check_ILI9341()
{
	GPIO_ModeSetup(46, 2);      //LSRSTB
	SET_LCD_CTRL_RESET_PIN;
	LCDDelayms(10);
	CLEAR_LCD_CTRL_RESET_PIN;
	LCDDelayms(10); 
	SET_LCD_CTRL_RESET_PIN;
	LCDDelayms(120);
    LCM_Read_Cmd_ILI9341(0xD3, 4);
	ili9341_lcm_id &= 0xffff; //dummy read
	//0x9341
    if (ili9341_lcm_id==0x9341)
      return KAL_TRUE;
    else
      return KAL_FALSE;
}
#endif


LCD_Funcs LCD_func_ILI9341 = { //qys mark
   LCD_Init_ILI9341,
   LCD_PWRON_ILI9341,
   LCD_SetContrast_ILI9341,
   LCD_ON_ILI9341,
   LCD_BlockWrite_ILI9341,
   LCD_Size_ILI9341,
   LCD_EnterSleep_ILI9341,
   LCD_ExitSleep_ILI9341,
   LCD_Partial_On_ILI9341,
   LCD_Partial_Off_ILI9341,
   LCD_Partial_line_ILI9341,
   /*Engineering mode*/
   LCD_GetParm_ILI9341,
   LCD_SetBias_ILI9341,
   LCD_Contrast_ILI9341,
   LCD_LineRate_ILI9341,
   LCD_Temp_Compensate_ILI9341
#ifdef LCM_ROTATE_SUPPORT
   ,LCD_Set_Scan_Direction_ILI9341
#endif
#ifdef LQT_SUPPORT/*Do not remove LQT code segment*/
   ,LCD_gamma_test_ILI9341
   ,LCD_flicker_test_ILI9341
#endif
  , NULL
   ,LCD_IOCTRL_ILI9341
};

void LCD_FunConfig_ILI9341(void)
{

	MainLCD = &LCD_func_ILI9341;

	#ifdef DUAL_LCD
		//SubLCD = &LCD_func_HD66791;
	#endif
}