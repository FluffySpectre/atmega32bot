/************************************************************************
 *
 * Datei:  			    LcdLib.h
 *							
 * Projekt:   			LcdLib
 * 
 * Beschreibung:	    Stellt Funktionen zur Steuerung des myAVR LCD Add-On zur Verfuegung
 * 						Schaltung: Port-D 0 bis 7: Display, Port-B 0 und 1: Display
 *
 *
 * Version    Datum          Autor                 Kommentare
 * -------  ----------   --------------   ------------------------------
 * 1.00	    20.07.2011   Björn Bosse	  Erstes Build
 * 2.00     31.07.2011   Björn Bosse      Aufteilung in Source und Header
 *
 *
 ***********************************************************************/


#ifndef LCDLIB_H
#define	LCDLIB_H

// Falls F_CPU noch nicht definiert wurde
// dies jetzt tun
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include	<avr\io.h>						// AVR Register und Konstantendefinitionen
#include	<util\delay.h>					// AVR Warteschleife

// Makros ------------------------------------------------------------------------------------------
#define		WaitMs	_delay_ms				// Wrapper fuer Warteschleife

#define		CTRL_LcdOPTIONS	0x08

// Daten Ports LCD
#define		LCDRS		PORTD
#define		LCDR		PORTD				
#define		LCDDB4		PORTD				
#define		LCDDB5		PORTD				
#define		LCDDB6		PORTD				
#define		LCDDB7		PORTD				
#define		LCDRW		PORTB				
#define		LCDA		PORTB				

// Datenrichtungsregister LCD
#define		LCDRS_DDR	DDRD
#define		LCDR_DDR	DDRD
#define		LCDDB4_DDR	DDRD
#define		LCDDB5_DDR	DDRD
#define		LCDDB6_DDR	DDRD
#define		LCDDB7_DDR	DDRD
#define		LCDRW_DDR	DDRB
#define		LCDA_DDR	DDRB

// Bits LCD
#define		LCDRS_BIT	PD0
#define		LCDR_BIT	PD1					
#define		LCDDB4_BIT	PD2					// 4. Datenbit
#define		LCDDB5_BIT	PD3					// 5. Datenbit
#define		LCDDB6_BIT	PD6					// 6. Datenbit
#define		LCDDB7_BIT	PD7					// 7. Datenbit
#define		LCDRW_BIT	PB0					// High -> Read-Mode, Low -> Write-Mode
#define		LCDA_BIT	PB1


// Typen -------------------------------------------------------------------------------------------
typedef enum LCDOPTION {
	OFF = 0,
	ON = 1
} Lcdoption_t;



typedef enum LCDLINE {
	Line1 = 1,
	Line2 = 2
} Lcdline_t;



typedef enum LCDPOSITION {
	Pos0 = 0,
	Pos1 = 1,
	Pos2 = 2,
	Pos3 = 3,
	Pos4 = 4,
	Pos5 = 5,
	Pos6 = 6,
	Pos7 = 7,
	Pos8 = 8,
	Pos9 = 9,
	Pos10 = 10,
	Pos11 = 11,
	Pos12 = 12,
	Pos13 = 13,
	Pos14 = 14,
	Pos15 = 15
} Lcdposition_t;



// Variablen ---------------------------------------------------------------------------------------
uint8_t uc_LCDBlink_g	    = 0x00;
uint8_t uc_LCDCursor_g	    = 0x02;
uint8_t uc_LCDDisplay_g	    = 0x04;
uint8_t uc_LCDDimmer_g	    = 0xFF;
uint8_t uc_LCDBacklightOn	= 1;



// Prototypen --------------------------------------------------------------------------------------
void Lcd_SendByte(uint8_t uc_Data);
void Lcd_SendCmd(uint8_t uc_Cmd);
void Lcd_SendData(uint8_t uc_Data);
void Lcd_Init(void);
void Lcd_InitPorts(void);
void Lcd_SetOn(void);
void Lcd_SetOff(void);
void Lcd_ConfigBlink(Lcdoption_t e_Option);
void Lcd_ConfigCursor(Lcdoption_t e_Option);
void Lcd_SetBacklight(Lcdoption_t e_Option);
void Lcd_SetBacklightDimmer(uint8_t uc_Value);
void Lcd_Clear(void);
void Lcd_SetHome(void);
void Lcd_SetPos(Lcdline_t e_Line, 
				Lcdposition_t e_Pos);
