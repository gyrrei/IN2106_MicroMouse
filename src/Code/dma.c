#include "dma.h"

unsigned int adcData[32] __attribute__((space(dma)));

void initDmaChannel4(void)
{

	//DMA4CON		= 0x;
	DMA4CONbits.CHEN = 0;	 // Disable channel
	DMA4CONbits.SIZE = 0;	 // Data transfer size (1=byte,0=word)
	DMA4CONbits.DIR = 0;	  // Transfer direction (1=read RAM,write to Periph. 0= read from periph, write to RAM)
	DMA4CONbits.HALF = 0;	 // Early block transfer complete interrupt (1=interrupt on half block transfer,0=int on full block transfer)
	DMA4CONbits.NULLW = 1;	// Null Data write to peripheral mode (Null data write to peripheral as well as write ro RAM, 0=normal)
	DMA4CONbits.AMODE = 0b10; // DMA channel operating mode	2,3=Peripheral Indirect Addressing mode, 1=Register Indirect without Post-Increment mode, 0=Register Indirect with Post-Increment mode
	DMA4CONbits.MODE = 0;	 // DMA channel opering mode select 0=Continuous, Ping-Pong modes disabled, 2=continuous, ping-pong

	DMA4REQbits.FORCE = 0;   // Force DMA Transfer (1=single DMA transfer,0=automatic initiated by DMA request)
	DMA4REQbits.IRQSEL = 13; // DMA Peripheral IRQ number select (ADC1)

	DMA4STA = (__builtin_dmaoffset(&(adcData[0]))); // start address of DMA RAM
	DMA4PAD = (volatile unsigned int)&ADC1BUF0;		// address of peripheral sfr (0x0300)
	DMA4CNT = 2;									// we have 3 a2d  s/h channels for  measurement

	IFS2bits.DMA4IF = 0;  // Clear DMA interrupt
	IEC2bits.DMA4IE = 1;  // enable interrupt
	IPC11bits.DMA4IP = 6; // set priority of interrupt

	DMA4CONbits.CHEN = 1; // enable channel
}

void __attribute__((interrupt, auto_psv)) _DMA4Interrupt(void)
{
	IFS2bits.DMA4IF = 0; // Clear DMA interrupt
};

