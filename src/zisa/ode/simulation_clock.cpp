/* Definitions of the simulation clock.
 */

#include <algorithm>
#include <zisa/ode/simulation_clock.hpp>

namespace zisa {

SimulationClock::SimulationClock(
    std::shared_ptr<TimeKeeper> time_keeper,
    std::shared_ptr<PlottingSteps> plotting_steps)
    : time_keeper(std::move(time_keeper)), plotting_steps(std::move(plotting_steps)) {

  clock_data.t = 0.0;
  clock_data.k = 0;

  clock_data.is_interrupted = false;
  clock_data.is_finished = false;
  clock_data.is_plotting_step = true;
}

bool SimulationClock::is_finished() const { return clock_data.is_finished; }

bool SimulationClock::is_interrupted() const {
  return clock_data.is_interrupted;
}

bool SimulationClock::is_plotting_step() const {
  return clock_data.is_plotting_step;
}

void SimulationClock::advance_to(double t, int_t k) {
  clock_data.t = t;
  clock_data.k = k;
}

void SimulationClock::advance() {
  clock_data.t += clock_data.dt;
  clock_data.k += 1;
}

void SimulationClock::set_time_step(double dt_cfl) {
  double dt_global = reduce(dt_cfl);

  if (is_master()) {
    auto dt_tmp = time_keeper->pick_time_step(clock_data, dt_global);
    clock_data.dt = plotting_steps->pick_time_step(clock_data, dt_tmp);

    clock_data.is_finished = time_keeper->is_finished(clock_data);
    clock_data.is_interrupted = time_keeper->is_interrupted(clock_data);
    clock_data.is_plotting_step = plotting_steps->is_plotting_step(clock_data);
  }

  broadcast();
}

std::string SimulationClock::compact_progess_string() const {
  return time_keeper->compact_progress_string(clock_data);
}

int_t SimulationClock::current_step() const { return clock_data.k; }

double SimulationClock::current_time() const { return clock_data.t; }

double SimulationClock::current_time_step() const { return clock_data.dt; }

bool SerialSimulationClock::is_master() const { return true; }

double SerialSimulationClock::reduce(double dt_local) const { return dt_local; }

void SerialSimulationClock::broadcast() { }

#ifdef ZISA_HAS_MPI
MPISimulationClock::MPISimulationClock(
    std::shared_ptr<TimeKeeper> time_keeper,
    std::shared_ptr<PlottingSteps> plotting_steps,
    const MPI_Comm &mpi_comm)
    : super(std::move(time_keeper), std::move(plotting_steps)),
      mpi_rank(zisa::mpi::rank(mpi_comm)),
      mpi_comm(mpi_comm) {}

bool MPISimulationClock::is_master() const { return mpi_rank == 0; }

double MPISimulationClock::reduce(double dt_local) const {
  double dt_global = -1.0;
  MPI_Reduce(&dt_local, &dt_global, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
  return dt_global;
}

void MPISimulationClock::broadcast() {
  void *address = (void *)(&clock_data);
  auto n_bytes = integer_cast<int>(sizeof(clock_data));
  MPI_Bcast(address, n_bytes, MPI_BYTE, 0, mpi_comm);
}
#endif

} // namespace zisa
