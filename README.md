**Disassembler** and **interpreter** for **Intel 8086** processor architecture on **MINIX** system. <br>
Code analyze binary created for 16bit Intel 8086 architecture and execute it. 

Based on:<br>
https://www.electro-tech-online.com/datasheets/8086_intel.pdf <br>
https://cdrdv2.intel.com/v1/dl/getContent/671110  <br>
https://github.com/Stichting-MINIX-Research-Foundation/minix

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
./build/firmware/firmware [-d|-m] {binary_file}
```
Example binary files compiled for dedicated architecture and OS are placed **bin** directory.

<h2>Execution:</h2>

- ` ` - To just execute
- `-m` - For executing with logs
- `-d` - For dissasembling 
