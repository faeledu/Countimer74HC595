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
	
	//_formatted_time = formatted_time;
		
	_countTime = (((hours * 3600) + (minutes * 60) + seconds) * 1000 );
		
	switch(_countType){
		case COUNT_UP:
			_currentCountTime = 0;
			break;
		case COUNT_DOWN:
			_currentCountTime = _countTime;
			break;
		default:
			break;
	}
		
	_startCountTime = _currentCountTime;
	
}

void Countimer74HC595::run()
{
	// timer is running only if is not completed or not stopped.
	if (_isCounterCompleted || _isStopped){
		return;
	}

	if (millis() - _previousMillis >= _interval) {

		switch(_countType){
			case COUNT_UP:
				countUp();
				break;
			case COUNT_DOWN:
				countDown();
				break;
			default:
				break;		
			
		}
		_previousMillis = millis();
	}
}


void Countimer74HC595::start()
{
	_isStopped = false;
	if(_isCounterCompleted){
		_isCounterCompleted = false;
	}
}

void Countimer74HC595::stop()
{
	_isStopped = true;
	_isCounterCompleted = true;
	_currentCountTime = _countTime;

	if(_countType == COUNT_UP)
	{
		_currentCountTime = 0;		
	}
}

void Countimer74HC595::pause()
{
	_isStopped = true;
}

void Countimer74HC595::restart()
{
	_currentCountTime = _startCountTime;
	_isCounterCompleted = false;
	_isStopped = false;

	start();
}


uint8_t Countimer74HC595::getCurrentHours()
{
	return _currentCountTime / 1000 / 3600;
}

uint8_t Countimer74HC595::getCurrentMinutes()
{
	return _currentCountTime / 1000 % 3600 / 60;
}

uint8_t Countimer74HC595::getCurrentSeconds()
{
	return _currentCountTime / 1000 % 3600 % 60 % 60;
}

char* Countimer74HC595::getCurrentTime()
{
	sprintf(_formatted_time, "%02d:%02d:%02d", getCurrentHours(), getCurrentMinutes(), getCurrentSeconds());
	return _formatted_time;
}





void Countimer74HC595::countUp()
{
	if (_currentCountTime < _countTime)
	{
		_currentCountTime += _interval;
	}
	else
	{
		stop();
		if (_onComplete != NULL){
			_onComplete();
		}
	}
}


void Countimer74HC595::countDown()
{
	if (_currentCountTime > 0)
	{
		for (int byte = 0 ; byte < _numberOfShiftRegisters; byte++){
			shiftOut(_serialDataPin, _clockPin, MSBFIRST, _dec_digits[5]);
		}
		    digitalWrite(_latchPin, HIGH); 
			digitalWrite(_latchPin, LOW); 
			
		_currentCountTime -= _interval;
		
	}
	else
	{
		stop();
		if (_onComplete != NULL){
			_onComplete();
		}
	}
}
void printAll(){
	
	



}


void Countimer74HC595::setAll(uint8_t * digitalValues) {
    int byte;
    
    // go through all bytes (most significant byte first)
    for (byte = 0 ; byte < _numberOfShiftRegisters; byte++)
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, digitalValues[byte]);
    
    _digitalValues = digitalValues; 
    
    digitalWrite(_latchPin, HIGH); 
    digitalWrite(_latchPin, LOW); 
}

void Countimer74HC595::setAllHigh() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++)
        _digitalValues[i] = 255;
    setAll(_digitalValues); 
}


void Countimer74HC595::setAllLow() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++)
        _digitalValues[i] = 0; 
    setAll(_digitalValues); 
}