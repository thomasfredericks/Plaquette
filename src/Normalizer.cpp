/*
 * Normalizer.cpp
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

#include "MovingStats.h"
#include "Normalizer.h"

namespace pq {

Normalizer::Normalizer()
: MovingFilter(),
  MovingStats(),
  _targetMean(NORMALIZER_DEFAULT_MEAN),
  _targetStdDev(NORMALIZER_DEFAULT_STDDEV)
{
  _value = NORMALIZER_DEFAULT_MEAN;
}

Normalizer::Normalizer(float timeWindow)
: MovingFilter(timeWindow),
  MovingStats(timeWindow),
  _targetMean(NORMALIZER_DEFAULT_MEAN),
  _targetStdDev(NORMALIZER_DEFAULT_STDDEV)
{
}

Normalizer::Normalizer(float mean, float stddev)
	: MovingFilter(),
    MovingStats(),
    _targetMean(mean),
    _targetStdDev(abs(stddev))
{
}

Normalizer::Normalizer(float mean, float stddev, float timeWindow)
	: MovingFilter(timeWindow),
    MovingStats(timeWindow),
    _targetMean(mean),
    _targetStdDev(abs(stddev))
{
}

void Normalizer::infiniteTimeWindow() {
  MovingStats::infiniteTimeWindow();
}

void Normalizer::timeWindow(float seconds) {
  MovingStats::timeWindow(seconds);
}

float Normalizer::timeWindow() const { return MovingStats::timeWindow(); }

float Normalizer::put(float value) {
  _value = isStarted() ? MovingStats::update(value, sampleRate()) : normalize(value);
  _value = _value * max(_targetStdDev, FLT_MIN) + _targetMean;
  return _value;
}

}
