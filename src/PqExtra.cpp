/*
 * PqExtra.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqExtra.h"

StreamIn::StreamIn(Stream& stream) : _value(0), _nextValue(0), _nextFraction(1), _nextIsValid(false), _nextIsNegative(false), _nextIsFraction(false), _stream(&stream) {}

void StreamIn::setup() {
	_nextValue = 0;
	_nextIsValid = false;
	_nextIsNegative = false;
	_nextIsFraction = false;
	_nextFraction = 1;
}

void StreamIn::update() {
	while (_stream->available()) {

		int c = _stream->read();
		if (c == '-') {
			_nextIsNegative = true;
		}
		else if (c == '.') {
			_nextIsFraction = true;
		}
		else if (c >= '0' && c <= '9') {
			_nextIsValid = true;
			_nextValue = _nextValue * 10 + (c - '0');
			if (_nextIsFraction)
				_nextFraction *= 0.1;
		}
		else {
			// Newline or carriage return: save value.
			if (c == '\n' || c == '\r') {
				// Save if valid.
				if (_nextIsValid) {
					if (_nextIsNegative)
						_nextValue = -_nextValue;
				  if (_nextIsFraction)
						_nextValue *= _nextFraction;
					_value = _nextValue;
				}
			}

			// Reset everything.
			_nextValue = 0;
			_nextIsValid = false;
			_nextIsNegative = false;
			_nextIsFraction = false;
			_nextFraction = 1;
		}
	}
}

SquareOsc::SquareOsc(float period_, float dutyCycle_) {
  period(period_);
  dutyCycle(dutyCycle_);
}

void SquareOsc::setup() {
	_startTime = seconds();
}

void SquareOsc::update() {
	// Notice: this computation is not exact but manages naturally changes in the period without
	// inducing dephasings on Arduino boards.
	float totalTime = seconds();
	float progress = (totalTime - _startTime) / _period;
	_isOn = (progress < _dutyCycle);
	if (progress >= 1) // reset
  	_startTime = totalTime;
}

SquareOsc& SquareOsc::period(float period) {
	_period = max(period, 1e-6);
	return *this;
}

SquareOsc& SquareOsc::dutyCycle(float dutyCycle) {
  _dutyCycle = constrain(dutyCycle, 0, 1);
	return *this;
}

SineOsc::SineOsc(float period_) : _value(0.5f), _phase(0) {
  period(period_);
}

void SineOsc::setup() {
  _startTime = seconds();
  _update(0);
}

void SineOsc::update() {
	// Notice: this computation is not exact but manages naturally changes in the period without
	// inducing dephasings on Arduino boards.
	float totalTime = seconds();
	float relativeTime = totalTime - _startTime;
	_update(relativeTime);
	if (relativeTime >= _period)
  	_startTime = totalTime;
}

void SineOsc::_update(float t) {
  _value = (sin( (_phase + (t / _period)) * TWO_PI) + 1) / 2;
}

SineOsc& SineOsc::period(float period) {
	_period = max(period, 1e-6);
	return *this;
}

SineOsc& SineOsc::phase(float phase) {
  _phase = phase;
	return *this;
}

TriOsc::TriOsc(float period_, float width_) {
  period(period_);
  width(width_);
}

void TriOsc::setup() {
  _startTime = seconds();
}

void TriOsc::update() {
	// Notice: this computation is not exact but manages naturally changes in the period without
	// inducing dephasings on Arduino boards.
	float totalTime = seconds();
	float relativeTime = totalTime - _startTime;

  // Check where we are.
	float progress = relativeTime / _period;
	if (progress >= 1) {
		_value = 0;
		_startTime = totalTime;
	}
	else if (progress >= _width) _value = (1 - progress) / (1 - _width);
	else                         _value = progress / _width;
}

TriOsc& TriOsc::period(float period) {
	_period = max(period, 1e-6);
	return *this;
}

TriOsc& TriOsc::width(float width) {
  _width = constrain(width, 0, 1);
	return *this;
}


Tween::Tween(float duration_) :
	_value(0), _duration(duration_),
	_from(0), _change(1),
	_startTime(0), _offsetTime(0),
	_isRunning(false)
{
	stop();
}

Tween& Tween::duration(float duration) {
	stop();
	_duration = duration;
	return *this;
}

Tween& Tween::fromTo(float from, float to) {
	stop();
	_from = from;
	_change = to - from;
	return *this;
}

Tween& Tween::to(float to) {
	stop();
	_from = _value;
	_change = to - _from;
	return *this;
}

void Tween::start() {
	_startTime = seconds();
	_offsetTime = 0;
	_isRunning = true;
}

void Tween::pause() {
	if (_isRunning) {
		_offsetTime = elapsed();
		_isRunning = false;
	}
}

void Tween::resume() {
	if (!_isRunning) {
		_startTime = seconds();
		_isRunning = true;
	}
}

void Tween::stop() {
	_startTime = _offsetTime = 0;
	_isRunning = false;
}

float Tween::progress() const {
	float prog = elapsed() / _duration;
	prog = constrain(prog, 0, 1);
	return prog;
}

void Tween::setup() {
	stop();
}

void Tween::update() {
	// Compute current elapsed time.
	_elapsedTime = _offsetTime + (_isRunning ? (seconds() - _startTime) : 0);

	// Compute value if running -- otherwise leave as is.
	if (_isRunning)
		_value = map(progress(), 0.0f, 1.0f, _from, (_from + _change));
}

StreamOut::StreamOut(Stream& stream) : _value(0), _digits(4), _stream(&stream) {}

float StreamOut::put(float value) {
  // Copy value.
  _value = value;

  // Print it.
  _stream->println(_value, _digits);

  // Return it.
  return _value;
}

void StreamOut::precision(uint8_t digits) {
	_digits = digits;
}


OscilloscopeOut::OscilloscopeOut(float minValue, float maxValue, uint8_t precision)
  : _value(0), _minValue(minValue), _maxValue(maxValue), _precision(precision) {}

float OscilloscopeOut::put(float value) {
  // Copy value.
  _value = value;

  // Convert to bin.
  float mapped = mapTo01(_value, _minValue, _maxValue);
  int bin = round( mapped * _precision );
  bin = constrain(bin, 0, _precision-1);

  // Print.
  print(_minValue, 2);
  print(" |");
  for (int i=0; i<_precision; i++)
    print(i == bin ? '*' : ' ');
  print("| ");
  print(_maxValue, 2);
  println();

  // Return it.
  return _value;
}

Smoother::Smoother(float factor)
  : PqPutter(),
    MovingAverage(factor) {
}

float Smoother::put(float value) {
  return MovingAverage::update(value);
}

AdaptiveNormalizer::AdaptiveNormalizer(float smoothFactor)
  : PqPutter(),
    MovingStats(smoothFactor),
    _value(0.5f),
    _mean(0.5f),
    _stddev(0.25f)
{}

AdaptiveNormalizer::AdaptiveNormalizer(float mean, float stddev, float smoothFactor)
	: PqPutter(),
    MovingStats(smoothFactor),
    _value(mean),
    _mean(mean),
    _stddev(abs(stddev))
{}

float AdaptiveNormalizer::put(float value) {
  return (_value = MovingStats::update(value) * _stddev + _mean);
}

Normalizer::Normalizer()
  : PqPutter(),
    SimpleStats(),
    _value(0.5f),
    _mean(0.5f),
    _stddev(0.25f)
{}

Normalizer::Normalizer(float mean, float stddev)
	: PqPutter(),
    SimpleStats(),
    _value(mean),
    _mean(mean),
    _stddev(abs(stddev))
{}

float Normalizer::put(float value) {
  return (_value = SimpleStats::update(value) * _stddev + _mean);
}

MinMaxScaler::MinMaxScaler()
 : PqPutter(),
   _value(0.5f),
   _minValue(FLT_MAX),
   _maxValue(FLT_MIN)
{}

float MinMaxScaler::put(float value)
{
  _minValue = min(value, _minValue);
  _maxValue = max(value, _maxValue);
  _value = (_minValue == _maxValue ? 0.5f : mapTo01(value, _minValue, _maxValue));
	return _value;
}

Thresholder::Thresholder(float threshold, uint8_t mode)
  : PqPutter() {
			_init(threshold, mode, threshold);
		}

Thresholder::Thresholder(float threshold, uint8_t mode, float resetThreshold)
	: PqPutter() {
			_init(threshold, mode, resetThreshold);
		}

float Thresholder::put(float value) {
  bool high = (value > _threshold);
  bool low  = (value < _threshold);
  bool rising = (high && _wasLow);
  bool falling = (low  && _wasHigh);

	// Reset.
	if (rising) _wasLow  = false;
	if (falling) _wasHigh = false;
  // bool raising = (high && _prev != (+1));
  // bool falling = (low  && _prev != (-1));
  switch (_mode) {
    case THRESHOLD_HIGH:    _value = high;    break;
    case THRESHOLD_LOW:     _value = low;     break;
    case THRESHOLD_RISING:  _value = rising; break;
    case THRESHOLD_FALLING: _value = falling; break;
    case THRESHOLD_CHANGE:
    default:                _value = rising || falling;
  }

	if (value < _resetThreshold)      _wasLow = true;
	else if (value > _resetThreshold) _wasHigh = true;

//  _prev = (value < _threshold2 ? (-1) : (value > _threshold2 ? (+1) : (0)));
  return get();
}

void Thresholder::_init(float threshold, uint8_t mode, float resetThreshold) {
	_threshold = threshold;
	_mode = mode;
	// Set resetThreshold, with correction.
	if (mode == THRESHOLD_RISING)
		_resetThreshold = min(resetThreshold, threshold);
	else if (mode == THRESHOLD_FALLING)
		_resetThreshold = max(resetThreshold, threshold);
	else
		_resetThreshold = threshold;
	_value = _wasLow = _wasHigh = false;
}
