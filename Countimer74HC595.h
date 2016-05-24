/*
Countimer74HC595.h
*/
#ifndef Countimer74HC595_h
#define Countimer74HC595_h

#include "Arduino.h"

typedef void(*timer_callback)(void);
static const byte _dec_digits[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000 };
static const byte _dec_digits_dot[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000 };

/*
static const byte _dec_digits_CC[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000 };
static const byte _dec_digits_dotCC[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000 };
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
		
		void setCounter(uint8_t hours, uint8_t minutes, uint8_t seconds, CountType countType, uint8_t interval, timer_callback onComplete);
		
		void run();
		void start();
		void stop();
		void restart();
		void pause();
		
		// Returns timer's current hours.
		uint8_t getCurrentHours();

		// Returns timer's current minutes.
		uint8_t getCurrentMinutes();

		// Returns timer's current seconds.
		uint8_t getCurrentSeconds();
		
			// Returns current timer as formatted string HH:MM:SS
		char* getCurrentTime();

		// Returns true if counter is completed, otherwise returns false.
		bool isCounterCompleted();

		// Returns true if counter is still running, otherwise returns false.
		bool isCounterRunning();

		// Returns true if timer is stopped, otherwise returns false.
		bool isStopped();
		
		
        void setAll(uint8_t * digitalValues);
        void setAllLow();
        void setAllHigh(); 


		
		
    private:
	
		// Counting up timer.
		void countDown();

		// Counting down timer.
		void countUp();
		
		void printAll();
        int _numberOfShiftRegisters;
        int _clockPin;
        int _serialDataPin;
        int _latchPin;
		
		
		uint8_t * _digitalValues;
		
		//uint8_t * _dec_digits;
		

		
		
		uint32_t _interval;
		unsigned long _previousMillis;

		// Stores current counter value in milliseconds.
		unsigned long _currentCountTime;
		unsigned long _startCountTime;

		// Stores cached user's time.
		unsigned long _countTime;

		timer_callback _onComplete;
		bool _isCounterCompleted;
		bool _isStopped;
		CountType _countType;

		
		
};

#endif