void Lcd_PrintChar(uint8_t uc_Char, 
				   Lcdline_t e_Line, 
				   Lcdposition_t e_Pos);
void Lcd_PrintLine(const uint8_t *puc_Text, 
				   Lcdline_t e_Line);
void Lcd_PrintText(const uint8_t *puc_Text);
void Lcd_PrintBar(uint16_t us_Value, 
				  uint16_t us_Maximum, 
			      Lcdline_t e_Line);
void Lcd_SetMyChar(uint8_t uc_Memory,
				   uint8_t uc_PxRow1,
				   uint8_t uc_PxRow2,
				   uint8_t uc_PxRow3,
				   uint8_t uc_PxRow4,
				   uint8_t uc_PxRow5,
				   uint8_t uc_PxRow6,
				   uint8_t uc_PxRow7,
				   uint8_t uc_PxRow8);
				   
				   

// Funktionen --------------------------------------------------------------------------------------

/**
  * @brief Sendet ein Byte in 4 Bit Paketen an das LCD (Nicht direkt aufrufen)
  *
  * @param data Zu sendendes Byte
  *
  * @return -
  *
  * @example LcdsendByte('F');
  */
void Lcd_SendByte(uint8_t uc_Data) {
	uint8_t uc_Halfbyte;

	// Obere 4 Bits ------------------------------
	// Daten in Zwischenspeicher schreiben
	uc_Halfbyte = uc_Data;
	// Untere 4 Bits der Daten loeschen
	uc_Halfbyte &= 0xF0;
	// Obere 4 Bits Port-D (D4-D7) loeschen
	LCDDB4 &= ~(1 << LCDDB4_BIT);
	LCDDB5 &= ~(1 << LCDDB5_BIT);
	LCDDB6 &= ~(1 << LCDDB6_BIT);
	LCDDB7 &= ~(1 << LCDDB7_BIT);
	
	// Daten auf Port-D (D4-D7) mappen
	LCDDB4 |= ((uc_Halfbyte >> 4) << LCDDB4_BIT);
	LCDDB5 |= ((uc_Halfbyte >> 5) << LCDDB5_BIT);
	LCDDB6 |= ((uc_Halfbyte >> 6) << LCDDB6_BIT);
	LCDDB7 |= ((uc_Halfbyte >> 7) << LCDDB7_BIT);
	
	// Enable (Port-D) auf 1
	LCDR |= (1 << LCDR_BIT);
	// Enable (Port-D) auf 0
	LCDR &= ~(1 << LCDR_BIT);
	// --------------------------------------------
	
	// Untere 4 Bits ------------------------------
	// Daten in Zwischenspeicher schreiben
	uc_Halfbyte = uc_Data;
	
	// Obere 4 Bits der Daten loeschen
	uc_Halfbyte &= 0x0F;
	// Untere 4 Bits hochschieben
	uc_Halfbyte = uc_Halfbyte << 4;
	
	// Obere 4 Bits Port-D (D4-D7) loeschen
	LCDDB4 &= ~(1 << LCDDB4_BIT);
	LCDDB5 &= ~(1 << LCDDB5_BIT);
	LCDDB6 &= ~(1 << LCDDB6_BIT);
	LCDDB7 &= ~(1 << LCDDB7_BIT);
	
	// Daten auf Port-D (D4-D7) mappen
	LCDDB4 |= ((uc_Halfbyte >> 4) << LCDDB4_BIT);
	LCDDB5 |= ((uc_Halfbyte >> 5) << LCDDB5_BIT);
	LCDDB6 |= ((uc_Halfbyte >> 6) << LCDDB6_BIT);
	LCDDB7 |= ((uc_Halfbyte >> 7) << LCDDB7_BIT);
	
	// Enable (Port-D) auf 1
	LCDR |= (1 << LCDR_BIT);
	// Enable (Port-D) auf 0
	LCDR &= ~(1 << LCDR_BIT);
	// --------------------------------------------
	
	WaitMs(1);
	
	// Ports auf Low schalten
	LCDRS &= ~(1 << LCDRS_BIT);
	LCDR &= ~(1 << LCDR_BIT);
	LCDDB4 &= ~(1 << LCDDB4_BIT);
	LCDDB5 &= ~(1 << LCDDB5_BIT);
	LCDDB6 &= ~(1 << LCDDB6_BIT);
	LCDDB7 &= ~(1 << LCDDB7_BIT);
}



