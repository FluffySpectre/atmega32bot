/************************************************************************
 *
 * Datei:  			    AdcLib.h
 *							
 * Projekt:   			AtmegaBot
 * 
 * Beschreibung:	    Stellt Funktionen f�r die Auswertung 
 *						der ADC-Kan�le des Atmega32 bereit
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00	    16.01.2011   Bj�rn Bosse	  Erstes Build
 *
 *
 ***********************************************************************/


#ifndef ADCLIB_H
#define ADCLIB_H

#include <avr/io.h>

// Prototypen
void Adc_Init(void);
uint16_t Adc_Read(uint8_t channel);
uint16_t Adc_Read_Avg(uint8_t channel, uint8_t numOfSamples);

#endif // ADCLIB_H
