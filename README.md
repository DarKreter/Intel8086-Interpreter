**Disassembler** for **Intel 8086** processor architecture on **MINIX** system. <br>
Code analyze binary created for 16bit Intel 8086 architecture and print result similiar to `mmvm -d` command. 

Based on official documentation:<br>
https://www.electro-tech-online.com/datasheets/8086_intel.pdf

Requirements:
```
apt install g++ cmake
```
Compilation:
```
mkdir build
cmake -S . -B build
cmake --build build --target firmware
```
Usage:
```
./build/firmware/firmware {binary_file}
```
Example binary files compiled for dedicated architecture and OS are placed **bin** directory.

Comparison with `mmvm -d` can be seen running `./test/test.sh {binary_file}` script. <br>
Absence of output in running this script is desired result. <br>
If any output occur, it means there are differences showed by `diff`.

To use `test.sh` it is required to install `mmvm` and place it in the system path.
