mmvm -d $1 > mmvm.result 2>&1
../build/firmware/firmware -d $1 > firmware.result 2>&1

diff --text --unified -w firmware.result mmvm.result | head -n 45 |  colordiff

