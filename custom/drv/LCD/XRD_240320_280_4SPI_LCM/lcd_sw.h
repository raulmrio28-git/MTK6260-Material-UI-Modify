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
 *   lcd_sw.h
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
 * -------
 *   
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
 * removed!
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
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "lcd_if_hw.h"
#include "lcm_if.h"
#include "reg_base.h"

#ifndef _LCD_SW_H
#define _LCD_SW_H
//MT6250 serial LCM driver

#define SERIAL_LCM_4WIRE_1_DATA_LANE

#define MAIN_LCD_8BIT_MODE
#define MAIN_LCD_8BIT_MODE_RGB565

#define COMBO_LCM_ID_AUTO_DETECT
 
#define __SERIAL_LCM__
#define __SERIAL_LCM_CS0__

#ifdef DUAL_LCD
#define SUB_LCD_8BIT_MODE  
#endif

#define MAIN_LCD_CMD_ADDR               LCD_SERIAL0_A0_LOW_ADDR //LOW_ADDR is CMD ADDR
#define MAIN_LCD_DATA_ADDR              LCD_SERIAL0_A0_HIGH_ADDR//HIGH_ADDR is DATA ADDR

#ifdef DUAL_LCD
#define SUB_LCD_CMD_ADDR               LCD_SERIAL1_A0_LOW_ADDR //LOW_ADDR is CMD ADDR
#define SUB_LCD_DATA_ADDR              LCD_SERIAL1_A0_HIGH_ADDR//HIGH_ADDR is DATA ADDR
#endif

#if (defined(MAIN_LCD_8BIT_MODE_RGB444))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_8BIT_12_BPP_RGB444_1
#elif (defined(MAIN_LCD_8BIT_MODE_RGB565))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_8BIT_16_BPP_RGB565_1
#elif (defined(MAIN_LCD_8BIT_MODE_RGB666))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_8BIT_18_BPP_RGB666_1
#elif (defined(MAIN_LCD_16BIT_MODE_RGB444))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_16BIT_12_BPP_RGB444_3
#elif (defined(MAIN_LCD_16BIT_MODE_RGB565))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_16BIT_16_BPP_RGB565_1
#elif (defined(MAIN_LCD_16BIT_MODE_RGB666))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_16BIT_18_BPP_RGB666_3
#elif (defined(MAIN_LCD_16BIT_MODE_RGB888))
#define MAIN_LCD_OUTPUT_FORMAT	LCM_16BIT_24_BPP_RGB888_1
#elif (defined(MAIN_LCD_18BIT_MODE_RGB666))
#define MAIN_LCD_OUTPUT_FORMAT LCM_18BIT_18_BPP_RGB666_1
#else
#error please check LCD TestCase define_error!!! 
#endif
/***********************ILI9341********************************/

#define LCD_CtrlWrite_ILI9341(_data) \
{\
SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_8);\
*((volatile kal_uint16 *)MAIN_LCD_CMD_ADDR) = (_data&0x00FF);\
for(ranesis_delay = 20; ranesis_delay > 0; ranesis_delay--);\
}

#define LCD_DataWrite_ILI9341(_data) \
{\
SET_LCD_SERIAL0_IF_SIZE(LCD_SCNF_IF_WIDTH_8);\
*((volatile kal_uint16 *)MAIN_LCD_DATA_ADDR) = (_data&0x00FF);\
for(ranesis_delay = 20; ranesis_delay > 0; ranesis_delay--);\
}

//For LCD read data
#if (defined(MAIN_LCD_8BIT_MODE))
#define LCD_DataRead_ILI9341    (*(volatile kal_uint8 *)MAIN_LCD_DATA_ADDR);
#elif (defined(MAIN_LCD_16BIT_MODE))
#define LCD_DataRead_ILI9341    (*(volatile kal_uint16 *)MAIN_LCD_DATA_ADDR);
#elif (defined(MAIN_LCD_18BIT_MODE))
#define LCD_DataRead_ILI9341    (*(volatile kal_uint32 *)MAIN_LCD_DATA_ADDR);
#endif

// define main supported combo lcm ID
typedef enum
{
    ILI9341  = 0,
    MAIN_LCM_MAX_NUM  //please add main lcd ID above this item, this "MAIN_LCM_MAX_NUM" will be used as the number of LCMs.
}  main_lcm_enum;

#ifdef DUAL_LCD
// define sub supported combo lcm ID
typedef enum
{
    SUB_EXAMPLE_LCM_A  = 0,
    SUB_EXAMPLE_LCM_B  = 1,
    SUB_EXAMPLE_LCM_C  = 2,
    SUB_LCM_MAX_NUM  //please add sub lcd ID above this item, this "SUB_LCM_MAX_NUM" will be used as the number of LCMs.
}  sub_lcm_enum;    //notice, the enum value should not be conflict with main lcd enum.
#endif

#ifdef COMBO_LCM_ID_AUTO_DETECT
kal_uint32 LCM_ID_Get(void);
#define DEFAULT_MAIN_LCD_OUTPUT_FORMAT MAIN_LCD_OUTPUT_FORMAT
#endif

#endif  /*_LCD_SW_H*/