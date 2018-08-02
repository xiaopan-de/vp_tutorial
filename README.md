# vp_tutorial
This repository consists of a set of tutorial examples for the lecture virtual prototyping that covers SysML, SystemC (incl. TLM and AMS extensions), instructions set simulator (ISS).  Details can be found in the subfolders.  

## Usage
0. This tool is available at github [vp_tutorial](https://github.com/panxiao-tech/vp_tutorial)
1. We assume you have a UNIX style system, e.g. OSX or Linux.
2. GCC version 4.9, CMake version 2.8, SystemC 2.3.0, SystemC-AMS 2.0 or higher are required.
3.  Set environment variables for SysremC and SystemC-AMS
  ```shell
  export SYSTEMC_BASE = PathToInstallationSystemC/systemc-2.3.0
  export SYSTEMCAMS_BASE = PathToInstallationSystemCAMS/systemc_ams
  ```
4. Make a directory `builddir` in the top-level directory:
```shell
> mkdir builddir
```
5. Go to `builddir` and build it:
```shell
> cd builddir
> cmake ..
> make
```
6. In case you want to setup debug level: 
```shell
> camke .. -DDEBUG_LEVEL=1
```
7. Go to folder `vp_tutorial\bin` to execute demos :
```shell
> cd ..
> cd bin
> ./tlm_demo1 
```
