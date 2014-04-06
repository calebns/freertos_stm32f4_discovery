/**
 * @file	vectors.c
 * @author	Wojciech Macek <wojciech.macek@gmail.com>
 * @date	16/03/2014
 *
 * @par LICENSE
 * Copyright (C) Wojciech Macek - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited.
 * Proprietary and confidential.
 */

#define INTERRUPT		__attribute__ ((interrupt))
#define INTERRUPT_WEAK	__attribute__ ((interrupt, weak, alias("__Generic_Handler")))

/* MSP stack end */
extern const char __main_stack_end;
/* Hard reset handler */
extern void Reset_Handler(void);

typedef void (*vector_table_funcptr)(void);

INTERRUPT static void __Generic_Handler(void);
INTERRUPT_WEAK void __Reserved(void);
INTERRUPT_WEAK void EXCV_NMI(void);
INTERRUPT_WEAK void EXCV_HardFault(void);
INTERRUPT_WEAK void EXCV_MemManage(void);
INTERRUPT_WEAK void EXCV_BusFault(void);
INTERRUPT_WEAK void EXCV_UsageFault(void);
INTERRUPT_WEAK void EXCV_SVC(void);
INTERRUPT_WEAK void EXCV_DebugMon(void);
INTERRUPT_WEAK void EXCV_PendSV(void);
INTERRUPT_WEAK void EXCV_SysTick(void);
INTERRUPT_WEAK void IRQ_WWDG(void);
INTERRUPT_WEAK void IRQ_PVD(void);
INTERRUPT_WEAK void IRQ_TAMP_STAMP(void);
INTERRUPT_WEAK void IRQ_RTC_WKUP(void);
INTERRUPT_WEAK void IRQ_FLASH(void);
INTERRUPT_WEAK void IRQ_RCC(void);
INTERRUPT_WEAK void IRQ_EXTI0(void);
INTERRUPT_WEAK void IRQ_EXTI1(void);
INTERRUPT_WEAK void IRQ_EXTI2(void);
INTERRUPT_WEAK void IRQ_EXTI3(void);
INTERRUPT_WEAK void IRQ_EXTI4(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream0(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream1(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream2(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream3(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream4(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream5(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream6(void);
INTERRUPT_WEAK void IRQ_ADC(void);
INTERRUPT_WEAK void IRQ_CAN1_TX(void);
INTERRUPT_WEAK void IRQ_CAN1_RX0(void);
INTERRUPT_WEAK void IRQ_CAN1_RX1(void);
INTERRUPT_WEAK void IRQ_CAN1_SCE(void);
INTERRUPT_WEAK void IRQ_EXTI9_5(void);
INTERRUPT_WEAK void IRQ_TIM1_BRK_TIM9(void);
INTERRUPT_WEAK void IRQ_TIM1_UP_TIM10(void);
INTERRUPT_WEAK void IRQ_TIM1_TRG_COM_TIM11(void);
INTERRUPT_WEAK void IRQ_TIM1_CC(void);
INTERRUPT_WEAK void IRQ_TIM2(void);
INTERRUPT_WEAK void IRQ_TIM3(void);
INTERRUPT_WEAK void IRQ_TIM4(void);
INTERRUPT_WEAK void IRQ_I2C1_EV(void);
INTERRUPT_WEAK void IRQ_I2C1_ER(void);
INTERRUPT_WEAK void IRQ_I2C2_EV(void);
INTERRUPT_WEAK void IRQ_I2C2_ER(void);
INTERRUPT_WEAK void IRQ_SPI1(void);
INTERRUPT_WEAK void IRQ_SPI2(void);
INTERRUPT_WEAK void IRQ_USART1(void);
INTERRUPT_WEAK void IRQ_USART2(void);
INTERRUPT_WEAK void IRQ_USART3(void);
INTERRUPT_WEAK void IRQ_EXTI15_10(void);
INTERRUPT_WEAK void IRQ_RTC_Alarm(void);
INTERRUPT_WEAK void IRQ_OTG_FS_WKUP(void);
INTERRUPT_WEAK void IRQ_TIM8_BRK_TIM12(void);
INTERRUPT_WEAK void IRQ_TIM8_UP_TIM13(void);
INTERRUPT_WEAK void IRQ_TIM8_TRG_COM_TIM14(void);
INTERRUPT_WEAK void IRQ_TIM8_CC(void);
INTERRUPT_WEAK void IRQ_DMA1_Stream7(void);
INTERRUPT_WEAK void IRQ_FSMC(void);
INTERRUPT_WEAK void IRQ_SDIO(void);
INTERRUPT_WEAK void IRQ_TIM5(void);
INTERRUPT_WEAK void IRQ_SPI3(void);
INTERRUPT_WEAK void IRQ_UART4(void);
INTERRUPT_WEAK void IRQ_UART5(void);
INTERRUPT_WEAK void IRQ_TIM6_DAC(void);
INTERRUPT_WEAK void IRQ_TIM7(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream0(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream1(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream2(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream3(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream4(void);
INTERRUPT_WEAK void IRQ_ETH(void);
INTERRUPT_WEAK void IRQ_ETH_WKUP(void);
INTERRUPT_WEAK void IRQ_CAN2_TX(void);
INTERRUPT_WEAK void IRQ_CAN2_RX0(void);
INTERRUPT_WEAK void IRQ_CAN2_RX1(void);
INTERRUPT_WEAK void IRQ_CAN2_SCE(void);
INTERRUPT_WEAK void IRQ_OTG_FS(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream5(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream6(void);
INTERRUPT_WEAK void IRQ_DMA2_Stream7(void);
INTERRUPT_WEAK void IRQ_USART6(void);
INTERRUPT_WEAK void IRQ_I2C3_EV(void);
INTERRUPT_WEAK void IRQ_I2C3_ER(void);
INTERRUPT_WEAK void IRQ_OTG_HS_EP1_OUT(void);
INTERRUPT_WEAK void IRQ_OTG_HS_EP1_IN(void);
INTERRUPT_WEAK void IRQ_OTG_HS_WKUP(void);
INTERRUPT_WEAK void IRQ_OTG_HS(void);
INTERRUPT_WEAK void IRQ_DCMI(void);
INTERRUPT_WEAK void IRQ_CRYP(void);
INTERRUPT_WEAK void IRQ_HASH_RNG(void);
INTERRUPT_WEAK void IRQ_FPU(void);

__attribute__ ((section(".vectors")))
vector_table_funcptr vectors[] = {
		(void (*)(void))&__main_stack_end,
		Reset_Handler,
		EXCV_NMI,
		EXCV_HardFault,
		EXCV_MemManage,
		EXCV_BusFault,
		EXCV_UsageFault,
		__Reserved,
		__Reserved,
		__Reserved,
		__Reserved,
		EXCV_SVC,
		EXCV_DebugMon,
		__Reserved,
		EXCV_PendSV,
		EXCV_SysTick,
		IRQ_WWDG,
		IRQ_PVD,
		IRQ_TAMP_STAMP,
		IRQ_RTC_WKUP,
		IRQ_FLASH,
		IRQ_RCC,
		IRQ_EXTI0,
		IRQ_EXTI1,
		IRQ_EXTI2,
		IRQ_EXTI3,
		IRQ_EXTI4,
		IRQ_DMA1_Stream0,
		IRQ_DMA1_Stream1,
		IRQ_DMA1_Stream2,
		IRQ_DMA1_Stream3,
		IRQ_DMA1_Stream4,
		IRQ_DMA1_Stream5,
		IRQ_DMA1_Stream6,
		IRQ_ADC,
		IRQ_CAN1_TX,
		IRQ_CAN1_RX0,
		IRQ_CAN1_RX1,
		IRQ_CAN1_SCE,
		IRQ_EXTI9_5,
		IRQ_TIM1_BRK_TIM9,
		IRQ_TIM1_UP_TIM10,
		IRQ_TIM1_TRG_COM_TIM11,
		IRQ_TIM1_CC,
		IRQ_TIM2,
		IRQ_TIM3,
		IRQ_TIM4,
		IRQ_I2C1_EV,
		IRQ_I2C1_ER,
		IRQ_I2C2_EV,
		IRQ_I2C2_ER,
		IRQ_SPI1,
		IRQ_SPI2,
		IRQ_USART1,
		IRQ_USART2,
		IRQ_USART3,
		IRQ_EXTI15_10,
		IRQ_RTC_Alarm,
		IRQ_OTG_FS_WKUP,
		IRQ_TIM8_BRK_TIM12,
		IRQ_TIM8_UP_TIM13,
		IRQ_TIM8_TRG_COM_TIM14,
		IRQ_TIM8_CC,
		IRQ_DMA1_Stream7,
		IRQ_FSMC,
		IRQ_SDIO,
		IRQ_TIM5,
		IRQ_SPI3,
		IRQ_UART4,
		IRQ_UART5,
		IRQ_TIM6_DAC,
		IRQ_TIM7,
		IRQ_DMA2_Stream0,
		IRQ_DMA2_Stream1,
		IRQ_DMA2_Stream2,
		IRQ_DMA2_Stream3,
		IRQ_DMA2_Stream4,
		IRQ_ETH,
		IRQ_ETH_WKUP,
		IRQ_CAN2_TX,
		IRQ_CAN2_RX0,
		IRQ_CAN2_RX1,
		IRQ_CAN2_SCE,
		IRQ_OTG_FS,
		IRQ_DMA2_Stream5,
		IRQ_DMA2_Stream6,
		IRQ_DMA2_Stream7,
		IRQ_USART6,
		IRQ_I2C3_EV,
		IRQ_I2C3_ER,
		IRQ_OTG_HS_EP1_OUT,
		IRQ_OTG_HS_EP1_IN,
		IRQ_OTG_HS_WKUP,
		IRQ_OTG_HS,
		IRQ_DCMI,
		IRQ_CRYP,
		IRQ_HASH_RNG,
		IRQ_FPU,
};

static void
__Generic_Handler(void)
{
	while (1) {};
}
