// SPDX-License-Identifier: MIT
// Copyright (c) 2021 ETH Zurich, Luc Grosheintz-Laval

/* Template and inline definitions for time_keeper.h.
 */
#ifndef TIME_KEEPER_INC_CAI3J4N5
#define TIME_KEEPER_INC_CAI3J4N5

#include "time_keeper_decl.hpp"

namespace zisa {

// ---------------------------------------------------------------------------
// FixedWallClock

template <class BaseTimeKeeper>
template <class... Args>
FixedWallClock<BaseTimeKeeper>::FixedWallClock(const std::string &duration,
                                               Args &&... args)
    : super(std::forward<Args>(args)...),
      wall_clock_budget(string_to_seconds(duration)),
      simulation_start(current_time_stamp()) {}

template <class BaseTimeKeeper>
bool FixedWallClock<BaseTimeKeeper>::is_finished(
    const SimulationClockData &clock_data) const {

  return time_is_up() || super::is_finished(clock_data);
}

template <class BaseTimeKeeper>
bool FixedWallClock<BaseTimeKeeper>::is_interrupted(
    const SimulationClockData &clock_data) const {

  // The run has stop due to the run-time constraint, if it was stopped,
  // but not stopped due to the usual constraint.
  return !super::is_finished(clock_data);
}

template <class BaseTimeKeeper>
bool FixedWallClock<BaseTimeKeeper>::time_is_up() const {
  auto elapsed = elapsed_seconds_since(simulation_start);
  return elapsed > wall_clock_budget;
}

} // namespace zisa
#endif /* end of include guard */
