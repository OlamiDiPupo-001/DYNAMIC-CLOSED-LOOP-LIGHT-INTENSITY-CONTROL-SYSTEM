/**
  ******************************************************************************
  * @file     : menu_config.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.3.0
  * @date     : 12-Jan-2021
  * @brief    : Simple linked-list menu example.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include <stdio.h>

#include "usart.h"
#include "tim.h"
#include "aio.h"
#include "lcd_config.h"
#include "led_config.h"
#include "bh1750_config.h"


/* Private function prototypes -----------------------------------------------*/
void menu_float_io_routine(MenuItem_TypeDef* hmenuitem, float value, unsigned int len, const char* name, const char* unit);

/* Public variables ----------------------------------------------------------*/
Menu_TypeDef hmenu = {
  .Display = &hlcd1, .Timer = &htim7, .SerialPort = &huart3,
};
unsigned int ADC1_ConvResults_mV[16];

/* Private variables ---------------------------------------------------------*/

MENU_ITEM_CONTRUCTOR(menu_ref_lux, { menu_float_io_routine(hmenuitem, hbh1750.Readout, 1, "Ref_Lux", "lux"); } );
MENU_ITEM_CONTRUCTOR(menu_illuminance, { menu_float_io_routine(hmenuitem, hbh1750.Readout, 2, "Illuminance", "lux"); } );
MENU_ITEM_CONTRUCTOR(menu_cntrl_sig, { menu_float_io_routine(hmenuitem, hbh1750.Readout, 3, "Control_Signal", "lux"); } );

//MENU_ITEM_CONTRUCTOR(menu_ref_lux, { menu_float_io_routine(hmenuitem, Ref_Lux, 1, "Ref_Lux", "lux"); } );
//MENU_ITEM_CONTRUCTOR(menu_illuminance, { menu_float_io_routine(hmenuitem, hbh1750.Readout, 2, "Illuminance", "lux"); } );
//MENU_ITEM_CONTRUCTOR(menu_cntrl_sig, { menu_float_io_routine(hmenuitem, Control_Signal, 3, "Control_Signal", "lux"); } );

#define MENU_MAIN_LEN (sizeof(MENU_MAIN_ARRAY)/sizeof(MENU_MAIN_ARRAY[0]))
MenuItem_TypeDef* MENU_MAIN_ARRAY[] = { /* Main menu list */
  &menu_ref_lux,
  &menu_illuminance,
  &menu_cntrl_sig
};

/* Private function ----------------------------------------------------------*/

void menu_float_io_routine(MenuItem_TypeDef* hmenuitem, float value, unsigned int len, const char* name, const char* unit)
{
  char temp_str[LCD_LINE_BUF_LEN];
  hmenuitem->DisplayStrLen = sprintf(temp_str, LCD_LINE_LEN, "%s: %*.2u%s", name, len, value, unit);
  MENU_ITEM_WriteDisplayBuffer(hmenuitem, temp_str); // Set display buffer
}


//void menu_float_io_routine(MenuItem_TypeDef* hmenuitem, float value, unsigned int len, const char* name, const char* unit)
//{
//  char temp_str[LCD_LINE_BUF_LEN];
//  hmenuitem->DisplayStrLen = sprintf(temp_str, LCD_LINE_LEN, "%s: %*.2u%s", name, len, value, unit);
//  MENU_ITEM_WriteDisplayBuffer(hmenuitem, temp_str); // Set display buffer
////  hmenuitem->SerialPortStrLen = sprintf(hmenuitem->SerialPortStr, "Reference_Lux = %s, ILLuminance = %5u.%03u, Control_Signal = %u\r",
////                                        temp_str, (unsigned int)value / 1000, (unsigned int)value % 1000, (unsigned int)value);
//}


/* Public function -----------------------------------------------------------*/
/**
 * @brief Menu initialization. Creates doubly-linked list from elements of MENU_MAIN_ARRAY.
 * @param[in/out] hmenu : Menu structure
 */
void MENU_Init(Menu_TypeDef* hmenu)
{
  /* Active element initialization */
  hmenu->Item = MENU_MAIN_ARRAY[0];

  /* Main menu initialization */
  for(uint8_t i = 0; i < (uint8_t)(MENU_MAIN_LEN-1); i++) //< Next item
    MENU_MAIN_ARRAY[i]->Next = MENU_MAIN_ARRAY[i+1];
  for(uint8_t i = 1; i < (uint8_t)MENU_MAIN_LEN; i++)     //< Previous item
    MENU_MAIN_ARRAY[i]->Prev = MENU_MAIN_ARRAY[i-1];
  /* Cyclic list */
  MENU_MAIN_ARRAY[MENU_MAIN_LEN-1]->Next = MENU_MAIN_ARRAY[0]; //< Next of last is first item
  MENU_MAIN_ARRAY[0]->Prev = MENU_MAIN_ARRAY[MENU_MAIN_LEN-1]; //< Previous of first is last item

  /* LCD set-up */
  LCD_DIO_SetCursor(hmenu->Display, 0, 0);
  LCD_DIO_printf(hmenu->Display, "%c", LCD_MENU_CURSOR_CHAR);
  LCD_DIO_SetCursor(hmenu->Display, 1, 0);
  LCD_DIO_printStr(hmenu->Display, " ");
}

/**
 * @brief Menu routine. Calls active items routines and updates output devices.
 * @param[in/out] hmenu : Menu structure
 */
void MENU_ROUTINE(Menu_TypeDef* hmenu)
{
  MENU_CALL_ROUTINE(hmenu->Item);
  MENU_CALL_ROUTINE(hmenu->Item->Next);

  hmenu->ItemChanged = 0;

  // #1 line - active item
  LCD_DIO_SetCursor(hmenu->Display, 0, 1);
  LCD_DIO_printStr(hmenu->Display, hmenu->Item->DisplayStr);

  // #1 line - next item
  LCD_DIO_SetCursor(hmenu->Display, 1, 1);
  LCD_DIO_printStr(hmenu->Display, hmenu->Item->Next->DisplayStr);
}

