mmvm -m $1 2>&1 | head -n $2 > mmvm.result 
../build/firmware/firmware -m $1 2>&1 | head -n $2 > firmware.result

diff --text --unified -w firmware.result mmvm.result | head -n 45  |  colordiff

