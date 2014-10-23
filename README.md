CMS Ph2 ACF (Acquisition & Control Framework)      {#mainpage}
======================================

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Supposed to contain__

- A middleware API layer, implemented in C++, which wraps the firmware calls and handshakes into abstracted functions

- A C++ object-based library describing the system components (CBCs,
Hybrids, Boards) and their properties(values, status)

- several utilities (like visitors to execute certain tasks for each item in the hierarchical Item description)

- a tools/ directory with several utilities (currently: calibration, hybrid testing)

- some applications: datatest, interfacetest, hybridtest, system, calibrate

Different versions
---------------------------------------

On this GitHub, you can find different version of the software :
- An agnostic (to the number of CBCs) version with the new structure in the Master branch
- An in-progress version in the Dev branch
<br>
<br>

Changelog:
------------

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Last Updates__

- 09/07/14 : Added threading for stack writing registers
- 12/08/14 : Working agnostic version of the new structure on Master
- 15/08/14 : System Controller Class working
- 19/08/14 : Project wrapped, called ACF for Acquisition & Control Framework
- 09/10/14 : added Visitor class and the corresponding accept methods to the HWDescription objects
- 15/10/14 : re-wrote the GlibFWInterface::ReadData() method, completeley re-wrote the parsing of the raw buffer and the offsets, modified the Data and Event classes to be more lightweight and less complex
- 17/10/14 : renamed the project to Ph2_ACF & re-structured the folder architecture and added command line parsing to the executables
<br>
<br>

Preliminary Setup
-----------------

Firmware for the GLIB can be found in /firmware. To check you are using the correct firmware with the correct FMC, please read /latex/FirmwareHardware.pdf.

NOTE: If you are doing the install for the first time on the latest [VM v1.1.0] (http://sbgcmstrackerupgrade.in2p3.fr/) then follow the preliminary setup, otherwise you can skip this.

1. Remove the current gcc and old boost libraries:

        sudo yum remove devtoolset-1.1-gcc-debuginfo
        sudo yum remove boost

2. Install the latest gcc compiler:

        sudo yum install devtoolset-2
        sudo ln -s /opt/rh/devtoolset-2/root/usr/bin* /usr/local/bin/
        hash -r

   This should give you gcc 4.8.1:

        /usr/bin/gcc --version

3. Finally, update uHAL to version 2.3:

        sudo yum groupremove uhal
        wget http://svnweb.cern.ch/trac/cactus/export/28265/tags/ipbus_sw/uhal_2_3_0/scripts/release/cactus.slc5.x86_64.repo 
        sudo cp cactus.slc5.x86_64.repo /etc/yum.repos.d/cactus.repo
        sudo yum clean all
        sudo yum groupinstall uhal

Note: You may also need to set the environment variables:

    export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
    export PATH=/opt/cactus/bin:$PATH


The Ph2DAQ (ACF) Software itself : 
-------------------------------------------------

Follow these instructions to install and compile the libraries:
(provided you installed the latest version of gcc, µHal, boost as mentioned above)

1. Clonethe GitHub repo.

2. Do a make in the root the repo (make sure you have all µHal, root, boost... libraries on your computer).

3. Launch system command if you want to test the reading of your hardware describing XML file.

4. Launch

        datatest --help
        
command if you want to test if you can correctly read data.

6. launch

        calibrate --help

to calibrate a hybrid,

        hybridtest --help

to test a hybird's I2C registers and input channel connectivity

7. an example of how to use visitors can be found in src/interfacetest.cc or in the HybridTester class


<br>
<br>
__What can you do with the software ?__

You can choose to load your Hardware description XML file and it will create the
 software counterpart of your existing hardware.

After this creation round, you can do anything you want :
- Configure the Glib or the Cbcs
- Manipulate the registers in the Glib
- Manipulate the registers in the Cbcs
- Read Data
- Calibrate Hybrids
- Validate Hybrids
- any other routine you want to implement yourself ... 

When you write a register in the Glib or the Cbc, the writing is updated to the
map contained in Glib/Cbc objects so that you're always fully aware of what is
in the Glib/Cbc register.

For writing value in register, we invite you to put in the following format : 0x__.
You must thus type '0xFF' for exemple and not just 'FF' in the command line.

For debugging purpose, you can activate DEV_FLAG in files or in Makefile and also activate the uHal log in RegManager.cc.

More features will be implemented later, so make sure to have the last release
locally to benefit from them.


Support, Suggestions ?
----------------------

For any support/suggestions, mail Lorenzo Bidegain, Nicolas Pierre or Georg Auzinger.
