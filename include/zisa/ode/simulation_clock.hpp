/* Combines two `TimeKeeper` and `PlottingSteps` objects.
 *
 * Authors: Luc Grosheintz <forbugrep@zoho.com>
 *    Date: 2017-11-28
 */

#ifndef SIMULATION_CLOCK_H_70YQ2
#define SIMULATION_CLOCK_H_70YQ2

#include <zisa/config.hpp>
#include <zisa/ode/plotting_steps.hpp>
#include <zisa/ode/time_keeper.hpp>

namespace zisa {

class SimulationClock {
public:
  SimulationClock(const std::shared_ptr<TimeKeeper> &time_keeper,
                  const std::shared_ptr<PlottingSteps> &plotting_steps);

  virtual ~SimulationClock() = default;

  /// Advance to time `t` and time-step `k`.
  void advance_to(double t, int_t k);

  /// Advance the clock by one step.
  void advance();

  /// Set the time-step for the next update.
  /* This may enforce additional constraints on the time-step. Than therefore
   * reduce `dt_clf` if needed to, say, achieve a certain plotting frequency.
   */
  void set_time_step(double dt_cfl);

  /// Should the time-loop stop?
  bool is_finished(void) const;

  /// Has the simulation been interrupted?
  /** The simulation `is_finished`, i.e. needs to be stopped. However, if
   *  it was interrupted one may want to restart the simulation again.
   *
   *  @note This method assumes that the `is_finished` is true.
   */
  bool is_interrupted(void) const;

  /// Should visualization be performed now?
  bool is_plotting_step(void) const;

  /// Return a compact progress string, e.g. `0011/1000`.
  std::string compact_progess_string(void) const;

  /// Return the number of time steps performed.
  int_t current_step(void) const;

  /// Return the current simulation time.
  double current_time(void) const;

  /// Return the size of the current time step.
  double current_time_step(void) const;

protected:
  virtual bool is_master() const = 0;
  virtual double reduce(double dt) const = 0;
  virtual void broadcast() = 0;

protected:
  SimulationClockData clock_data;
  std::shared_ptr<TimeKeeper> time_keeper;
  std::shared_ptr<PlottingSteps> plotting_steps;
};

class SerialSimulationClock : public SimulationClock {
private:
  using super = SimulationClock;

public:
  using super::super;

protected:
  virtual bool is_master() const override;
  virtual double reduce(double dt) const override;
  virtual void broadcast() override;
};

#ifdef ZISA_HAS_MPI
class MPISimulationClock : public SimulationClock {
private:
  using super = SimulationClock;

public:
  MPISimulationClock(const std::shared_ptr<TimeKeeper> &time_keeper,
                     const std::shared_ptr<PlottingSteps> &plotting_steps,
                     const MPI_Comm &mpi_comm);

protected:
  virtual bool is_master() const override;
  virtual double reduce(double dt) const override;
  virtual void broadcast() override;

protected:
  int mpi_rank;
  MPI_Comm mpi_comm;
};
#endif

} // namespace zisa

#endif /* end of include guard */
