Git Repository for PH2_DAQ Software
===================================

__Supposed to contain__

- A middleware API layer, implemented in C++, which will basically wrap
to abstracted functions the firmware calls and handshakes currently
hardcoded into DAQ systems software

- A C++ object-based library describing the system components (CBCs,
Hybrids, Boards) and their properties(values, status)

- The MCP test program that is the wrapping the previous two


Different versions for different setups
---------------------------------------

On this GitHub, you can find different version of the software :
- A 2CBC version on the Master and the 2CBC branches
- A 8CBC version on the 8CBC branch
- An agnostic (to the number of CBCs) version with the new structure still in development in the Dev branch
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__What are the differences between the 2CBC/8CBC versions ?__

The differences mainly resides in the size of the data buffer for the DAQ, when all the access to both Board and Cbc registers is done the same way.
Also, some functions are present in 8CBC and not in 2CBC due to the fact that the firmware of the 8CBC is offering more possibilities of recovering infos from the Hardware (as the type of hardware for example)


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

4. Do a make in the root the repo (make sure you have all uHal, root, boost... libraries on your computer)

5. Launch testpgrm command if you want to test if everything is working good

6. Launch mcp to play with the Test Interface
<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__What can you do with the software ?__

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
- Do the manipulation to Start/Stop an acquisition, but not actually acquiring something relevant
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

More features will be implemented later, so make sure to have the last release
locally to benefit from them.

Warning ! : be careful with options choice in the program menus, some mistypes can leed
to unexpected hazards :-(.
  
  
On the go...
------------
  
__Last Updates__

- 09/07/14 : Added threading for stack writing registers
- 10/07/14 : Read Data from acquisition in a rubbish format
- ~~25/07/14 : Fully functional for 2CBC (safe from Broadcast obviously), pending for 8CBC~~
- ~~28/07/14 : Found a bug in the reading of CBC1 of 2CBC, trying to see if coming from soft or hard~~
- 30/07/14 : Working 2CBC version, find a 8CBC working version in the 8CBC branch
<br>
<br>
__Future Improvements__

- Finish the implementation of the new structure
- Make a System Manager class that is wrapping the new structure


Support, Suggestions ?
----------------------

For any support/suggestions, mail Lorenzo Bidegain, Nicolas Pierre or Georg Auzinger.