/**
  * @brief Sendet ein Steuerkommando an das LCD
  *
  * @param uc_Cmd Zu sendendes Byte
  *
  * @return -
  *
  * @example Lcd_SendCmd(0x28);	// 4 Bit, 2 Zeilen, 5x7 Matrix
  */
void Lcd_SendCmd(uint8_t uc_Cmd) {
	LCDRS &= ~(1 << LCDRS_BIT);
	Lcd_SendByte(uc_Cmd);
}



/**
  * @brief Sendet ein Datenbyte an das LCD
  *
  * @param uc_Data Zu sendendes Byte
  *
  * @return -
  *
  * @example -
  */
void Lcd_SendData(uint8_t uc_Data) {
	LCDRS |= (1 << LCDRS_BIT);
	Lcd_SendByte(uc_Data);
}



/**
  * @brief Initialisiert das LCD
  *
  * @return -
  *
  * @example -
  */
void Lcd_Init(void) {
	Lcd_InitPorts();
	
	// Warten bis LCD gebootet
	WaitMs(50);
	
	// LCD-Hintergrundbeleuchtung einschalten
	Lcd_SetBacklight(ON);

	// Soft-Reset
	// D4, D5 = 1
	LCDDB4 |= (1 << LCDDB4_BIT);
	LCDDB5 |= (1 << LCDDB5_BIT);
	// D4, D5 = 1 Enable 1
	LCDR |= (1 << LCDR_BIT);
	// D4, D5 = 1 Enable 0
	LCDR &= ~(1 << LCDR_BIT);
	WaitMs(5);
	// D4, D5 = 1 Enable 1
	LCDR |= (1 << LCDR_BIT);
	// D4, D5 = 1 Enable 0
	LCDR &= ~(1 << LCDR_BIT);
	WaitMs(1);
	// D4, D5 = 1 Enable 1
	LCDR |= (1 << LCDR_BIT);
	// D4, D5 = 1 Enable 0
	LCDR &= ~(1 << LCDR_BIT);
	WaitMs(5);

	// 4-Bit Modus aktivieren
	LCDDB4 &= ~(1 << LCDDB4_BIT);
	// D4, D5 = 1 Enable 1
	LCDR |= (1 << LCDR_BIT);
	// D4, D5 = 1 Enable 0
	LCDR &= ~(1 << LCDR_BIT);
	WaitMs(5);

	// 4 Bit, 2 Zeilen, 5x7 Matrix
	Lcd_SendCmd(0x28);
	// LCD ausschalten
	Lcd_SetOff();
	// Einfuegemodus, kein Scrollen
	Lcd_SendCmd(0x06);
	// LCD einschalten
	Lcd_SetOn();
	// LCD leeren
	Lcd_Clear();
}



/**
  * @brief Konfiguriert die Kanaele des LCD's
  *
  * @return -
  *
  * @example -
  */
void Lcd_InitPorts(void) {
	// Ausgangsports schalten
	LCDRS_DDR |= (1 << LCDRS_BIT);
	LCDR_DDR |= (1 << LCDR_BIT);
	LCDDB4_DDR |= (1 << LCDDB4_BIT);
	LCDDB5_DDR |= (1 << LCDDB5_BIT);
	LCDDB6_DDR |= (1 << LCDDB6_BIT);
	LCDDB7_DDR |= (1 << LCDDB7_BIT);
	LCDRW_DDR |= (1 << LCDRW_BIT);
	LCDA_DDR |= (1 << LCDA_BIT);

	// Ports auf Low setzen
	LCDRS &= ~(1 << LCDRS_BIT);
	LCDR &= ~(1 << LCDR_BIT);
	LCDDB4 &= ~(1 << LCDDB4_BIT);
	LCDDB5 &= ~(1 << LCDDB5_BIT);
	LCDDB6 &= ~(1 << LCDDB6_BIT);
	LCDDB7 &= ~(1 << LCDDB7_BIT);
	LCDRW &= ~(1 << LCDRW_BIT);
	LCDA &= ~(1 << LCDA_BIT);
}



