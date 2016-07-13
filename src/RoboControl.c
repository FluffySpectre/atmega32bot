/************************************************************************
 *
 * Datei:  			    RoboControl
 *							
 * Projekt:   			Atmega32Bot
 * 
 * Beschreibung:	    Hauptprogramm zur Steuerung des Roboters
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00     16.01.2011   Björn Bosse	  Erstes Build
 *
 *
 ***********************************************************************/


#include "RoboControl.h"

//volatile uint16_t timeCount;	// 



/**
 * Bereitet den Prozessor für die Verwendung vor
 * - Ein-/Ausgänge
 * - Timer
 *
 * @param -
 *
 * @return -
 *
 * @example int main(void) {
 *				Init();
 *
 *				while(1) {}
 *			}
 */
void Init(void) {	
	// Eingaenge für Sensoren etc.
	DDRA &=  ~(1 << PA0) | ~(1 << PA1) | ~(1 << PA2) | ~(1 << PA3) | ~(1 << PA4) | ~(1 << PA5) | ~(1 << PA6) | ~(1 << PA7);
	
	// IR Sensoren
	DDRB |= IR_FRONT_ENABLE;
	
	// Front- und Heckledausgänge
	DDRC |= FRONT_LED_LEFT | FRONT_LED_RIGHT | BACK_LED_LEFT | BACK_LED_RIGHT; 
	
	// LCD initialisieren und konfigurieren
	Lcd_Init();
	Lcd_ConfigCursor(OFF);
	Lcd_ConfigBlink(OFF);
	
	// Motorcontroller initialisieren
	InitMotorController();
	
	// ADC initialisieren
	Adc_Init();
	
	// IR aktivieren
	EnableIRTransmitter();
	
	// Led check
	FrontLED(ON, ON);
	BackLED(ON, ON);
	MSleep(500);
	FrontLED(OFF, OFF);
	BackLED(OFF, OFF);
	
	// Willkommensmeldung anzeigen
	Lcd_PrintLine("  -AtmegaBot-", Line1);
	Lcd_PrintLine("by Bjoern Bosse", Line2);
	
	uint16_t dur = 350;
	uint8_t i;
	Lcd_SetMyChar(5, 0b00000, 0b10000, 0b01000, 0b00100, 0b00010, 0b00001, 0b00000, 0b00000);
	for (i = 0; i < 5; i++) {
		Lcd_PrintChar('|', Line1, Pos0);
		Lcd_PrintChar('|', Line1, Pos15);
		MSleep(dur);
		Lcd_PrintChar('/', Line1, Pos0);
		Lcd_PrintChar('/', Line1, Pos15);
		MSleep(dur);
		Lcd_PrintChar('-', Line1, Pos0);
		Lcd_PrintChar('-', Line1, Pos15);
		MSleep(dur);
		Lcd_SetPos(Line1, Pos0);
		Lcd_SendData(0x05);
		Lcd_SetPos(Line1, Pos15);
		Lcd_SendData(0x05);
		MSleep(dur);
		Lcd_PrintChar('|', Line1, Pos0);
		Lcd_PrintChar('|', Line1, Pos15);
		MSleep(dur);
		Lcd_PrintChar('/', Line1, Pos0);
		Lcd_PrintChar('/', Line1, Pos15);
		MSleep(dur);
		Lcd_PrintChar('-', Line1, Pos0);
		Lcd_PrintChar('-', Line1, Pos15);
		MSleep(dur);
		Lcd_SetPos(Line1, Pos0);
		Lcd_SendData(0x05);
		Lcd_SetPos(Line1, Pos15);
		Lcd_SendData(0x05);
		MSleep(dur);
	}
	
	// LCD löschen
	Lcd_Clear();
}



/**
 * Schaltet die Status-LED ein/aus
 *
 * @param status Der Status der LED (ON/OFF)
 *
 * @return -
 *
 * @example StatusLED(ON);
 */
inline void StatusLED(uint8_t status) {
	PORTC = (PORTC & ~(1 << STATUS_LED)) | (status << STATUS_LED);
}



/**
 * Schaltet die Frontscheinwerfer ein/aus
 *
 * @param left Linke LED
 * @param right Rechte LED
 *
 * @return -
 *
 * @example FrontLED(ON, OFF); // LED links an
 */
