/* When to save the state to disk? The logic is here.
 */
#ifndef IO_STEPS_H_SUTZCMO4
#define IO_STEPS_H_SUTZCMO4

#include <zisa/config.hpp>
#include <zisa/ode/time_keeper.hpp>

namespace zisa {
/// Abstract interface for deciding if a 'plotting' is required.
/** The term plotting is used very liberally to mean, store a
 *  snapshot to disk.
 */
class PlottingSteps {
public:
  PlottingSteps() = default;
  virtual ~PlottingSteps() = default;

  /// Convenience interface.
  bool is_plotting_step(const SimulationClockData &clock_data);

  /// Convenience interface.
  double pick_time_step(const SimulationClockData &clock_data, double dt);

protected:
  virtual bool is_plotting_step_impl(const SimulationClockData &clock_data) = 0;
  virtual double pick_time_step_impl(const SimulationClockData &clock_data,
                                     double dt);
};

/// Plot every fixed number of time-steps.
/** For example every 5-th step, e.g. `step = 0, 5, 10, 15, ...`.
 */
class PlotEveryNthStep : public PlottingSteps {
public:
  PlotEveryNthStep(int_t first_frame, int_t steps_per_frame);
  virtual ~PlotEveryNthStep() = default;

protected:
  virtual bool
  is_plotting_step_impl(const SimulationClockData &clock_data) override;

private:
  int_t first_frame;
  int_t steps_per_frame;
};

/// Plot at a fixed time-interval
/** Plotting times are `t = k*dt_vis` with `k = 0, 1, 2, ...`.
 */
class PlotAtFixedInterval : public PlottingSteps {
public:
  PlotAtFixedInterval(double t_vis, double dt_vis, double t_end);
  virtual ~PlotAtFixedInterval() = default;

protected:
  virtual bool
  is_plotting_step_impl(const SimulationClockData &clock_data) override;
  virtual double pick_time_step_impl(const SimulationClockData &clock_data,
                                     double dt) override;

private:
  double dt_vis;
  double t0;
  double t_vis;
  double t_vis_last;
  double t_end;
};

/// Plot at a predefined list of steps.
/** For example at `step = 0, 10, 12, 13, 300`.
 */
class PlotAtFixedTimeSteps : public PlottingSteps {
public:
  PlotAtFixedTimeSteps(const std::vector<int_t> &frames);
  virtual ~PlotAtFixedTimeSteps() = default;

protected:
  virtual bool
  is_plotting_step_impl(const SimulationClockData &clock_data) override;

private:
  bool is_in_list(int_t k) const;

private:
  std::vector<int_t> frames;
};

class DummyPlottingSteps : public PlottingSteps {
protected:
  virtual bool is_plotting_step_impl(const SimulationClockData &) override;
};

} // namespace zisa
#endif /* end of include guard: IO_STEPS_H_SUTZCMO4 */
