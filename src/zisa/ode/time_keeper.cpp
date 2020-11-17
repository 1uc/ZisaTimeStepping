/* Keep track of how much simulation time passed.
 *
 * Authors: Luc Grosheintz <forbugrep@zoho.com>
 *    Date: 2016-03-08
 */
#include <zisa/math/basic_functions.hpp>
#include <zisa/math/comparison.hpp>
#include <zisa/ode/time_keeper.hpp>

namespace zisa {

// ------------------------------------------------------------
// TimeKeeper
TimeKeeper::TimeKeeper() : needs_small_steps(false) {}

bool TimeKeeper::is_interrupted(const SimulationClockData &) const {
  return false;
}

double TimeKeeper::pick_time_step(const SimulationClockData &clock_data,
                                  double dt_cfl) const {
  if (needs_small_steps) {
    dt_cfl = enforce_initially_small_time_steps(clock_data, dt_cfl);
  }

  if (dt_cfl <= 0) {
    LOG_ERR(string_format("Non-positive `dt_cfl`. [%e]", dt_cfl));
  }

  return dt_cfl;
}

void TimeKeeper::activate_small_steps() { needs_small_steps = true; }

double TimeKeeper::enforce_initially_small_time_steps(
    const SimulationClockData &clock_data, double dt) const {

  int_t k = clock_data.k;
  double alpha = (k < 100 ? (double(k) + 1) * 0.01 : 1.0);
  return alpha * dt;
}

// ------------------------------------------------------------
// FixedDurationAndTimeSteps

FixedDurationAndTimeSteps::FixedDurationAndTimeSteps(double final_time,
                                                     int_t n_steps)
    : t_end(final_time), n_steps(n_steps) {

  auto n_digits = int_t(zisa::ceil(zisa::log10(double(n_steps))));
  n_digits = zisa::min(n_digits + 1, int_t(6));

  progress_format
      = string_format("%% %dd / %% %dd | %%.2e / %%.2e", n_digits, n_digits);
}

bool FixedDurationAndTimeSteps::is_finished(
    const SimulationClockData &clock_data) const {
  return clock_data.k >= n_steps || clock_data.t >= t_end;
}

std::string FixedDurationAndTimeSteps::compact_progress_string(
    const SimulationClockData &clock_data) const {

  return string_format(
      progress_format.c_str(), clock_data.k, n_steps, clock_data.t, t_end);
}

double
FixedDurationAndTimeSteps::pick_time_step(const SimulationClockData &clock_data,
                                          double dt_cfl) const {

  double dt = super::pick_time_step(clock_data, dt_cfl);
  dt = zisa::min(dt, t_end - clock_data.t);

  LOG_ERR_IF(dt < 0.0, string_format("Non-positive `dt`. [%.e]", dt));

  return dt;
}

bool DummyTimeKeeper::is_finished(const SimulationClockData &) const {
  return true;
}

std::string
DummyTimeKeeper::compact_progress_string(const SimulationClockData &) const {
  return "";
}
} // namespace zisa
