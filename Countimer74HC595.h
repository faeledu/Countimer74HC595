/*
Countimer74HC595.h
*/

#include "Arduino.h"

typedef void(*timer_callback)(void);

class Countimer74HC595 
{
    public:
	
        Countimer74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
		setCounter(uint8_t hours, uint8_t minutes, uint8_t seconds, CountType countType, uint8_t interval, timer_callback onComplete);
		
        void setAll(uint8_t * digitalValues);
        void setAllLow();
        void setAllHigh(); 

		enum CountType
		{
			COUNT_NONE = 0,
			COUNT_UP = 1,
			COUNT_DOWN = 2
		}
		
		
    private:
		
        int _numberOfShiftRegisters;
        int _clockPin;
        int _serialDataPin;
        int _latchPin;
		
		timer_callback _onComplete;
		
        uint8_t * _digitalValues;
		uint8_t * _dec_digits;
		
		
};