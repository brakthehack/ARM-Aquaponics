/* f3d_button.c --- 
 * 
 * Filename: f3d_button.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Jan 17 16:21:57 2013 (-0500)
 * Version: 
 * Last-Updated: 01/17/2014
 *           By: Ching Yuen Ng
 *     Update #: 1
 * URL: 
 * Doc URL: 
 * Keywords
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */
#include <stm32f30x_gpio.h>
#include <f3d_button.h>

void f3d_button_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure PD13 as an Inputs, extra button
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_2);
}

//return the button state, either 1 or 0
uint8_t f3d_button_read() {
  uint8_t button_state = 0x00;
  button_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
  return button_state;
}

uint8_t f3d_extra_button(){
  uint8_t button_state = 0x00;
  button_state = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13);
  button_state ^= 1;
  return button_state;
}
/* f3d_button.c ends here */
