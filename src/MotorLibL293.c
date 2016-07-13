/************************************************************************
 *
 * Datei:  			    MotorLibL293.c
 *							
 * Projekt:   			MotorLibL293
 * 
 * Beschreibung:	    Stellt Funktionen zur Steuerung von zwei Motoren über den 
 * 						L293 Controller
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00     19.08.2011   Björn Bosse	  Erstes Build
 *
 *
 ***********************************************************************/
 

#include "MotorLibL293.h"


/**
 * Setzt die benötigten Pins auf Ausgang und 
 * startet den Timer1 für PWM
 *
 * @return -
 */
void InitMotorController(void) {
	// Ausgaenge für den Motortreiber
	L293_PWM_DDR |= (1 << L293_PIN_PWM_LEFT) | (1 << L293_PIN_PWM_RIGHT);
	L293_DIR_DDR |= (1 << L293_PIN_1A) | (1 << L293_PIN_2A) | (1 << L293_PIN_3A) | (1 << L293_PIN_4A);
	
	// Timer1 für PWM konfigurieren und aktivieren
	// PWM Mode 1, Kanal A und B
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	
	// PWM Frequenz CLK/8
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS10);
	
	// Motoren aus
	OCR1A = 0;
	OCR1B = 0;
}



/**
 * Setzt die Drehrichtung der Motoren
 *
 * @param leftDir Drehrichtung des linken Motors (FWD/RWD)
 * @param rightDir Drehrichtung des rechten Motors (FWD/RWD)
 *
 * @return -
 *
 * @example MotorDir(FWD, FWD);
 */
void MotorDir(uint8_t leftDir, uint8_t rightDir) {
    // Rechter Motor
	if(rightDir == FWD) {
		L293_DIR_PORT &= ~(1 << L293_PIN_1A);
		L293_DIR_PORT |= (1 << L293_PIN_2A);
	}else{
		L293_DIR_PORT &= ~(1 << L293_PIN_2A);
		L293_DIR_PORT |= (1 << L293_PIN_1A);
	}
	
	// Linker Motor
	if(leftDir == FWD) {
		L293_DIR_PORT &= ~(1 << L293_PIN_3A);
		L293_DIR_PORT |= (1 << L293_PIN_4A);
	}else{
		L293_DIR_PORT &= ~(1 << L293_PIN_4A);
		L293_DIR_PORT |= (1 << L293_PIN_3A);
	}
}



/**
 * Setzt die Geschwindigkeit der Motoren
 *
 * @param leftSpeed Geschwindigkeit des linken Motors (0 - 255)
 * @param rightSpeed Geschwindigkeit des rechten Motors (0 - 255)
 *
 * @return -
 *
 * @example MotorSpeed(220, 220);
 */
inline void MotorSpeed(uint8_t leftSpeed, uint8_t rightSpeed) {
	OCR1A = leftSpeed;
	OCR1B = rightSpeed;
}



/**
 * Schließt die Motoren kurz und bremst somit
 *
 * @return -
 *
 * @example MotorBreak(); 
 */
inline void MotorBreak() {
	// Linken Motor kurz schließen
	L293_DIR_PORT |= (1 << L293_PIN_1A) | (1 << L293_PIN_2A);
	
	// Rechten Motor kurz schließen
	L293_DIR_PORT |= (1 << L293_PIN_3A) | (1 << L293_PIN_4A);
}



/**
 * Beide Motoren stoppen
 *
 * @param -
 *
 * @return -
 *
 * @example MotorStop();
 */
inline void MotorStop(void) {
	L293_DIR_PORT &= ~(1 << L293_PIN_1A) & 
					 ~(1 << L293_PIN_2A) & 
					 ~(1 << L293_PIN_3A) & 
					 ~(1 << L293_PIN_4A);
}



/**
 * Verbindung der Motoren trennen
 *
 * @param -
 *
 * @return -
 *
 * @example MotorFree();
 */
inline void MotorFree(void) {

}



/**
 * Beide Motoren vorwärts mit bestimmter Geschwindigkeit
 *
 * @param leftSpeed Geschwindigkeit des linken Motors (0 - 255)
 * @param rightSpeed Geschwindigkeit des rechten Motors (0 - 255)
 *
 * @return -
 *
 * @example MotorFWD(220, 220);
 */
void MotorFWD(uint8_t leftSpeed, uint8_t rightSpeed) {
	MotorDir(FWD, FWD);
	MotorSpeed(leftSpeed, rightSpeed);
}



/**
 * Beide Motoren rückwärts mit bestimmter Geschwindigkeit
 *
 * @param leftSpeed Geschwindigkeit des linken Motors (0 - 255)
 * @param rightSpeed Geschwindigkeit des rechten Motors (0 - 255)
 *
 * @return -
 *
 * @example MotorRWD(220, 220);
 */
void MotorRWD(uint8_t leftSpeed, uint8_t rightSpeed) {
	MotorDir(RWD, RWD);
	MotorSpeed(leftSpeed, rightSpeed);
}