/*
 * SineWave.h
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

#ifndef SINE_WAVE_H_
#define SINE_WAVE_H_

#include "AbstractWave.h"

namespace pq {

/// Sine oscillator. Phase is expressed as % of period.
class SineWave : public AbstractWave {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  SineWave(float period=1.0f, float width=0.5f);
  virtual ~SineWave() {}

  /**
   * Sets the width (also called duty-cycle) (ie. the proportion of time during which the signal is on).
   * @param width the width as a value in [0, 1]
   */
  virtual void width(float width);

  /// Returns the width (also called duty-cycle) (as a value in [0, 1]).
  virtual float width() const { return phaseTime2float(_width); }

protected:
  // Returns value in [0, 1].
  virtual float _get(phase_time_t t);

  // Tipping-point (in % of period).
  phase_time_t _width;
};

[[deprecated("Use SineWave instead.")]]
typedef SineWave SineOsc;

}

#endif
