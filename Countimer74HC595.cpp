/*
Countimer74HC595.cpp
*/

#include "Arduino.h"
#include "Countimer74HC595.h"


// constructor
Countimer74HC595::Countimer74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin)
{
	// set attributes
    _numberOfShiftRegisters = numberOfShiftRegisters;
    _clockPin = clockPin;
    _serialDataPin = serialDataPin;
    _latchPin = latchPin;
    
    // define pins as outputs
    pinMode(clockPin, OUTPUT);
    pinMode(serialDataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    
    // set pins low
    digitalWrite(clockPin, LOW);
    digitalWrite(serialDataPin, LOW);
    digitalWrite(latchPin, LOW);
    
    // allocates the specified number of bytes and initializes them to zero
    _digitalValues = (uint8_t *)calloc(numberOfShiftRegisters, sizeof(uint8_t));
    
	//byte dec_digits[] = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000011,0b11111000,0b10000000,0b10011000 };
	
	// numbers without dot
	_dec_digits[0] = 0b11000000;
	_dec_digits[1] = 0b11111001;
	_dec_digits[2] = 0b10100100;
	_dec_digits[3] = 0b10110000;
	_dec_digits[4] = 0b10011001;
	_dec_digits[5] = 0b10010010;
	_dec_digits[6] = 0b10000011;
	_dec_digits[7] = 0b11111000;
	_dec_digits[8] = 0b10000000;
	_dec_digits[9] = 0b10011000;
	
	// numbers with dot
	_dec_digits[10] = 0b01000000;
	_dec_digits[11] = 0b01111001;
	_dec_digits[12] = 0b00100100;
	_dec_digits[13] = 0b00110000;
	_dec_digits[14] = 0b00011001;
	_dec_digits[15] = 0b00010010;
	_dec_digits[16] = 0b00000011;
	_dec_digits[17] = 0b01111000;
	_dec_digits[18] = 0b00000000;
	_dec_digits[19] = 0b00011000;
	
	
	_previousMillis = 0;
	_currentCountTime = 0;
	_countTime = 0;
	_isCounterCompleted = false;
	_isStopped = true;
	_countType = COUNT_NONE;
	_startCountTime = 0;
	
	setAll(_digitalValues); // reset shift register
}

void Countimer74HC595::setCounter(uint8_t hours, uint8_t minutes, uint8_t seconds, CountType countType, uint8_t interval, timer_callback onComplete)
{
	_onComplete = onComplete;
	_countType = countType;
	_interval = interval;	
		
	_countTime = (((hours * 3600) + (minutes * 60) + seconds) * 1000 );
		
	switch(_countType){
		case COUNT_UP:
			_currentCountTime = 0;
			break;
		case COUNT_DOWN;
			_currentCountTime = _countTime;
			break;
		default;
			break;
	}
		
	_startCountTime = _currentCountTime;
	
}

void Countimer74HC595::run()
{
	// timer is running only if is not completed or not stopped.
	if (_isCounterCompleted || _isStopped)
		return;

	if (millis() - _previousMillis >= _interval) {

		if (_countType == COUNT_DOWN)
		{
			countDown();
		}
		else if (_countType == COUNT_UP)
		{
			countUp();
		}
		else
		{
			_callback();
		}
		_previousMillis = millis();
	}
}




void Countimer74HC595::setAll(uint8_t * digitalValues) {
    int x;
    for (x = 0 - 1; x < _numberOfShiftRegisters ; x++) {
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, digitalValues[x]);
    }
    _digitalValues = digitalValues; 
    
    digitalWrite(_latchPin, HIGH);
    digitalWrite(_latchPin, LOW);

}

void Countimer74HC595::setAllHigh() {
    int x; 
    for (x = 0; x < _numberOfShiftRegisters; x++)
        _digitalValues[x] = 255;
    setAll(_digitalValues); 
}


void Countimer74HC595::setAllLow() {
    int x; 
    for (x = 0; x < _numberOfShiftRegisters; x++)
        _digitalValues[x] = 0; 
    setAll(_digitalValues); 
}