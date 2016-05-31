/*
Countimer74HC595.h
*/
#ifndef Countimer74HC595_h
#define Countimer74HC595_h

#include "Arduino.h"

typedef void(*timer_callback)(void);
static const byte _dec_digits[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000};
static const byte _dec_digits_dot[] = {0b01000000,0b01111001,0b00100100,0b00110000,0b00011001,0b00010010,0b00000011,0b01111000,0b00000000,0b00011000};

/*
static const byte _dec_digits_CC[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000};
static const byte _dec_digits_dot_CC[] = {0b01000000,0b01111001,0b00100100,0b00110000,0b00011001,0b00010010,0b00000011,0b01111000,0b00000000,0b00011000};
*/


class Countimer74HC595 
{
    public:
	
		Countimer74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
	
		enum CountType
		{
			COUNT_NONE = 0,
			COUNT_UP = 1,
			COUNT_DOWN = 2
		};
		
		char _formatted_time[9];
		
		void setCounter(int hours, int minutes, int seconds, CountType countType, int interval, String format, timer_callback onComplete);
		
		void run();
		void start();
		void stop();
		void restart();
		void pause();
		
		long getCurrentHours();
		long getCurrentMinutes();
		long getCurrentSeconds();
		
		// Returns current timer as formatted string HH:MM:SS
		char* getCurrentTime();
		
        void setAll(uint8_t * digitalValues);
        void setAllLow();
        void setAllHigh(); 

		
    private:
	
		int _cthour;
		int _ctminute;
		int _ctsecond;
		
		unsigned long hour;
		unsigned long minute;
		unsigned long second;
	
	
	
		int * _arrayMinute;
		int * _arrayHour;
		int * _arraySecond;
	
		int _numberOfShiftRegisters;
        int _clockPin;
        int _serialDataPin;
        int _latchPin;
		
		String _format;
		uint8_t * _toPrint;
	
		uint8_t * _digitalValues;
		uint32_t _interval;
		unsigned long _previousMillis;
	
		void countDown();
		void countUp();
		void sendSR();

		long _currentCountTime;
		unsigned long _startCountTime;
		unsigned long _countTime;

		timer_callback _onComplete;
		
		bool _isCounterCompleted;
		bool _isStopped;
		CountType _countType;

};
#endif