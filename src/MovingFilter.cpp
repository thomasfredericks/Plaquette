/*
 * MovingFilter.cpp
 *
 * (c) 2022 Sofian Audry        :: info(@)sofianaudry(.)com
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

#include "MovingFilter.h"

namespace pq {

MovingFilter::MovingFilter()
  : AnalogSource(),
    _isAdaptive(true)
{
  infiniteTimeWindow();
}

MovingFilter::MovingFilter(float timeWindow_)
  : AnalogSource(),
    _isAdaptive(true)
{
  timeWindow(timeWindow_);
}

void MovingFilter::cutoff(float hz) {
  if (hz <= 0)
    infiniteTimeWindow();
  else
    timeWindow(1.0f/hz);
}

float MovingFilter::cutoff() const {
  return (_timeWindow == MOVING_FILTER_INFINITE_TIME_WINDOW ? 0 : 1.0f/_timeWindow);
}

void MovingFilter::reset() {
}

void MovingFilter::start() {
  _isAdaptive = true;
}

void MovingFilter::stop() {
  _isAdaptive = false;
}

bool MovingFilter::isStarted() const {
  return _isAdaptive;
}

}
