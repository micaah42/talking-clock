#~/usr/bin/bash

device=clock
set -e
set -x

cmake ..
make -j12 package
scp talking-clock-0.3-Linux.tar.gz $device:~/
ssh $device "tar -xvf ~/talking-clock-0.3-Linux.tar.gz -C /usr"
ssh $device "systemctl restart clockd"
