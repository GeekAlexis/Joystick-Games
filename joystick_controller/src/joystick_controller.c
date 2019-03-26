#include "string.h"
#include "board.h"

const char begin_flag = 'b';
const char stop_flag = 's';
static volatile bool fDebouncing;
static volatile bool blink;

/******************************************************************************
* Typedefs and defines
*****************************************************************************/

/******************************************************************************
* Local Functions
*****************************************************************************/

/******************************************************************************
* Main method
*****************************************************************************/

void TIMER0_IRQHandler(void) {
	fDebouncing = false;
	Chip_TIMER_Disable(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_ClearMatch(LPC_TIMER0,0);
}

void TIMER1_IRQHandler(void) {
	Board_LED_Toggle(0);
	Chip_TIMER_Reset(LPC_TIMER1);
	Chip_TIMER_ClearMatch(LPC_TIMER1,1);
}

//led blinking button
void GPIO_IRQHandler(void)
{
	if(fDebouncing) {}
	else {
		blink = !blink;
		fDebouncing = true;
		Chip_TIMER_Enable(LPC_TIMER0);
		if(blink) {
			Board_LED_Toggle(0);
			Chip_TIMER_Reset(LPC_TIMER1);
			Chip_TIMER_Enable(LPC_TIMER1);
		}
		else {
			Board_LED_Set(0, false);
			Chip_TIMER_Disable(LPC_TIMER1);
		}
	}
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIOINT_PORT2, 1 << 10);
}

int main (void) {
  SystemCoreClockUpdate();
  Board_Init();
  Board_LED_Set(0, false);
  int PrescaleValue = 120000;
  blink = false;

  Chip_GPIO_SetPinDIRInput(LPC_GPIO2, GPIOINT_PORT2, 22); // center
  Chip_GPIO_SetPinDIRInput(LPC_GPIO2, GPIOINT_PORT2, 23); // left
  Chip_GPIO_SetPinDIRInput(LPC_GPIO2, GPIOINT_PORT2, 25); // up
  Chip_GPIO_SetPinDIRInput(LPC_GPIO2, GPIOINT_PORT2, 26); // right
  Chip_GPIO_SetPinDIRInput(LPC_GPIO2, GPIOINT_PORT2, 27); // down
  LPC_GPIO2->DIR &= ~(1<<22|1<<23|1<<25|1<<26|1<<27); // set up GPIO port2

  //led blinking
  Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, GPIOINT_PORT2, 1 << 10);
  NVIC_ClearPendingIRQ(GPIO_IRQn);
  NVIC_EnableIRQ(GPIO_IRQn);

  Chip_TIMER_Init(LPC_TIMER0);
  Chip_TIMER_PrescaleSet(LPC_TIMER0, PrescaleValue);
  Chip_TIMER_SetMatch(LPC_TIMER0, 0, 100);
  Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);

  Chip_TIMER_Init(LPC_TIMER1);
  Chip_TIMER_PrescaleSet(LPC_TIMER1, PrescaleValue);
  Chip_TIMER_SetMatch(LPC_TIMER1, 1, 250);
  Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);

  Chip_TIMER_Init(LPC_TIMER2); // timer for debouncing joystick
  Chip_TIMER_PrescaleSet(LPC_TIMER2, PrescaleValue);

  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NVIC_EnableIRQ(TIMER0_IRQn);
  NVIC_ClearPendingIRQ(TIMER1_IRQn);
  NVIC_EnableIRQ(TIMER1_IRQn);

  Board_UART_Init(LPC_UART0);
  Chip_UART_Init(LPC_UART0);
  Chip_UART_SetBaud(LPC_UART0, 115200);
  Chip_UART_ConfigData(LPC_UART0, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
  Chip_UART_IntEnable(LPC_UART0, (UART_IER_RBRINT | UART_IER_RLSINT));
  Chip_UART_TXEnable(LPC_UART0);
  Chip_UART_SetupFIFOS(LPC_UART0, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
							UART_FCR_TX_RS | UART_FCR_TRG_LEV3));

  // command to send to python script
  char byte;
  uint32_t pinVal = 0;

  while(1) {
    pinVal = LPC_GPIO2->PIN;
    //center
    if((pinVal & (1 << 22)) == 0) {
    	printf("center pushed\n");
    	Chip_UART_Send(LPC_UART0, &begin_flag, 1);
    }
    //down
	if((pinVal & (1 << 27)) == 0) {
		byte = 'd';
		Chip_UART_Send(LPC_UART0, &byte, 1);
	}
	//right
	if((pinVal & (1 << 26)) == 0) {
		byte = 'r';
		Chip_UART_Send(LPC_UART0, &byte, 1);
	}
	//up
	if((pinVal & (1 << 25)) == 0) {
		byte = 'u';
		Chip_UART_Send(LPC_UART0, &byte, 1);
	}
	//left
	if((pinVal & (1 << 23)) == 0) {
		byte = 'l';
		Chip_UART_Send(LPC_UART0, &byte, 1);
	}

	// debouncing timer
	Chip_TIMER_Reset(LPC_TIMER2);
	Chip_TIMER_Enable(LPC_TIMER2);
	while(Chip_TIMER_ReadCount(LPC_TIMER2) < 75);
	Chip_TIMER_Disable(LPC_TIMER2);
  }
}

