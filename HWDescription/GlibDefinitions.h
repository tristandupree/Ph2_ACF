#ifndef _GlibDefinitions_h__
#define _GlibDefinitions_h__

namespace Ph2_HwDescription{

	// These are examples copied from Kirika
	// I expect this list to grow as soon as we get more familiar with the GLIB FW Registers!
	#define I2C_CTRL_ENABLE 	0x000009F4
	#define I2C_CTRL_DISABLE	0
	#define I2C_STROBE			1
	#define I2C_M16B			0
	#define I2C_MEM				1
	#define I2C_WRITE_ADDR		0x09
	#define I2C_READ_ADDR		0x06

	#define I2C_SLAVE			0x5B
	#define I2C_COMMAND			"user_wb_ttc_fmc_regs.cbc_reg_i2c_command"
	#define I2C_REPLY			"user_wb_ttc_fmc_regs.cbc_reg_i2c_reply"

	#define MAX_NB_LOOP			50	

	const unsigned int BeController::fPacketSize = Strasbourg::Event::EVENT_SIZE_32;//single event data size in 32 bits words
	const std::string BeController::fStrI2cSettings = "user_wb_ttc_fmc_regs.cbc_reg_i2c_settings";
	const std::string BeController::fStrI2cCommand = I2C_COMMAND;
	const std::string BeController::fStrI2cReply = I2C_REPLY;
	const uint32_t BeController::fI2cSlave = I2C_SLAVE;
	const unsigned int BeController::fI2cSramSize = 256; 
}


#endif