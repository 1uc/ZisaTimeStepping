// SPDX-License-Identifier: MIT
// Copyright (c) 2021 ETH Zurich, Luc Grosheintz-Laval

#ifndef TIME_KEEPER_DECL_H_DY6TALRI
#define TIME_KEEPER_DECL_H_DY6TALRI

#include <vector>

#include <zisa/config.hpp>
#include <zisa/datetime.hpp>
#include <zisa/math/comparison.hpp>

namespace zisa {
struct SimulationClockData {
  double t;
  double dt;
  int_t k;

  bool is_finished;
  bool is_interrupted;
  bool is_plotting_step;
};

/// Interface for keeping track of simulation time.
/** @see PlottingSteps
 */
class TimeKeeper {
public:
  TimeKeeper();
  virtual ~TimeKeeper() = default;

  /// Is the simulation finished?
  virtual bool is_finished(const SimulationClockData &clock_data) const = 0;

  /// Has the simulation been interrupted?
  /** The simulation `is_finished`, i.e. needs to be stopped. However, if
   *  it was interrupted one may want to restart the simulation again.
   *
   *  @note This method assumes that the `is_finished` is true.
   */
  virtual bool is_interrupted(const SimulationClockData &clock_data) const;

  /// Enforce constraints on the proposed time step.
  virtual double pick_time_step(const SimulationClockData &clock_data,
                                double dt_cfl) const;

  /// Return a compact progress string, e.g. `0011/1000`.
  virtual std::string
  compact_progress_string(const SimulationClockData &clock_data) const = 0;

  /// Reduce the time-step during the first few steps.
  /** For discontinuous initial conditions the wave speeds are typically
   *  underestimated. Therefore, the CFL time-step is too large. The
   *  resulting scheme can be unstable.
   *
   *  Calling this will activate a fix, see
   *  `enforce_initially_small_time_steps`.
   */
  void activate_small_steps();

protected:
  virtual double
  enforce_initially_small_time_steps(const SimulationClockData &clock_data,
                                     double dt) const;

private:
  bool needs_small_steps;
};

/// Simulation ends when it reaches a certain time or number of time steps.
class FixedDurationAndTimeSteps : public TimeKeeper {
private:
  using super = TimeKeeper;

public:
  /// Construct time-keeper that ends the simulation after `n_steps` steps.
  FixedDurationAndTimeSteps(double final_time, int_t n_steps);

  virtual bool
  is_finished(const SimulationClockData &clock_data) const override;

  virtual double pick_time_step(const SimulationClockData &clock_data,
                                double dt_cfl) const override;

  virtual std::string
  compact_progress_string(const SimulationClockData &clock_data) const override;

private:
  double t_end;
  int_t n_steps;
  std::string progress_format;
};

/// Terminate simulation after specified wall-clock duration.
template <class BaseTimeKeeper>
class FixedWallClock : public BaseTimeKeeper {
private:
  using super = BaseTimeKeeper;

public:
  template <class... Args>
  FixedWallClock(const std::string &duration, Args &&... args);

  virtual ~FixedWallClock() = default;

  virtual bool
  is_finished(const SimulationClockData &clock_data) const override;

  virtual bool
  is_interrupted(const SimulationClockData &clock_data) const override;

protected:
  bool time_is_up() const;

private:
  double wall_clock_budget;
  zisa::time_stamp_t simulation_start;
};

class DummyTimeKeeper : public TimeKeeper {
public:
  virtual bool is_finished(const SimulationClockData &) const override;

  virtual std::string
  compact_progress_string(const SimulationClockData &) const override;
};

} // namespace zisa
#endif /* end of include guard: TIME_KEEPER_H_DY6TALRI */
