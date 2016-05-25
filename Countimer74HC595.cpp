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

void Countimer74HC595::setCounter(int hours, int minutes, int seconds, CountType countType, int interval, String format, timer_callback onComplete)
{
	_onComplete = onComplete;
	_countType = countType;
	_interval = interval;
	_format = format;

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


long Countimer74HC595::getCurrentHours()
{
	return _currentCountTime / 1000 / 3600;
}

long Countimer74HC595::getCurrentMinutes()
{
	return _currentCountTime / 1000 / 60;
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
		stop();
		if (_onComplete != NULL){
			_onComplete();
		}
	}
}
void Countimer74HC595::sendSR(){
	
	uint8_t * digitalValues;
	
	int cthour = 0;
	int ctminute = 0;
	int ctsecond = 1;
			
	unsigned long hour;
	unsigned long minute;
	unsigned long second;

	for (int i = 0; i < _format.length() ; i++){
		switch (_format[i]){
			case 'H':
				cthour++ ;
				break;
			case 'h':
				cthour++ ;
				break;			
			case 'M':
				ctminute++ ;
				break;
			case 'm':
				ctminute++ ;
				break;
			default:
				break;
		}
	}
	
	hour = getCurrentHours();
	
	int arrayHour[sizeof(hour)];
	int number = hour;
	for (int i = sizeof(hour); i >= 0; i--) {
		arrayHour[i] = number % 10;
		//Serial.print("arrayHour: ");
		//Serial.println(arrayHour[i]);
		number /= 10;
	}
	

	
		
	
	minute = getCurrentMinutes();
	
	int arrayMinute[sizeof(minute)];
	number = minute;
	for (int i = sizeof(minute); i >= 0; i--) {
		arrayMinute[i] = number % 10;
		//Serial.print("arrayMinute: ");
		//Serial.println(arrayMinute[i]);
		number /= 10;
	}
	
	
	
	second = getCurrentSeconds();
		
	int arraySecond[sizeof(second)];
	number = second;
	for (int i = sizeof(second); i >= 0; i--) {
		arraySecond[i] = number % 10;
		//Serial.print("arraySecond: ");
		//Serial.println(arraySecond[i]);
		number /= 10;
	}
	
	
	/* DEBUG
	Serial.print("_currentCountTime: ");
	Serial.println(_currentCountTime);
	
	Serial.print("hour: ");
	Serial.println(hour);
	
	Serial.print("minute :");
	Serial.println(minute);
	
	Serial.print("second :");
	Serial.println(second);
	*/
	
	
	

	
	for (int i = _format.length(); i > 0 ; i--){
		
		switch (_format[i]){
		/*
			case 'H':
				if ( cthour > hour.length()){
					digitalValues[i] = _dec_digits_dot[(hour[cthour])];
					cthour-- ;
				}
				break;
			case 'h':
				if ( cthour > hour.length()){
					digitalValues[i] = _dec_digits[(hour[cthour])];
					cthour-- ;
				}
				break;	


				
			case 'M':
			
				if ( ctminute > minute.length() ){
					digitalValues[i] = _dec_digits_dot[(minute[ctminute])];
					ctminute-- ;
				}
				break;
			case 'm':
				if ( ctminute > minute.length()){
					digitalValues[i] = _dec_digits[(minute[ctminute])];
					ctminute-- ;
				}
				break;
				
			
*/
			
			// seconds	
			case 'S':
				if ( ctsecond >= 0){
					digitalValues[i] = _dec_digits_dot[arraySecond[ctsecond]];
					ctsecond-- ;
				} else {
					digitalValues[i] = _dec_digits_dot[0];
				}
				break;
			case 's':
				if ( ctsecond >= 0){
					digitalValues[i] = _dec_digits[arraySecond[ctsecond]];
					ctsecond-- ;
				} else {
					digitalValues[i] = _dec_digits[0];
				}
				break;
			default:
				break;

		}

	}
	
	setAll(digitalValues);

}

/*
int *
*/

void Countimer74HC595::setAll(uint8_t * digitalValues) {
    int byte;
    for (byte = 0 ; byte < _numberOfShiftRegisters; byte++)
        shiftOut(_serialDataPin, _clockPin, MSBFIRST, digitalValues[byte]);
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