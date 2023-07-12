mmvm -m $1 2> mmvm.result >&2
../build/firmware/firmware -m $1 2> firmware.result >&2

diff --text --unified -w firmware.result mmvm.result | head -n 45 |  colordiff

