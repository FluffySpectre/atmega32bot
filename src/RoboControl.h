/************************************************************************
 *
 * Datei:  			    RoboControl.h
 *							
 * Projekt:   			AtmegaBot
 * 
 * Beschreibung:	    Hauptprogramm zur Steuerung des Roboters
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00	    10.07.2011   Björn Bosse	  Erstes Build
 *
 *
 ***********************************************************************/


#ifndef ROBOCONTROL_H
#define ROBOCONTROL_H

#ifndef F_CPU
#define F_CPU 8000000UL				  // Quarz mit 8 MHz
//#define F_CPU 3686400			      // Quarz mit 3.6864 MHz
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LcdLib.h"
#include "MotorLibL293.h"
#include "AdcLib.h"

#define OFF			        	0	  // 
#define ON  		   			1	  // 

#define FRONT_LED_LEFT          (1 << PC6)
#define FRONT_LED_RIGHT         (1 << PC7)
#define BACK_LED_LEFT           (1 << PC4)
#define BACK_LED_RIGHT          (1 << PC5)
#define STATUS_LED        		PC6	       // Pin der Status-LED

#define IR_FRONT_ENABLE         (1 << PB1)
#define IR_FRONT_RX_PORT        0
#define IR_FRONT_NEAR           512

#define LDR_ADC_PORT			7
#define LDR_THRESHOLD           900

// Funktionsdeklaration

void Init(void);
inline void StatusLED(uint8_t status);
inline void FrontLED(uint8_t left, uint8_t right);
inline void BackLED(uint8_t left, uint8_t right);
uint16_t GetLdrValue(void);
void EnableIRTransmitter();
void DisableIRTransmitter();
uint16_t GetIRValue(void);
void MSleep(uint16_t t);

#endif // ROBOCONTROL_H