inline void FrontLED(uint8_t left, uint8_t right) {
	// 
	if(left == ON) {
		PORTC &= ~FRONT_LED_LEFT;
	} else {
		PORTC |= FRONT_LED_LEFT;
	}

	// 
	if(right == ON) {
		PORTC &= ~FRONT_LED_RIGHT;
	} else {
		PORTC |= FRONT_LED_RIGHT;
	}
}



/**
 * Schaltet die Rückscheinwerfer ein/aus
 *
 * @param left Linke LED
 * @param right Rechte LED
 *
 * @return -
 *
 * @example BackLED(ON, OFF); // LED links an
 */
inline void BackLED(uint8_t left, uint8_t right) {
	// 
	if(left == ON) {
		PORTC &= ~BACK_LED_LEFT;
	} else {
		PORTC |= BACK_LED_LEFT;
	}

	// 
	if(right == ON) {
		PORTC &= ~BACK_LED_RIGHT;
	} else {
		PORTC |= BACK_LED_RIGHT;
	}
}



/**
 * Gibt den aktuellen Wert des Lichtsensors zurück
 * Durchschnittswert aus 32 Messungen
 *
 * @param -
 *
 * @return 
 *
 * @example -
 */
uint16_t GetLdrValue(void) {
	return Adc_Read_Avg(LDR_ADC_PORT, 32);
}



/**
 * Aktiviert den IR-Transmitter
 *
 * @param -
 *
 * @return 
 *
 * @example -
 */
void EnableIRTransmitter() {
	PORTB |= IR_FRONT_ENABLE;
}



/**
 * Deaktiviert den IR-Transmitter
 *
 * @param -
 *
 * @return 
 *
 * @example -
 */
void DisableIRTransmitter() {
	PORTB &= ~IR_FRONT_ENABLE;
}



/**
 * Gibt den aktuellen Wert des IR-Sensors zurück
 * Durchschnittswert aus 32 Messungen
 *
 * @param -
 *
 * @return 
 *
 * @example -
 */
uint16_t GetIRValue(void) {
	return Adc_Read_Avg(IR_FRONT_RX_PORT, 32);
}



/**
 * Timergesteuerte Sleep-Funktion
 * Wartet t ms
 *
 * @param t Zeit in Millisekunden die gewartet werden soll
 *
 * @return -
 *
 * @example MSleep(100); // Wartet 100 ms
 */
void MSleep(uint16_t t) {
	uint16_t z;
	
	for(z = 0; z < t; z++) {
		_delay_ms(1);
		
		//timeCount = 0;
		//while (timeCount < 36);
	}
}



/**
 * Hauptroutine
 *
 * @param -
 *
 * @return 0
 *
 * @example -
 */
