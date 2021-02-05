#! /usr/bin/env bash

if [[ "$#" -ne 2 ]]
then
    echo "Usage: $0 COMPILER DESTINATION"
fi

component_name="ZisaTimeStepping"
zisa_dependencies=("ZisaCore" "ZisaMemory" "ZisaMPI")

zisa_memory_root=$(realpath $(dirname $(readlink -f $0))/..)

install_dir=$(${zisa_memory_root}/bin/install_dir.sh $1 $2)
source_dir=${install_dir}/sources

echo ${install_dir}
echo ${source_dir}

conan_file=${zisa_memory_root}/conanfile.txt

if [[ -f $conan_file ]]
then
   mkdir -p ${install_dir}/conan && cd ${install_dir}/conan
   conan install $conan_file -s compiler.libcxx=libstdc++11
fi

mkdir -p ${source_dir}
for dep in "${zisa_dependencies[@]}"
do
    echo ${src_dir}
    src_dir=${source_dir}/$dep
    git clone git@github.com:1uc/${dep}.git ${src_dir}

    mkdir -p ${src_dir}/build-dep
    cd ${src_dir}/build-dep

    echo ${install_dir}

    cmake -DCMAKE_INSTALL_PREFIX=${install_dir}/zisa \
          -DCMAKE_PREFIX_PATH=${install_dir}/zisa/lib/cmake/zisa \
          -DCMAKE_PROJECT_${dep}_INCLUDE=${install_dir}/conan/conan_paths.cmake \
          -DCMAKE_BUILD_TYPE=Release \
          ..

    cmake --build .
    cmake --install .
done
