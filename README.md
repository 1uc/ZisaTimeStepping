# ZisaTimeStepping
ZisaTimeStepping is provides funcionality related to time stepping. In HPC
applications often the problem is solved by taking small steps forward in time.
This library provides a progress bar and a `TimeKeeper` which keeps track of
the current simulation time, as well as classes to decide when the simulation
should be written to file or (eventually) stopped.

Note: This library does implement any ODE solvers.

## Quickstart
Start by cloning the repository

    $ git clone https://github.com/1uc/ZisaTimeStepping.git

and change into the newly created directory. Then proceed to install the
dependencies:

    $ bin/install_dir.sh COMPILER DIRECTORY       \
                         [--zisa_has_mpi={0,1}]   \
                         [--zisa_has_cuda={0,1}]

they will be placed into a subdirectory of `DIRECTORY` and print
part of the CMake command needed to include the dependencies. `COMPILER` must
be replaced with the compiler you want to use.

If this worked continue by adding [project specific flags] to the CMake
command. If not it's not going to be a quick start. Please read
[Dependencies] and then [Building].

[project specific flags]: https://1uc.github.io/ZisaTimeStepping/md_cmake.html#cmake_flags
[Dependencies]: https://1uc.github.io/ZisaTimeStepping/md_dependencies.html
[Building]: https://1uc.github.io/ZisaTimeStepping/md_cmake.html