int main(void) {
	uint8_t i = 0;
	uint8_t z = 0;
	uint16_t value = 0;
	uint16_t testCntr = 0;
	char bufValue[4];
	
	// Prozessor und Peripherie initialisieren
	Init(); 
	
	// Draw mouth
	//Lcd_PrintLine("     #####", Line2);
	
	// Programmschleife
	while(1) {
		value = GetIRValue();
		itoa(value, bufValue, 10);
		
		
		if (value < IR_FRONT_NEAR) {
			BackLED(ON, ON);
			
			MotorDir(FWD, RWD);
			MotorSpeed(75, 75);
			MSleep(820);
		} else {
			BackLED(OFF, OFF);
			
			MotorDir(FWD, FWD);
			MotorSpeed(65, 65);
		}
		
		MSleep(10);
		
		/*
		Lcd_PrintLine("IR-Sensor", Line1);
		Lcd_PrintLine(bufValue, Line2);
		MSleep(200);
		*/
		
		/*
		MotorDir(FWD, FWD);
		MotorSpeed(255, 255);
		MSleep(500);
		
		MotorDir(FWD, RWD);
		MotorSpeed(190, 190);
		MSleep(300);
		
		MotorDir(RWD, FWD);
		MotorSpeed(190, 190);
		MSleep(300);
		
		MotorDir(RWD, RWD);
		MotorSpeed(255, 255);
		MSleep(500);
		*/
		
		// LCD Test
		/*
		lcd_PrintLine("Warte...", Line1);
		i++;
		if(i < 100) {
			lcd_PrintBar(i, 100, Line2);
		} else { 
			i = 0;
		}
		*/
		
		//Lcd_Clear();
		//testCntr = 0;
		
		/* (i = 0; i < 16; i++) {
			//Lcd_Clear();
			if (i > 0) Lcd_PrintChar(' ', Line1, i-1);
			
			Lcd_PrintManAnimated(Line1, i);
			MSleep(200);
		}*/
		
		
		/*
		Lcd_Clear();
		Lcd_PrintLine("Test 1 - LDR", Line1);
		MSleep(3000);
		Lcd_Clear();
		
		
		// Lichtsensor abfragen (5 sek.)
		while (testCntr < 500) {
			value = GetLdrValue();
			
			if(value >= LDR_THRESHOLD) {
				FrontLED(ON, ON);
				BackLED(ON, ON);
			} else {
				FrontLED(OFF, OFF);
				BackLED(OFF, OFF);
			}
			
			//itoa(value, bufValue, 10);
			
			Lcd_PrintLine("Lichtsensor", Line1);
			//Lcd_PrintLine(bufValue, Line2);
			Lcd_PrintBar(value, 1023, Line2);
			
			MSleep(10);
			testCntr++;
		}
		
		Lcd_Clear();
		testCntr = 0;
		
		Lcd_PrintLine("Test 2 - Motor", Line1);
		MSleep(3000);
		Lcd_Clear();
		
		while (testCntr < 3) {
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Vorwaerts", Line2);
			MotorFWD(75, 75);
			MSleep(2000);
			
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Bremsen", Line2);
			MotorBreak();
			MSleep(500);
			
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Rueckwaerts", Line2);
			MotorRWD(75, 75);
			MSleep(2000);
			
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Bremsen", Line2);
			MotorBreak();
			MSleep(500);
			
			// 
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Rechts drehen", Line2);
			MotorDir(FWD, RWD);
			MotorSpeed(75, 75);
			MSleep(2000);
			
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Bremsen", Line2);
			MotorBreak();
			MSleep(500);
			
			//
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Links drehen", Line2);
			MotorDir(RWD, FWD);
			MotorSpeed(75, 75);
			MSleep(2000);
			
			Lcd_Clear();
			Lcd_PrintLine("Aktion:", Line1);
			Lcd_PrintLine("Bremsen", Line2);
			MotorBreak();
			MSleep(500);
			
			testCntr++;
		}
		*/
		
		/*
		MotorDir(FWD, FWD);
		for(i = 0; i < 255; i++) {
			MotorSpeed(i, i);
			MSleep(1);
		}
		for(i = 255; i > 0; i -= 5) {
			MotorSpeed(i, i);
			MSleep(30);
		}
		
		MotorBreak();
		MSleep(500);
		
		MotorDir(RWD, RWD);
		for(i = 0; i < 255; i++) {
			MotorSpeed(i, i);
			MSleep(1);
		}
		for(i = 255; i > 0; i -= 5) {
			MotorSpeed(i, i);
			MSleep(30);
		}
		
		MotorBreak();
		MSleep(500);
		
		MotorDir(FWD, RWD);
		for(i = 0; i < 255; i += 5) {
			MotorSpeed(i, i);
			MSleep(10);
		}
		for(i = 255; i > 0; i--) {
			MotorSpeed(i, i);
			MSleep(10);
		}
		
		MotorBreak();
		MSleep(500);
		
		MotorDir(RWD, FWD);
		for(i = 0; i < 255; i += 5) {
			MotorSpeed(i, i);
			MSleep(10);
		}
		for(i = 255; i > 0; i--) {
			MotorSpeed(i, i);
			MSleep(10);
		}
		
		MotorBreak();
		MSleep(500);
		*/
		
		/*
		MotorDir(FWD, RWD);
		for(i = 0; i < 255; i += 5) {
			MotorSpeed(i, i);
			MSleep(100);
		}
		for(i = 255; i > 0; i-= 5) {
			MotorSpeed(i, i);
			MSleep(100);
		}
		*/
		
		/*
		MotorDir(FWD, FWD);
		MotorSpeed(250, 250);
		MSleep(700);
		
		MotorBreak();
		MSleep(200);
		
		MotorDir(FWD, RWD);
		MotorSpeed(250, 250);
		MSleep(195);
		
		MotorBreak();
		MSleep(200);
		*/
	}
	
	return 0;
}
