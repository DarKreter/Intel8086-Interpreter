mmvm -d $1 2> mmvm.result
../build/firmware $1 > firmware.result

diff --text --unified -w firmware.result mmvm.result | head -n 45 | colordiff
