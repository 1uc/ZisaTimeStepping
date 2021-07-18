# ZisaTimeStepping
[![Build Status](https://github.com/1uc/ZisaTimeStepping/actions/workflows/basic_integrity_checks.yml/badge.svg?branch=main)](https://github.com/1uc/ZisaTimeStepping/actions)
[![Docs Status](https://github.com/1uc/ZisaTimeStepping/actions/workflows/publish_docs.yml/badge.svg?branch=main)](https://1uc.github.io/ZisaTimeStepping)

ZisaTimeStepping is provides funcionality related to time stepping. In HPC
applications often the problem is solved by taking small steps forward in time.
This library provides a progress bar and a `TimeKeeper` which keeps track of
the current simulation time, as well as classes to decide when the simulation
should be written to file or (eventually) stopped.

Note: This library does not implement any ODE solvers.

## Quickstart
Start by cloning the repository

    $ git clone https://github.com/1uc/ZisaTimeStepping.git

and change into the newly created directory. Then proceed to install the
dependencies:

    $ bin/install_dir.sh COMPILER DIRECTORY DEPENDENCY_FLAGS

they will be placed into a subdirectory of `DIRECTORY` and print
part of the CMake command needed to include the dependencies. `COMPILER` must
be replaced with the compiler you want to use. The available `DEPENDENCY_FLAGS`
are

  * `--zisa_has_mpi={0,1}` to request MPI.
  * `--zisa_has_hdf5={0,1}` to request HDF5 for writing arrays.
  * `--zisa_has_netcdf={0,1}` to request NetCDF for writing arrays.
  * `--zisa_has_cuda={0,1}` to request CUDA (experimental).

If this worked continue by running the `cmake` command and compiling the
library. Take a look at the [project specific flags] for CMake if you want to
modify something. If this didn't work, it's not going to be a quick start.
Please read [Dependencies] and then [Building].

[project specific flags]: https://1uc.github.io/ZisaTimeStepping/md_cmake.html#cmake_flags
[Dependencies]: https://1uc.github.io/ZisaTimeStepping/md_dependencies.html
[Building]: https://1uc.github.io/ZisaTimeStepping/md_cmake.html
