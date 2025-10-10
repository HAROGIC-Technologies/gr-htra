# gr-htra
A GNU Radio source module for HTRA SDR devices, providing real-time IQ data streaming and control via the Harogic API.

## Requirements
- 64-bit Linux operating system  
  - Tested on Ubuntu 22.04 and above
- Native USB 3.0 support

## Prerequisites
1. Install [**GNU Radio**](https://wiki.gnuradio.org/index.php/InstallingGR)
  - GNU Radio 3.9 or above is required.

3.  Before building, you must have the following installed on your system:
   - **CMake**: `cmake`  
   - **C++ Compiler**: `g++`  
   - **Git**: `git`  
   - **Pybind11**: `pybind11`

   Example installation on Ubuntu:
  ```bash
  sudo apt update
  sudo apt install -y git cmake g++ libpython3-dev python3-numpy python3-pip python3-setuptools pybind11-dev
  ```

## Installation
1. Clone the repository
```bash
git clone https://github.com/HAROGIC-Technologies/gr-htra.git
```
2. Build the Module
- Build the module with CMake:
```bash
cd gr-htra
mkdir build
cd build
sudo cmake ..
sudo make install
```
3. Copy the calibration file(s)
  - Copy the calibration file(s) for your device from the included USB drive into the `gr-htra/CalFile` folder.
  - Then run the following command to return to the **gr-htra** folder and install these calibration files system-wide:
```bash
cd ..
sudo sh CopyCalFile.sh
```
4. After the installation is complete, you can open **GNU Radio Companion** and use the HTRA OOT module. Example usage can be found in the **Examples** folder.

## Uninstallation
If you need to uninstall the HTRA OOT module along with all related configuration files, simply run the following script in the **gr-htra** folder:
```bash
sudo sh uninstall.sh
```

## Usage
- Add the `HTRA:IQ Source` block to flowgraphs in the GNU Radio Companion. It is located under the HTRA category.
  - See `examples` folder for demos.
<p align="center">
  <a href="Examples/photos/FM_demod.png" title="FM Demod Flowgraph">
    <img src="Examples/photos/FM_demod.png" alt="FM Demod Flowgraph" width="800">
  </a>
</p>
