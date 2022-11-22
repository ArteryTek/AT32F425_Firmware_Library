/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f425_board.h"
#include "at32f425_clock.h"

/** @addtogroup AT32F425_periph_examples
  * @{
  */

/** @addtogroup 425_TMR_cascade_synchro TMR_cascade_synchro
  * @{
  */

gpio_init_type gpio_init_struct = {0};
tmr_output_config_type tmr_output_struct;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr1/tmr3/tmr15/gpioa/gpiob clock */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR15_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* timer1/timer3/timer15 output pin configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_6;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_2);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_0);

  /* tmr1/3/15 configuration */
  tmr_base_init(TMR1, 119, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  tmr_base_init(TMR3, 11, 0);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

  tmr_base_init(TMR15, 11, 0);
  tmr_cnt_dir_set(TMR15, TMR_COUNT_UP);

  /* channelx Configuration in output mode */
  tmr_output_default_para_init(&tmr_output_struct);
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_output_struct.oc_idle_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_idle_state = FALSE;

  /* timer1 channel 1 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 29);

  /* timer1 select enable signal to sub timer */
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR1, TRUE);

  /* timer1 channel 1 */
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, 3);

  /* sub mode selection: tmr3  */
  tmr_sub_mode_select(TMR3, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR3, TMR_SUB_INPUT_SEL_IS0);

  /* timer3 select enable signal to sub timer */
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR3, TRUE);

  /* timer15 channel 1 */
  tmr_output_channel_config(TMR15, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR15, TMR_SELECT_CHANNEL_1, 3);

  /* sub mode selection: tmr15 */
  tmr_sub_mode_select(TMR15, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR15, TMR_SUB_INPUT_SEL_IS1);

  /* tmr1/tmr15 output enable */
  tmr_output_enable(TMR1, TRUE);
  tmr_output_enable(TMR15, TRUE);

  /* enable tmr1/tmr3/tmr15 */
  tmr_counter_enable(TMR1, TRUE);
  tmr_counter_enable(TMR3, TRUE);
  tmr_counter_enable(TMR15, TRUE);

  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
