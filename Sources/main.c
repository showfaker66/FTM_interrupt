#include "Cpu.h"
#include <string.h>

volatile int exit_code = 0;
#define Msg "Interrupt is triggered\r\n"
int flag;

  void ftmTimerISR(void)
  {
	  flag = 1;
	  //PINS_DRV_TogglePins(PTD,1 << 0);
	  PINS_DRV_TogglePins(PTD,1 << 15);
	  //PINS_DRV_TogglePins(PTD,1 << 16);
	  FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1,(uint32_t)FTM_TIME_OVER_FLOW_FLAG);
  }
  void delay(volatile int cycles)
    {
    	while(cycles--);
    }

int main(void)
{
	ftm_state_t ftmStateStruct;//定时器结构体
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
     					 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig,&ftmStateStruct);
    INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn,&ftmTimerISR,(isr_t*) 0U);//(中断类型:定时器溢出中断,定时器中断函数,中断函数的参数);
    INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);

    FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig);//定时器实例，计数器配置
    FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    //I2C_MasterInit(&i2c1_instance,&i2c1_MasterConfig0);
    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

    while(1)
    {
    	if(flag==1)
    	{
    		LPUART_DRV_SendData(INST_LPUART1,(uint8_t *)Msg, strlen(Msg));
    		flag=0;
    	}
    	delay(1);
    }
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */


