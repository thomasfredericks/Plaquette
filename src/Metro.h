/*
 * Metro.h
 *
 * (c) 2018 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2018 Thomas O Fredericks :: tof(@)t-o-f(.)info
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

#ifndef METRO_H_
#define METRO_H_

#include "PqCore.h"
#include "pq_osc_utils.h"

namespace pq {

/**
 * Chronometer digital unit which emits 1/true/"on" for one frame, at a regular pace.
 */
class Metro : public DigitalSource {
public:
  /**
   * Constructor.
   * @param period the period of oscillation (in seconds)
   */
  Metro(float period=1.0f);

  /**
   * Sets the period (in seconds).
   * @param period the period of oscillation (in seconds)
   * @return the unit itself
   */
  virtual Metro& period(float period);
  virtual float period() const { return _period; }

  /**
   * Sets the frequency (in Hz).
   * @param frequency the frequency of oscillation (in Hz)
   * @return the unit itself
   */
  virtual Metro& frequency(float frequency);
  virtual float frequency() const { return (1/_period); }

  /**
   * Sets the phase (ie. the offset, in % of period).
   * @param phase the phase (in % of period)
   * @return the unit itself
   */
  virtual Metro& phase(float phase);
  virtual float phase() const { return _phase; }

protected:
  virtual void begin();
  virtual void step();

  // Phase (in % of period).
  float _period;

  // Phase (in % of period).
  float _phase;

  // Internal use.
  phase_time_t _phaseTime;
};

}

#endif
