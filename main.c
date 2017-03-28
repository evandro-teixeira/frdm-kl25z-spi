/*
 * main.c
 * Autor: Evandro Teixeira
 */
 
#include "MKL25Z4.h"
#include "spi.h"
 
void delay(uint32_t t);
uint8_t ch;
 
int main(void)
{
 
	// Incializa PTD1 - LED AZUL
	SIM_SCGC5  |= SIM_SCGC5_PORTD_MASK;
	PORT_PCR_REG(PORTD_BASE_PTR,1) = PORT_PCR_MUX(1);
	GPIOD_PDDR |= (1 << 1);
 
	// Inicializa PTE4 - Pino CS
	SIM_SCGC5  |= SIM_SCGC5_PORTE_MASK;
	PORT_PCR_REG(PORTE_BASE_PTR,4) = PORT_PCR_MUX(1);
	GPIOE_PDDR |= (1 << 4);
 
	// Set pino CS = 1
	GPIOE_PSOR |= (1 << 4);
 
	// Inicaliza SPI 1 - Alternativa de Pino 0
	// PTE2 - SPI1_SCK
	// PTE1 - SPI1_MOSI
	// PTE3 - SPI1_MISO
	// Clock = 165 KHz - Prescale = 3 - Divisor = 4
	// Chip Selet Controle Manual
	spi_init(SPI_1,ALT_0,PRESCALE_2,DIVISOR_1,CS_MAN);
 
    for (;;)
    {
    	// Intervalo de tempo
    	delay(50000);
    	// CS = 0 - Inicia a comunicação com o MCP41010
    	GPIOE_PDOR &=~ (1 << 4);
    	// Aguarda perido antes de eviar os dados para o MCP41010
    	delay(20);
    	// Envia comando de Escrita para o MCP41010
    	spi_send(SPI_1,0b00010001);
    	// Envia dado  para configurar o valor do potenciometro
    	spi_send(SPI_1,ch);
    	// Aguarda um perido de tempo para encerrar a comunicação com o MCP41010
    	delay(200);
    	// CS = 1 - Encerra a comunicação com o MCP41010
    	GPIOE_PSOR |= (1 << 4);
    	// Incrementa a variavel
    	ch++;
    	if(ch >= 255) ch = 0;
    	// Inverter o valor do pino do LED Azul-(ON ~ OFF)
    	GPIOD_PTOR = (1 << 1);
    }
 
    return 0;
}
 
void delay(uint32_t t)
{
	uint32_t i = 0;
 
	for(i=0;i<=t;i++)
	{
		__asm("nop");
	}
}