#include <algorithm>

#include <zisa/math/comparison.hpp>
#include <zisa/ode/plotting_steps.hpp>

namespace zisa {
// ---- PlottingSteps --------------------------------------------------------
bool PlottingSteps::is_plotting_step(const SimulationClockData &clock_data) {
  return is_plotting_step_impl(clock_data);
}

double PlottingSteps::pick_time_step(const SimulationClockData &clock_data,
                                     double dt) {
  return pick_time_step_impl(clock_data, dt);
}

double PlottingSteps::pick_time_step_impl(const SimulationClockData &,
                                          double dt) {
  return dt;
}

// ---- PlotEveryNthStep -----------------------------------------------------
PlotEveryNthStep::PlotEveryNthStep(int_t first_frame, int_t steps_per_frame)
    : first_frame(first_frame), steps_per_frame(steps_per_frame) {}

bool PlotEveryNthStep::is_plotting_step_impl(
    const SimulationClockData &clock_data) {

  auto k = clock_data.k;

  // don't replot the restarting frame.
  if (first_frame != 0 && k == first_frame) {
    return false;
  }

  return k % steps_per_frame == 0;
}

// ---- PlotAtFixedInterval --------------------------------------------------
PlotAtFixedInterval::PlotAtFixedInterval(double t0, double dt_vis, double t_end)
    : dt_vis(dt_vis),
      t0(t0),
      t_vis(t0 + dt_vis),
      t_vis_last(t0),
      t_end(t_end) {}

bool PlotAtFixedInterval::is_plotting_step_impl(
    const SimulationClockData &clock_data) {
  double t = clock_data.t;

  // wlog do not plot the restarting frame.
  if (t0 != 0.0 && t == t0) {
    return false;
  }

  bool is_first_frame = (t == 0.0);
  bool is_over_due = (t >= t_vis);
  bool is_repeated_query = (t == t_vis_last);
  bool is_finished = clock_data.is_finished;

  return is_first_frame || is_over_due || is_repeated_query || is_finished;
}

double
PlotAtFixedInterval::pick_time_step_impl(const SimulationClockData &clock_data,
                                         double dt) {
  double t = clock_data.t;

  if (t >= t_vis) {
    t_vis_last = t_vis;
    t_vis += dt_vis;

    if (zisa::abs(t_vis - t_end) < 1e-14 * t_end) {
      t_vis = t_end;
    }
  }

  dt = zisa::min(dt, t_vis - t);

  LOG_ERR_IF(dt < 0.0, string_format("Non-positive `dt`. [%e]", dt));
  return dt;
}

// ---- PlotAtFixedTimeSteps -------------------------------------------------
PlotAtFixedTimeSteps::PlotAtFixedTimeSteps(const std::vector<int_t> &frames)
    : frames(frames) {
  std::sort(this->frames.begin(), this->frames.end());
}

bool PlotAtFixedTimeSteps::is_plotting_step_impl(
    const SimulationClockData &clock_data) {
  return is_in_list(clock_data.k);
}

bool PlotAtFixedTimeSteps::is_in_list(int_t k) const {
  return std::binary_search(frames.begin(), frames.end(), k);
}

bool DummyPlottingSteps::is_plotting_step_impl(
    const SimulationClockData &clock_data) {
  return false;
}
} // namespace zisa
