/* f3d_buzzer.h --- 
 * 
 * Filename: f3d_buzzer.h
 * Description: Header for a simple buzzer on Pin D12
 * Author: Brian Rak, Ching Ng
 * Maintainer: 
 * Created: Feb 21 2014
 * Last-Updated: 02/21/2014
 *           By: brak, ching
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Code: */
#include <stm32f30x.h>

void f3d_buzzer_init(void);
void f3d_buzzer_on(void); 
void f3d_buzzer_off(void); 
static void f3d_timer4_pin(void);
void f3d_timer4_init(void);
void f3d_frequency(uint32_t freq);

/* buzzer.h ends here */