/**
  * @brief Schaltet das LCD ein
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetOn(void) {
	uc_LCDDisplay_g = 0x04;

	Lcd_SendCmd(CTRL_LcdOPTIONS + uc_LCDDisplay_g + uc_LCDCursor_g + uc_LCDBlink_g);
}



/**
  * @brief Schaltet das LCD aus
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetOff(void) {
	uc_LCDDisplay_g = 0x00;

	Lcd_SendCmd(CTRL_LcdOPTIONS + uc_LCDDisplay_g + uc_LCDCursor_g + uc_LCDBlink_g);
}



/**
  * @brief Stellt das Blinken des Cursors an oder aus
  *
  * @param e_Option 
  *
  * @return -
  *
  * @example -
  */
void Lcd_ConfigBlink(Lcdoption_t e_Option) {
	if(e_Option == ON) {
		uc_LCDBlink_g = 0x01;
	} else {
		uc_LCDBlink_g = 0x00;
	}

	Lcd_SendCmd(CTRL_LcdOPTIONS + uc_LCDDisplay_g + uc_LCDCursor_g + uc_LCDBlink_g);
}



/**
  * @brief Schaltet den Cursor ein oder aus
  *
  * @param e_Option Status (ein/aus) 
  *
  * @return -
  *
  * @example -
  */
void Lcd_ConfigCursor(Lcdoption_t e_Option) {
	if(e_Option == ON) {
		uc_LCDCursor_g = 0x02;
	} else {
		uc_LCDCursor_g = 0x00;
	}

	Lcd_SendCmd(CTRL_LcdOPTIONS + uc_LCDDisplay_g + uc_LCDCursor_g + uc_LCDBlink_g);
}



/**
  * @brief Schaltet die Hintergrundbeleuchtung ein oder aus
  *
  * @param e_Option Status (ein/aus) 
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetBacklight(Lcdoption_t e_Option) {
	LCDA_DDR |= (1 << LCDA_BIT);
	
	if(e_Option == ON) {
		LCDA |= (1 << LCDA_BIT);
		uc_LCDBacklightOn = 1;
	} else {
		LCDA &= ~(1 << LCDA_BIT);
		uc_LCDBacklightOn = 0;
	}

	Lcd_SetBacklightDimmer(uc_LCDDimmer_g);
}



/**
  * @brief Dimmt die Hintergrundbeleuchtung
  *
  * @param uc_Value  Wert, auf den das LCD gedimmt wird
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetBacklightDimmer(uint8_t uc_Value) {
	LCDA_DDR |= (1 << LCDA_BIT);

	if(uc_LCDBacklightOn == 1) {
		// PWM Mode 1, Kanal A
		TCCR1A |= (1 << COM1A1);
		TCCR1A |= (1 << WGM10);
		// PWM Frequenz CLK/8
		TCCR1B &= ~(1 << CS12);
		TCCR1B |= (1 << CS11);
		TCCR1B &= ~(1 << CS10);
		// PWM Vergleichswert
		OCR1A = uc_Value;
	} else {
		// PWM Mode aus, Kanal aus
		TCCR1A = 0x00;
		// PWM Frequenz aus
		TCCR1B = 0x00;
		// PWM Vergleichswert
		OCR1A = 0x00;
	}
	
	uc_LCDDimmer_g = uc_Value;
}



/**
  * @brief Loescht die Anzeige
  *
  * @return -
  *
  * @example -
  */
void Lcd_Clear(void) {
	Lcd_SendCmd(0x01);
	WaitMs(2);
}



/**
  * @brief Setzt den Cursor an die Startposition
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetHome(void) {
	Lcd_SendCmd(0x02);
	WaitMs(2);
}



/**
  * @brief Setzt den Cursor an eine bestimmte Position
  *
  * @param e_Line	Anzuspringende Zeile  
  * @param e_Pos	Position in Zeile 
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetPos(Lcdline_t e_Line, Lcdposition_t e_Pos) {
	uint8_t uc_Temp = 0;

	switch(e_Line) {
		case Line1: uc_Temp = 0x80; break;
		case Line2: uc_Temp = 0xC0; break;
		default: uc_Temp = 0x80;
	}

	e_Pos &= 0x0F;
	uc_Temp |= e_Pos;
	Lcd_SendCmd(uc_Temp);
}



/**
  * @brief Schreibt ein Zeichen an eine bestimmte Position
  *
  * @param uc_Char  Anzuzeigendes Zeichen
  * @param e_Line	Anzuspringende Zeile  
  * @param e_Pos	Position in Zeile 
  *
  * @return -
  *
  * @example Lcd_PrintChar(0xFF, Line1, Pos6); // 
  */
