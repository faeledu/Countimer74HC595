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
    _digitalValues = (uint8_t *)calloc(_numberOfShiftRegisters, sizeof(uint8_t));
	_previousMillis = 0;
	_currentCountTime = 0;
	_countTime = 0;
	_isCounterCompleted = false;
	_isStopped = true;
	_countType = COUNT_NONE;
	_startCountTime = 0;
	
	setAll(_digitalValues); // reset shift register
}

void Countimer74HC595::setCounter(int hours, int minutes, int seconds, CountType countType, int interval, String format, timer_callback onComplete)
{
	_onComplete = onComplete;
	_countType = countType;
	_interval = interval;
	_format = format;
	
	_cthour = 0;
	_ctminute = 0;
	_ctsecond = 1;
	
	
	if (hours > 200){
		hours = 200;
	}
		if (minutes > 10000){
		minutes = 10000;
	}
		if (seconds > 10000){
		seconds = 10000;
	}

	_countTime = (((hours * 3600L) + (minutes * 60L) + seconds) * 1000L);
	
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
		
	for (int i = 0; i < _format.length(); i++){
		switch (_format[i]){
			case 'H':
				_cthour++ ;
				break;
			case 'h':
				_cthour++ ;
				break;			
			case 'M':
				_ctminute++ ;
				break;
			case 'm':
				_ctminute++ ;
				break;
			default:
				break;
		}
	}
	
	
	_arrayHour = (int *)calloc(_cthour-1, sizeof(int));
	_arrayMinute = (int *)calloc(_ctminute-1, sizeof(int));
	_arraySecond = (int *)calloc(_ctsecond, sizeof(int));
	
	sendSR();
	
}

void Countimer74HC595::run()
{
	// timer is running only if is not completed or not stopped.
	if (_isCounterCompleted || _isStopped){
		return;
	}

	if ( millis() - _previousMillis >= _interval ) {

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


long Countimer74HC595::getCurrentHours()
{
	return _currentCountTime / 1000 / 3600;
}

long Countimer74HC595::getCurrentMinutes()
{
	if (_ctminute < 3){
		return _currentCountTime / 1000 % 3600 / 60;
	} else {
		return _currentCountTime / 1000 / 60;
	}
}

long Countimer74HC595::getCurrentSeconds()
{
	return _currentCountTime / 1000 % 60;
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
		sendSR();
		_currentCountTime += _interval;
	}
	else
	{
		sendSR();
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
		sendSR();
		_currentCountTime -= _interval;	
	}
	else
	{
		sendSR();
		stop();
		if (_onComplete != NULL){
			_onComplete();
		}
	}
}
void Countimer74HC595::sendSR(){
	
	int cthour = _cthour - 1;
	int ctminute = _ctminute - 1;
	int ctsecond = _ctsecond;
	int i;
	int number;

		
	hour = getCurrentHours();
	number = hour;
		
	for (i = cthour; i >= 0; i--) {
		_arrayHour[i] = number % 10;
		number /= 10;
	}
	
	// minute	
	minute = getCurrentMinutes();
	number = minute;
	
	for (i = ctminute; i >= 0; i--) {
		_arrayMinute[i] = number % 10;
		number /= 10;
	}
	
	// second
	second = getCurrentSeconds();
	number = second;
	
	for (i = ctsecond; i >= 0; i--) {
		_arraySecond[i] = number % 10;
		number /= 10;
	}
	
	/* DEBUG
	
	Serial.print("_currentCountTime: ");
	Serial.println(_currentCountTime);

	*/	
	
	for (i = (_format.length() -1); i >= 0 ; i--){
	
		switch (_format[i]){
		
			// Hours
			case 'H':
				if ( cthour >= 0 ){
					_digitalValues[i] = _dec_digits_dot[_arrayHour[cthour]];
					cthour-- ;
				} else {
					_digitalValues[i] = _dec_digits_dot[0];
				}
				break;
			case 'h':
				if ( cthour >= 0 ){
					_digitalValues[i] = _dec_digits[_arrayHour[cthour]];
					cthour-- ;
				} else {
					_digitalValues[i] = _dec_digits[0];
				}
				break;


			
			
			// Minutes
			case 'M':
				if ( ctminute >= 0 ){
					_digitalValues[i] = _dec_digits_dot[_arrayMinute[ctminute]];
					ctminute-- ;
				} else {
					_digitalValues[i] = _dec_digits_dot[0];
				}
				break;
			case 'm':
				if ( ctminute >= 0 ){
					_digitalValues[i] = _dec_digits[_arrayMinute[ctminute]];
					ctminute-- ;
				} else {
					_digitalValues[i] = _dec_digits[0];
				}
				break;
				
			// seconds	
			case 'S':
				if ( ctsecond >= 0){
					_digitalValues[i] = _dec_digits_dot[_arraySecond[ctsecond]];
					ctsecond-- ;
				} else {
					_digitalValues[i] = _dec_digits_dot[0];
				}
				break;
			case 's':
				if ( ctsecond >= 0){
					_digitalValues[i] = _dec_digits[_arraySecond[ctsecond]];
					ctsecond-- ;
				} else {
					_digitalValues[i] = _dec_digits[0];
				}
				break;
			default:
				_digitalValues[i] = _dec_digits[0];
				break;
		}

	}

	setAll(_digitalValues);

}

void Countimer74HC595::setAll(uint8_t * digitalValues) {
    int byte;
	//for (byte = 0 ; byte < _numberOfShiftRegisters; byte++){
    for (byte = (_numberOfShiftRegisters - 1); byte >= 0; byte--) {
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, digitalValues[byte]);
	}
    _digitalValues = digitalValues; 
    digitalWrite(_latchPin, HIGH); 
    digitalWrite(_latchPin, LOW); 
}

void Countimer74HC595::setAllHigh() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++){
		_digitalValues[i] = 255;
	}
    setAll(_digitalValues); 
}


void Countimer74HC595::setAllLow() {
    int i; 
    for (i = 0; i < _numberOfShiftRegisters; i++){
        _digitalValues[i] = 0; 
	}
    setAll(_digitalValues); 
}