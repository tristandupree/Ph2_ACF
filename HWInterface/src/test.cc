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

int main()
{
        int i;

	std::cout << "\nHurray, it's working !\n" << std::endl;

    Ph2_HwInterface::GLIBInterface MyGlibInterface("file:///opt/testing/trackerDAQ-3.2/CBCDAQ/GlibSupervisor/xml/connections.xml","board");

	std::cout << "\nConfiguration...\n" << std::endl;

	MyGlibInterface.ConfigureGLIB();

	std::cout << "\nConfiguration Complete !\n" << std::endl;

	std::cin >> i;

	std::cout << "\nStart...\n" << std::endl;

	MyGlibInterface.Start();

	std::cout << "\nStarted !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nPause...\n" << std::endl;

	MyGlibInterface.Pause();

        std::cout << "\nPaused !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nUnpause...\n" << std::endl;

	MyGlibInterface.Unpause();

        std::cout << "\nUnpaused !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nRead DAQ...\n" << std::endl;

	MyGlibInterface.ReadDAQ(4,false);

        std::cout << "\nDAQ read !\n" << std::endl;

        std::cin >> i;

	std::cout << "\nStop...\n" << std::endl;

	MyGlibInterface.Stop(10);

        std::cout << "\nStopped !\n" << std::endl;

        std::cin >> i;

        std::cout << "\nHurray, it's working !\n" << std::endl;
}