void Lcd_PrintChar(uint8_t uc_Char, Lcdline_t e_Line, Lcdposition_t e_Pos) {
	// Cursor ausrichten
	Lcd_SetPos(e_Line, e_Pos);
	
	// Byte senden
	Lcd_SendData(uc_Char);
}



/**
  * @brief Gibt einen einzeiligen Text aus
  *
  * @param puc_Text	Anzuzeigender Text (muss mit \0 enden !)
  * @param e_Line	Anzuspringende Zeile (1 oder 2)
  *
  * @return -
  *
  * @example Lcd_PrintLine("Hallo Welt !", Line1);
  */
void Lcd_PrintLine(const uint8_t *puc_Text, Lcdline_t e_Line) {
	uint8_t i = 0;
	
	// Anzeige resetten
	Lcd_SetPos(e_Line, Pos0);
	
	// Text ausgeben
	while((*puc_Text != 0) && (i < 16)) {
		Lcd_SendData(*puc_Text);
		puc_Text++;
		i++;
	}
}



/**
  * @brief Gibt einen zweizeiligen Text aus
  *
  * @param puc_Text	Anzuzeigender Text (muss mit \0 enden !) 
  *
  * @return -
  *
  * @example Lcd_PrintText("Ich bin ein langer Text und passe nicht in eine Zeile");
  */
void Lcd_PrintText(const uint8_t *puc_Text) {
	uint8_t i = 0;

	// Anzeige resetten
	Lcd_Clear();
	Lcd_SetHome();

	// Text ausgeben
	while(*puc_Text != 0) {
		Lcd_SendData(*puc_Text);
		puc_Text++;
		i++;
		
		// Automatischer Zeilensprung
		if(i == 16) {
			Lcd_SetPos(Line2, Pos0);
		} else if(i == 32) {
			Lcd_SetPos(Line1, Pos0);
			i = 0;
		}
	}
}



/**
  * @brief Gibt einen Zahlenwert als Balken aus
  *
  * @param	us_Value	Der auszugebende Zahlenwert
  * @param	us_Maximum	Maximaler erwarteter Pegel (zur Normalisierung)
  * @param	e_Line		Anzuspringende Zeile (1 oder 2)
  *
  * @return -
  *
  * @example Lcd_PrintBar(10, 100, Line1); // 10 von 100
  */
