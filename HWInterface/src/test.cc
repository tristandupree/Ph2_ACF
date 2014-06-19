/*

  FileName :                                    Test.cc
  Content :                                      Proto for testing
  Programmer :                            Nicolas PIERRE
  Version :                                      0.0
  Date of creation :            12/06/14
  Support :                                      mail to : nicolas.pierre@etu.unistra.net

*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "Interface/RegManager.h"
#include "Interface/GLIBInterface.h"
#include "../../HWDescription/Description/Glib.h"

int main()
{
        int i;
	Glib cGlib;

	std::cout << "\nHurray, it's working !\n" << std::endl;

    Ph2_HwInterface::GlibInterface MyGlibInterface("file:///opt/testing/trackerDAQ-3.2/CBCDAQ/GlibSupervisor/xml/connections.xml");

	std::cout << "\nConfiguration...\n" << std::endl;

	MyGlibInterface.ConfigureGlib(cGlib);

	std::cout << "\nConfiguration Complete !\n" << std::endl;

	std::cin >> i;

	std::cout << "\nStart...\n" << std::endl;

	MyGlibInterface.Start(cGlib);

	std::cout << "\nStarted !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nPause...\n" << std::endl;

	MyGlibInterface.Pause(cGlib);

        std::cout << "\nPaused !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nUnpause...\n" << std::endl;

	MyGlibInterface.Unpause(cGlib);

        std::cout << "\nUnpaused !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nRead DAQ...\n" << std::endl;

	MyGlibInterface.ReadData(cGlib,4,false);

        std::cout << "\nDAQ read !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nStop...\n" << std::endl;

	MyGlibInterface.Stop(cGlib,10);

        std::cout << "\nStopped !\n" << std::endl;

        std::cin >> i;

        std::cout << "\nHurray, it's working !\n" << std::endl;
}
