Git Repository for the ACF (Acquisition & Control Framework)
============================================================

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Contains__

- A middleware API layer, implemented in C++, which will basically wrap
to abstracted functions the firmware calls and handshakes currently
hardcoded into DAQ systems software

- A C++ object-based library describing the system components (CBCs,
Hybrids, Boards) and their properties(values, status)

- An .xml parser to read configuration files that describe the HW & connection.

- The MCP test program / system / data-acqisition tests

Different versions for different setups
---------------------------------------

On this GitHub, you can find different version of the software :
- An agnostic (to the number of CBCs) version with the new structure in the Master branch
- A 2CBC version on the 2CBC branch with the previous structure (old -- do not use!)
- A 8CBC version on the 8CBC branch with the previous structure (old -- do not use!)
- An in-progress version in the dev branch
<br>
<br>
__What are the differences between the 2CBC/8CBC versions ?__

The differences in the two versions are located in the data format (event buffer size) & the presence of two registers in the FW (see HWdescription.xml for both versions!)


Installation Guide
==================


Prerequisites
-------------
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

3. Download [boost] (http://www.boost.org/users/history/version_1_55_0.html), then install:

        tar --bzip2 -xf boost_1_55_0.tar.bz2
	    cd boost_1_55_0
	    ./bootstrap.sh --prefix=/usr/
	    ./b2 install

   Then check Boost v1.55 is correctly installed using:

        cat /usr/include/boost/version.hpp | grep "BOOST_LIB_VERSION"

4. Finally, update uHAL to version 2.3:

        sudo yum groupremove uhal
        wget http://svnweb.cern.ch/trac/cactus/export/28265/tags/ipbus_sw/uhal_2_3_0/scripts/release/cactus.slc5.x86_64.repo
        sudo cp cactus.slc5.x86_64.repo /etc/yum.repos.d/cactus.repo
        sudo yum clean all
        sudo yum groupinstall uhal

Note: You may also need to set the environment variables:

    export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
    export PATH=/opt/cactus/bin:$PATH


Installing the Software
-------------------------------------------------

Here are the step to make the program functional
(Tested with the latest version of gcc, µHal, boost)

1. Recover the GitHub repo.

2. Create /lib, /bin and /output directories in the root of the repo.

3. Do a make in the root the repo (make sure you have all µHal, root, boost... libraries on your computer).

4. Launch system command if you want to test the reading of your hardware describing XML file.

5. Launch testpgrm command if you want to test if everything is working good.

6. You can test the data acquisition by lauching datatest2cbc or datatest8cbc.

7. Launch mcp to play with the Test Interface
<br>
<br>
__Functionality__

You can choose to load your Hardware description XML file and it will create the
 software counterpart of your existing hardware.

After this creation round, you can do anything you want :
- Configure the Glib or the Cbcs
- Manipulate the registers in the Glib
- Manipulate the registers in the Cbcs
- Perform a DAQ and tracing diagrams counting the number of hits
- Have a configuration summary of all the objects you created

When you write a register in the Glib or the Cbc, the writing is updated to the
map contained in Glib/Cbc objects so that you're always fully aware of what is
in the Glib/Cbc register.

For writing value in register, we invite you to put in the following format : 0x__.
You must thus type '0xFF' for exemple and not just 'FF' in the command line.

You can also read data and some test-histograms will be saved to the output folder.

At the end of the program, the register maps are saved into output_X_X_X.txt files
you can find in the settings directory. For example, output_0_1_2.txt contains the
register map of the Cbc 2 of the Module 1 of the Board 0
You can also find different .pdf files containing the histograms of the DAQ.

For debugging purpose, you can activate DEV_FLAG in files or in Makefile and also activate the uHal log in RegManager.cc.

More features will be implemented later, so make sure to have the last release
locally to benefit from them.

Warning ! : be careful with options choice in the program menus, some mistypes can leed
to unexpected hazards :-(.


On the go...
------------

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Last Updates__

- 09/07/14 : Added threading for stack writing registers
- 10/07/14 : Read Data from acquisition in a rubbish format
- ~~25/07/14 : Fully functional for 2CBC (safe from Broadcast obviously), pending for 8CBC~~
- ~~28/07/14 : Found a bug in the reading of CBC1 of 2CBC, trying to see if coming from soft or hard~~
- 30/07/14 : Working 2CBC version, find a 8CBC working version in the 8CBC branch
- 12/08/14 : Working agnostic version of the new structure on Master
- 15/08/14 : System Controller Class working
- 19/08/14 : Project wrapped, called ACF for Acquisition & Control Framework
<br>
<br>


Support, Suggestions ?
----------------------

For any support/suggestions, mail Lorenzo Bidegain, Nicolas Pierre or Georg Auzinger.
