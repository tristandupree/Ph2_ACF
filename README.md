Git Repository for the ACF (Acquisition & Control Framework)
============================================================

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
- A 2CBC version on the 2CBC branch with the previous structure
- A 8CBC version on the 8CBC branch with the previous structure
- An in-progress version in the dev branch
<br>
<br>
__What are the differences between the 2CBC/8CBC versions ?__

The differences mainly resides in the size of the data buffer for the DAQ, when all the access to both Board and Cbc registers is done the same way.
Also, some functions are present in 8CBC and not in 2CBC due to the fact that the firmware of the 8CBC is offering more possibilities of recovering infos from the Hardware (as the type of hardware for example)


The Test Software itself : the MCP Test Interface
-------------------------------------------------

You'll find an install step by step and a How To.
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__Install the MCP Test Interface step by step...__

Here are the step to make the program functional
(Tested with the latest version of gcc, µHal, boost)

1. Recover the GitHub repo.

2. Create /lib, /bin and /output directories in the root of the repo.

3. Do a make in the root the repo (make sure you have all µHal, root, boost... libraries on your computer)

4. Launch testpgrm command if you want to test if everything is working good.

5. You can test the data acquisition by lauching datatest2cbc or datatest8cbc.

6. Launch mcp to play with the Test Interface
<br>
<br>
__What can you do with the software ?__

You can choose to load your Hardware description XML file and it will create the
 software counterpart of your existing hardware.

After this creation round, you can do anything you want :
- Configure the Glib or the Cbcs
- Manipulate the registers in the Glib
- Manipulate the registers in the Cbcs
- Perform a DAQ and tracing diagrams counting the number of hits
- Have a configuration recap of all the objects you created

When you write a register in the Glib or the Cbc, the writing is updated to the
map contained in Glib/Cbc objects so that you're always fully aware of what is
in the Glib/Cbc register.

For writing value in register, we invite you to put in the following format : 0x__.
You must thus type '0xFF' for exemple and not just 'FF' in the command line.

You can also run a DAQ round and recover the Histogram for each Cbc in the output folder.

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
