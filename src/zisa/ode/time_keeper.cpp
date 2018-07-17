/* Keep track of how much simulation time passed.
 *
 * Authors: Luc Grosheintz <forbugrep@zoho.com>
 *    Date: 2016-03-08
 */
#include <zisa/ode/time_keeper.hpp>
#include <zisa/math/basic_functions.hpp>
#include <zisa/math/comparison.hpp>

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
// FixedDuration

FixedDuration::FixedDuration(double t_end) : t_end(t_end) {}

bool FixedDuration::is_finished(const SimulationClockData &clock_data) const {
  return clock_data.t >= final_time();
}

double FixedDuration::pick_time_step(const SimulationClockData &clock_data,
                                     double dt_cfl) const {
  double dt = super::pick_time_step(clock_data, dt_cfl);
  dt = zisa::min(dt, final_time() - clock_data.t);

  LOG_ERR_IF(dt < 0.0, string_format("Non-positive `dt`. [%.e]", dt));

  return dt;
}

std::string FixedDuration::compact_progress_string(
    const SimulationClockData &clock_data) const {
  return string_format("%.2e/%.2e", clock_data.t, final_time());
}

double FixedDuration::final_time(void) const { return t_end; }

// ------------------------------------------------------------
// FixedTimeSteps

FixedTimeSteps::FixedTimeSteps(int_t n_steps) : n_steps(n_steps) {
  auto n_digits = int_t(zisa::ceil(zisa::log10(double(zisa::max(n_steps, int_t(10))))));
  progress_format = string_format("%% %dd/%% %dd", n_digits, n_digits);
}

bool FixedTimeSteps::is_finished(const SimulationClockData &clock_data) const {
  return clock_data.k >= final_step();
}

std::string FixedTimeSteps::compact_progress_string(
    const SimulationClockData &clock_data) const {
  return string_format(progress_format.c_str(), clock_data.k, final_step());
}

int_t FixedTimeSteps::final_step(void) const { return n_steps; }

} // namespace zisa
