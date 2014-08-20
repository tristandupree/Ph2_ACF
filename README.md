Git Repository for PH2_DAQ Software
===================================

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Supposed to contain__

- A middleware API layer, implemented in C++, which will basically wrap
to abstracted functions the firmware calls and handshakes currently
hardcoded into DAQ systems software

- A C++ object-based library describing the system components (CBCs,
Hybrids, Boards) and their properties(values, status)

- The MCP test program which is the wrapping the previous two.

Different versions for different setups
---------------------------------------

On this GitHub, you can find different version of the software :
- An agnostic (to the number of CBCs) version with the new structure in the Master branch
- A 2CBC version on the 2CBC branch
- A 8CBC version on the 8CBC branch
- An in-progress version in the dev branch
<br>
<br>
__What are the differences between the 2CBC/8CBC versions ?__

The differences mainly resides in the size of the data buffer for the DAQ, when all the access to both Board and Cbc registers is done the same way.
Also, some functions are present in 8CBC and not in 2CBC due to the fact that the firmware of the 8CBC is offering more possibilities of recovering infos from the Hardware (as the type of hardware for example)

Preliminary Setup
-----------------

Firmware for the GLIB can be found in /firmware. To check you are using the correct firmware with the correct FMC, please read /latex/FirmwareHardware.pdf.

NOTE: If you are doing the install for the first time on the latest [VM v1.1.0] (http://sbgcmstrackerupgrade.in2p3.fr/) then follow the preliminary setup, otherwise you can skip this.

i. Remove the current gcc and old boost libraries:

        sudo yum remove devtoolset-1.1-gcc-debuginfo
        sudo yum remove boost

ii. Install the latest gcc compiler:

        sudo yum install devtoolset-2
        sudo ln -s /opt/rh/devtoolset-2/root/usr/bin* /usr/local/bin/
        hash -r

   This should give you gcc 4.8.1:

        /usr/bin/gcc --version

iii. Download [boost] (http://www.boost.org/users/history/version_1_55_0.html), then install:

        tar --bzip2 -xf boost_1_55_0.tar.bz2
	cd boost_1_55_0
	./bootstrap.sh --prefix=/usr/
	./b2 install

   Then check Boost v1.55 is correctly installed using:

        cat /usr/include/boost/version.hpp | grep "BOOST_LIB_VERSION"

iv) Finally, update uHAL to version 2.3:

	sudo yum groupremove uhal 
	wget http://svnweb.cern.ch/trac/cactus/export/28265/tags/ipbus_sw/uhal_2_3_0/scripts/release/cactus.slc5.x86_64.repo
	sudo cp cactus.slc5.x86_64.repo /etc/yum.repos.d/cactus.repo
	sudo yum clean all
	sudo yum groupinstall uhal

Note: You may also need to set the environment variables:
	
  	export LD_LIBRARY_PATH=/opt/cactus/lib:$LD_LIBRARY_PATH
   	export PATH=/opt/cactus/bin:$PATH


The Test Software itself : the MCP Test Interface
-------------------------------------------------

You'll find an install step by step and a How To.
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Install the MCP Test Interface step by step...__

Here are the step to make the program functional

1. Recover the GitHub repo with the latest tested version of the MCP.

2. Change in HWDescription/Definition.h the path to the uHal connection file.
   - It's per default set to :
   file:///afs/cern.ch/user/n/npierre/dev/settings/connections.xml
   - You have to change it to :
   file://$(BUILD)/settings/connections.xml (where $(BUILD) is the path to the
   root of the GitHub repo you recovered)

3. Change in settings/connections.xml the path to the adress table
   - It's per default set to :
   file:///afs/cern.ch/user/n/npierre/dev/settings/adress_table.xml
   - You have to change it to :
   file://$(BUILD)/settings/adress_table.xml (where $(BUILD) is the path to the
   root of the GitHub repo you recovered)

4. Create /lib, /bin and /output directories in the root of the repo.

5. Do a make in the root the repo (make sure you have all uHal, root, boost... libraries on your computer)

6. Launch testpgrm command if you want to test if everything is working good.

7. You can test the data acquisition by lauching datatest2cbc or datatest8cbc.

8. Launch mcp to play with the Test Interface
<br>
<br>
__What can you do with the software ?__

A Glib is created per default (maybe in the future you will be able to play with more than one Glib)

You can then add Modules to the Glib and Cbcs to the Modules you created.
When creating a Cbc, you can choose the config file you will load to its
register map.
If you want to add your personal config file, please make sure to add it in #define
in the HWDescription/Description.h and then to add an option in the MCP code.

After this creation round, you can do anything you want :
- Configure the Glib or the Cbcs
- Manipulate the registers in the Glib
- Manipulate the registers in the Cbcs
- Perform a DAQ and tracing diagrams counting the number of hits
- Have a configuration recap of all the objects you created

Concerning the manipulation of the Cbcs, you have the opportunity to modify all the
Cbcs of a same Module at once with the Broadcast feature of the Cbc.

When you write a register in the Glib or the Cbc, the writing is updated to the
map contained in Glib/Cbc objects so that you're always fully aware of what is
in the Glib/Cbc register.

For writing value in register, we invite you to put in the following format : 0x__.
You must thus type '0xFF' for exemple and not just 'FF' in the command line. That
might change in the future..

You can also run DAQ and recover the Hit Histogram for each Cbc and each Event in the output file.

At the end of the program, the register maps are saved into output_X_X.txt files
you can find in the settings directory. For example, output_1_2.txt contains the
register map of the Cbc 2 of the Module 1.
You can also find different .pdf files containing the histograms of the DAQ.

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
<br>
<br>
__Future Improvements__

- Make a System Manager class that is wrapping the new structure
- System Controller class


Support, Suggestions ?
----------------------

For any support/suggestions, mail Lorenzo Bidegain, Nicolas Pierre or Georg Auzinger.
