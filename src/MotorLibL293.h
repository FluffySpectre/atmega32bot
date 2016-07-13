/************************************************************************
 *
 * Datei:  			    MotorLibL293.h
 *							
 * Projekt:   			MotorLibL293
 * 
 * Beschreibung:	    	Stellt Funktionen zur Steuerung von zwei Motoren über den 
 * 						L293 Controller
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00     19.08.2011   Björn Bosse	  Erstes Build
 *
 *
 ***********************************************************************/
 

#ifndef MOTORLIBL293_H
#define MOTORLIBL293_H

// Falls Taktfrequenz noch nicht eingestellt, dies jetzt tun
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>


// Pinbelegung des Motortreibers
#define L293_PWM_DDR       DDRD
#define L293_PWM_PORT      PORTD
#define L293_DIR_DDR       DDRC
#define L293_DIR_PORT      PORTC

// Linke H-Brücke
#define L293_PIN_PWM_LEFT  PD4
#define L293_PIN_1A        PC0
#define L293_PIN_2A		   PC1

// Rechte H-Brücke
#define L293_PIN_PWM_RIGHT PD5
#define L293_PIN_3A        PC2
#define L293_PIN_4A        PC3

//  
#define RWD 		   		 0
#define FWD            		 1 


// Funktionsdeklaration
void InitMotorController(void);
void MotorDir(uint8_t leftDir, uint8_t rightDir);
inline void MotorSpeed(uint8_t leftSpeed, uint8_t rightSpeed);
inline void MotorBreak(void);
inline void MotorStop(void);
inline void MotorFree(void);
void MotorFWD(uint8_t leftSpeed, uint8_t rightSpeed);
void MotorRWD(uint8_t leftSpeed, uint8_t rightSpeed);


#endif // MotorLib.h
