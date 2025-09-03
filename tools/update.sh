#~/usr/bin/bash

set -e

device="${1:-clock}"
environment_file="${2:-/opt/poky/5.0.8/environment-setup-cortexa72-poky-linux}"
build_directory="${3:-build/rpi-auto}"

echo building in $build_directory, using $environment_file and deploying to $device

cd $(dirname $0)/..

mkdir -p $build_directory
cd $build_directory

source $environment_file

cmake ../.. \
      -DCMAKE_BUILD_TYPE=Release \
      -DFETCHCONTENT_FULLY_DISCONNECTED=ON

cmake --build . \
      -j $(nproc --ignore 2) \
      --target all package


archive=$(ls -t talking-clock-*-Linux.tar.gz | head -1)
scp $archive $device:/tmp
ssh $device "tar -xvf /tmp/${archive} -C /usr"
ssh $device "systemctl restart clockd"