void Lcd_PrintBar(uint16_t us_Value, uint16_t us_Maximum, Lcdline_t e_Line) {
	uint8_t i = 0;
	
	// Eigenes Zeichen auf Position 7 definieren
	//Lcd_SetMyChar(7, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F);
	Lcd_SetMyChar(7, 0b11111, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111);
	
	// Position in Zeile anspringen
	Lcd_SetPos(e_Line, Pos0);
	
	// Wert berechnen
	us_Maximum = us_Maximum / 16;
	us_Value = us_Value / us_Maximum;

	// Ausgabe
	for (i = 0; i < us_Value; i++) {
		Lcd_SendData(0xFF);
	}
	for (i = us_Value; i < 16; i++) {
		Lcd_SendData(0x07);
	}
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
void MSleepTwo(uint16_t t) {
	uint16_t z;
	
	for(z = 0; z < t; z++) {
		_delay_ms(1);
		
		//timeCount = 0;
		//while (timeCount < 36);
	}
}



void Lcd_PrintMan(Lcdline_t e_Line, Lcdposition_t e_Pos) {
	Lcd_SetMyChar(6, 0b00100, 0b01010, 0b00100, 0b11111, 0b00100, 0b00100, 0b01010, 0b10001);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
}



void Lcd_PrintManAnimated(Lcdline_t e_Line, Lcdposition_t e_Pos) {
	Lcd_SetMyChar(6, 0b00100, 0b01010, 0b00100, 0b11111, 0b00100, 0b00100, 0b01010, 0b10001);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(250);
	
	Lcd_SetMyChar(6, 0b00100, 0b01010, 0b10101, 0b01110, 0b00100, 0b00100, 0b11011, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(250);
}



void Lcd_PrintManJump(Lcdline_t e_Line, Lcdposition_t e_Pos) {
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
	Lcd_SetMyChar(6, 0b00000, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
	Lcd_SetMyChar(6, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
	Lcd_SetMyChar(6, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
	Lcd_SetMyChar(6, 0b00000, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00100, 0b01010, 0b00100, 0b01110, 0b00100, 0b01010);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(80);
}



void Lcd_PrintArrow(Lcdline_t e_Line, Lcdposition_t e_Pos) {
	uint8_t dur = 220;

	// arrow leaves the screen at top
	Lcd_SetMyChar(6, 0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00100, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00100, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	// arrow comes back from bottom
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100, 0b01110);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100, 0b01110, 0b10101);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00000, 0b00100, 0b01110, 0b10101, 0b00100);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00000, 0b00100, 0b01110, 0b10101, 0b00100, 0b00100);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00000, 0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
	
	Lcd_SetMyChar(6, 0b00000, 0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00100);
	Lcd_SetPos(e_Line, e_Pos);
	Lcd_SendData(0x06);
	MSleepTwo(dur);
}



/**
  * @brief Definition eigener Zeichen 5x7 Pixel (Ausgabe via 0 bis 7)
  *
  * @param	uc_Memory	Speicherplatz 0 bis 7
  * @param	uc_PxRow1	Bit 0-4 Pixel der ersten Zeile
  * @param	uc_PxRow2	Bit 0-4 Pixel der zweiten Zeile
  * @param	uc_PxRow3	Bit 0-4 Pixel der dritten Zeile
  * @param	uc_PxRow4	Bit 0-4 Pixel der vierten Zeile
  * @param	uc_PxRow5	Bit 0-4 Pixel der fuenften Zeile
  * @param	uc_PxRow6	Bit 0-4 Pixel der sechsten Zeile
  * @param	uc_PxRow7	Bit 0-4 Pixel der siebten Zeile
  * @param	uc_PxRow8	Bit 0-4 Pixel der achten Zeile
  *
  * @return -
  *
  * @example -
  */
void Lcd_SetMyChar(uint8_t uc_Memory,
				   uint8_t uc_PxRow1,
				   uint8_t uc_PxRow2,
				   uint8_t uc_PxRow3,
				   uint8_t uc_PxRow4,
				   uint8_t uc_PxRow5,
				   uint8_t uc_PxRow6,
				   uint8_t uc_PxRow7,
				   uint8_t uc_PxRow8) {
				   
	if(uc_Memory < 8) {
		uc_Memory = uc_Memory << 3;

		// Hoehere 3 Bits loeschen (5x7 Segment)
		uc_PxRow1 &= 0x1F;
		uc_PxRow2 &= 0x1F;
		uc_PxRow3 &= 0x1F;
		uc_PxRow4 &= 0x1F;
		uc_PxRow5 &= 0x1F;
		uc_PxRow6 &= 0x1F;
		uc_PxRow7 &= 0x1F;
		uc_PxRow8 &= 0x1F;

		// Bild eintragen
		Lcd_SendCmd(0x40 + uc_Memory);		// cmd 0x40, Adr=0x00
		Lcd_SendData(uc_PxRow1);
		Lcd_SendCmd(0x41 + uc_Memory);		// cmd 0x40, Adr=0x01
		Lcd_SendData(uc_PxRow2);
		Lcd_SendCmd(0x42 + uc_Memory);		// cmd 0x40, Adr=0x02
		Lcd_SendData(uc_PxRow3);
		Lcd_SendCmd(0x43 + uc_Memory);		// cmd 0x40, Adr=0x03
		Lcd_SendData(uc_PxRow4);
		Lcd_SendCmd(0x44 + uc_Memory);		// cmd 0x40, Adr=0x04
		Lcd_SendData(uc_PxRow5);
		Lcd_SendCmd(0x45 + uc_Memory);		// cmd 0x40, Adr=0x05
		Lcd_SendData(uc_PxRow6);
		Lcd_SendCmd(0x46 + uc_Memory);		// cmd 0x40, Adr=0x06
		Lcd_SendData(uc_PxRow7);
		Lcd_SendCmd(0x47 + uc_Memory);		// cmd 0x40, Adr=0x07
		Lcd_SendData(uc_PxRow8);

		// An Startposition springen
		Lcd_SetHome();
	}
}

#endif