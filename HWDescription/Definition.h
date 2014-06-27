/*

  FileName :                     Definition.h
  Content :                      Definition File, listing the registers
  Programmer :                   Nicolas PIERRE
  Version :                      0.3
  Date of creation :             07/06/14
  Support :                      mail to : nico.pierre@icloud.com

*/

//------------------------------------------------------------------------------
//uHal Connection File

#define UHAL_CONNECTION_FILE "file:///afs/cern.ch/user/n/npierre/public/settings/connections.xml"


//-----------------------------------------------------------------------------
//Glib Config Files

#define DEFAULT_GLIB_FILE  "settings/glib_settings.cfg"


//------------------------------------------------------------------------------
//Cbc Config Files

#define DEFAULT_FILE   "settings/default_file.txt"
#define FE0CBC0HOLE    "settings/FE0CBC0Hole.txt"
#define FE0CBC1        "settings/FE0CBC1.txt"
#define FE0CBC1HOLE    "settings/FE0CBC1Hole.txt"


//------------------------------------------------------------------------------
//I2C Parameters

#define I2C_CTRL_ENABLE   0x000009F4
#define I2C_CTRL_DISABLE  0
#define I2C_STROBE			  1
#define I2C_M16B				  0
#define I2C_MEM				    1
#define I2C_WRITE_ADDR	  0x09
#define I2C_READ_ADDR		  0x06

#define I2C_SLAVE			    0x5B
#define I2C_COMMAND			  "user_wb_ttc_fmc_regs.cbc_reg_i2c_command"
#define I2C_REPLY			    "user_wb_ttc_fmc_regs.cbc_reg_i2c_reply"
#define I2C_SETTINGS             "user_wb_ttc_fmc_regs.cbc_reg_i2c_settings"

#define MAX_NB_LOOP			  50


//------------------------------------------------------------------------------
//GLIB Useful Registers

//SRAM

    //SRAM IDs
#define SRAM1             "sram1"
#define SRAM2             "sram2"

    //SRAM user logic
#define SRAM1_USR_LOGIC   "ctrl_sram.sram1_user_logic"
#define SRAM2_USR_LOGIC   "ctrl_sram.sram2_user_logic"

    //SRAM End Readout
#define SRAM1_END_READOUT "user_wb_ttc_fmc_regs.pc_commands.SRAM1_end_readout"
#define SRAM2_END_READOUT "user_wb_ttc_fmc_regs.pc_commands.SRAM2_end_readout"

    //SRAM Full flags, reset when their end readout is 1
#define SRAM1_FULL        "user_wb_ttc_fmc_regs.flags.SRAM1_full"
#define SRAM2_FULL        "user_wb_ttc_fmc_regs.flags.SRAM2_full"

//GLIBS settings
#define FAKE_DATA	  "user_wb_ttc_fmc_regs.pc_commands.CBC_DATA_GENE"
#define EXT_TRG		  "user_wb_ttc_fmc_regs.pc_commands.TRIGGER_SEL"

//------------------------------------------------------------------------------
//CBC

    //CBC expectation
#define CBC_EXPECTED         "CBC_expected"

    //CBC DATA PACKET NUMBER
#define CBC_PACKET_NB        "user_wb_ttc_fmc_regs.pc_commands.CBC_DATA_PACKET_NUMBER"

    //CBC TEST PULSE VALID
#define CBC_TEST_PULSE_VALID "COMMISSIONNING_MODE_CBC_TEST_PULSE_VALID"

    //CBC Data generator
#define CBC_DATA_GENE        "user_wb_ttc_fmc_regs.pc_commands.CBC_DATA_GENE"

    //CBC_TRIGGER_ONE_SHOT
#define CBC_TRIGGER_1SHOT    "user_wb_ttc_fmc_regs.cbc_acquisition.CBC_TRIGGER_ONE_SHOT"

    //CBC stubdata latency adjustement
#define CBC_STUB_LATENCY_FE1 "cbc_stubdata_latency_adjust_fe1"
#define CBC_STUB_LATENCY_FE2 "cbc_stubdata_latency_adjust_fe2"

    //CBC I2C command acknoledgement
#define CBC_I2C_CMD_ACK      "cbc_i2c_cmd_ack"

    //CBC I2C command request
#define CBC_I2C_CMD_RQ       "cbc_i2c_cmd_rq"

    //CBC reset
#define CBC_HARD_RESET       "cbc_hard_reset"
#define CBC_FAST_RESET       "cbc_fast_reset"

//------------------------------------------------------------------------------
//Delays

    //Delay after fast reset
#define DELAY_AF_FAST_RESET "COMMISSIONNING_MODE_DELAY_AFTER_FAST_RESET"

    //Delay after L1A
#define DELAY_AF_L1A        "COMMISSIONNING_MODE_DELAY_AFTER_L1A"

    //Delay after test pulse
#define DELAY_AF_TEST_PULSE "COMMISSIONNING_MODE_DELAY_AFTER_TEST_PULSE"

//------------------------------------------------------------------------------
//Triggers

    //Break Trigger
#define BREAK_TRIGGER    "break_trigger"

    //Internal Trigger freq
#define INT_TRIGGER_FREQ "user_wb_ttc_fmc_regs.pc_commands.INT_TRIGGER_FREQ"

    //Trigger Select
#define TRIGGER_SELECT   "user_wb_ttc_fmc_regs.pc_commands.TRIGGER_SEL"

//------------------------------------------------------------------------------
//Others

    //PC Config OK
#define PC_CONFIG_OK    "user_wb_ttc_fmc_regs.pc_commands.PC_config_ok"

    //Spurious frame
#define SPURIOUS_FRAME  "user_wb_ttc_fmc_regs.pc_commands.SPURIOUS_FRAME"

    //Force BG0 Start
#define FORCE_BG0_START "user_wb_ttc_fmc_regs.pc_commands2.force_BG0_start"

    //CMD_START_VALID Flag
#define CMD_START_VALID "user_wb_ttc_fmc_regs.status_flags.CMD_START_VALID"

    //FE expectation
#define FE_EXPECTED     "FE_expected"

    //RQ
#define RQ              "COMMISSIONNING_MODE_RQ"

    //Acquisition mode
#define ACQ_MODE        "user_wb_ttc_fmc_regs.pc_commands.ACQ_MODE"

    //Clock shift
#define CLOCK_SHIFT     "user_wb_ttc_fmc_regs.pc_commands2.clock_shift"

    //Negative logic
#define NEG_LOGIC_CBC   "user_wb_ttc_fmc_regs.pc_commands2.negative_logic_CBC"
#define NEG_LOGIC_STTS  "user_wb_ttc_fmc_regs.pc_commands2.negative_logic_sTTS"

    //Polarity
#define POLARITY        "user_wb_ttc_fmc_regs.pc_commands2.polarity_tlu"

    //Event number
#define EVENT_NUMBER     100

    //Packet size
#define PACKET_SIZE      32

//------------------------------------------------------------------------------
