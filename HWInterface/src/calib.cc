/*

	FileName : 					Calib.cc
	Content : 					 Sandbox testing w/r in user defined registers
	Programmer : 				Nicolas PIERRE
	Version : 					 1.0
	Date of creation : 	06/06/14
	Support : 					 mail to : nicolas.pierre@etu.unistra.net

*/
#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include "ICCalib/Calibrator.h"
#include "TApplication.h"
#include <iostream>
#include "CbcDaq/GUI.h"
#include "Interface/HWInterface.h"

int main()
{
	// Get the config file name
	std::string cConfigFileName = "settings/CbcCalibHole.txt";

	// Create the calibrator after the content of the config file
	ICCalib::Calibrator cDaq( cConfigFileName.c_str() );

	// Initialise the Glib for tests
	cDaq.Initialise();



	//Here begin the SandBox



	int i;
	std::string ans;
	uint32_t val;

	std::cout << "\nSandbox DAQ'14 v1.0\n" << std::endl;

	// Creation of Connection Manager via the HWInterface
	Interface::HWInterface MyInterface("file:///opt/testing/trackerDAQ-3.2/CBCDAQ/GlibSupervisor/xml/connections.xml","board");

	// Menu
	do
	{

	std::cout << "\n\nWhat would you like to do ?\n" << std::endl;

	std::cout << "1: Write a register\n" << std::endl;
	std::cout << "2: Read a register\n" << std::endl;
	std::cout << "3: Quit\n" << std::endl;

	std::cin >> i;

	switch(i)
	{
		// Write a Register
		case 1:
			std::cout << "\n\nRegister you wanna write ?" << std::endl;
			std::cout << "(Careful, some registers can't be modified)" << std::endl;
			std::cin >> ans; //Node of the register
			std::cout << "\nAnd what value ?" << std::endl;
			std::cin >> val; //Value you want to write

			// Test if the writing is correctly done
			if(MyInterface.WriteReg(ans,val))
			{
			   std::cout << "\nWriting successful !\n" << std::endl;
			}
			else
			{
			   std::cout <<	"\nWriting failed !\n" << std::endl;
			}

			// Print the written value
			MyInterface.ReadReg(ans);

			break;

		// Read a register
		case 2:
			std::cout << "\n\nRegister you wanna read ?" << std::endl;
			std::cin >> ans; //Node of the register

			MyInterface.ReadReg(ans);

			break;

		// Quit
		case 3:
			break;

		default:
			std::cout << "This option is not specified !\n\n\n\n\n" << std::endl;
			break;
	}

	} while(i!=3);

	return 0;
}
