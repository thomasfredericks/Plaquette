/*
 * pq_map_real.cpp
 *
 * Equivalent of Arduino map() method but for floats and doubles.
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

#include "pq_map_real.h"

float mapFloat(double value, double fromLow, double fromHigh, double toLow, double toHigh)
{
 return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

float mapFrom01(double value, double toLow, double toHigh) {
  return (value * (toHigh - toLow)) + toLow;
}
float mapTo01(double value, double fromLow, double fromHigh) {
  return (value - fromLow) / (fromHigh - fromLow);
}
