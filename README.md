Git Repository for PH2DAQ HW Interface API & HW Description API


Supposed to contain:

- A middleware API layer, implemented in C++, which will basically wrap 
to abstracted functions the firmware calls and handshakes currently 
hardcoded into DAQ systems software.
- A C++ object-based library describing the system components (CBCs, 
Hybrids, Boards) and their properties(values, status)
- The standard XDAQ-based system, which could really benefit in its 
future versions of the previous two proposed APIs.