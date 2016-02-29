#ifndef _MYFREHAL_H_
#define	_MYFREHAL_H_

// �Ĵ�����ַ����
#define PN512_PAGE_REG           0x00   // Ĭ��ֵΪ0x00 Selects the register page
#define PN512_COMMAND_REG        0x01   // Ĭ��ֵΪ0x20 Starts and stops command execution
#define PN512_COMMIEN_REG        0x02   // Ĭ��ֵΪ0x80 Controls bits to enable and disable the passing of interrupt requests
#define PN512_DIVIEN_REG         0x03   // Ĭ��ֵΪ0x00 Controls bits to enable and disable the passing of interrupt requests
#define PN512_COMMIRQ_REG        0x04   // Ĭ��ֵΪ0x14 Contains Interrupt Request bits
#define PN512_DIVIRQ_REG         0x05   // Ĭ��ֵΪ0x00 Contains Interrupt Request bits
#define PN512_ERROR_REG          0x06   // Ĭ��ֵΪ0x00 Error bits showing the error status of the last command executed
#define PN512_STATUS1_REG        0x07   // Ĭ��ֵΪ0x42 Contains status bits for communication
#define PN512_STATUS2_REG        0x08   // Ĭ��ֵΪ0x00 Contains status bits of the receiver and transmitter
#define PN512_FIFODATA_REG       0x09   // Ĭ��ֵΪ��ȷ��In- and output of 64 byte FIFO buffer
#define PN512_FIFOLEVEL_REG      0x0A   // Ĭ��ֵΪ0x00 Indicates the number of bytes stored in the FIFO
#define PN512_WATERLEVEL_REG     0x0B   // Ĭ��ֵΪ0x08 Defines the level for FIFO under- and overflow warning
#define PN512_CONTROL_REG        0x0C   // Ĭ��ֵΪ0x00 Contains miscellaneous Control Register
#define PN512_BITFRAMING_REG     0x0D   // Ĭ��ֵΪ0x00 Adjustments for bit oriented frames
#define PN512_COLL_REG           0x0E   // Ĭ��ֵΪ101xxxxx Bit position fo the first bit collision detected on the RF-interface

#define PN512_MODE_REG           0x11   // Ĭ��ֵΪ0x3B Defines general modes for the transmitting and receiving
#define PN512_TXMODE_REG         0x12   // Ĭ��ֵΪ0x00 Defines the data rate and framing during transmission
#define PN512_RXMODE_REG         0x13   // Ĭ��ֵΪ0x00 Defines the data rate and framing during receiving
#define PN512_TXCONTROL_REG      0x14   // Ĭ��ֵΪ0x80 Controls the logical behavior of the antenna driver pins TX1 and TX2
#define PN512_TXAUTO_REG         0x15   // Ĭ��ֵΪ0x00 Controls the setting of the antenna drivers
#define PN512_TXSEL_REG          0x16   // Ĭ��ֵΪ0x10 Selects the internal sources for the antenna driver
#define PN512_RXSEL_REG          0x17   // Ĭ��ֵΪ0x84 Selects internal receiver settings
#define PN512_RXTHRESHOLD_REG    0x18   // Ĭ��ֵΪ0x84 Selects thresholds for the bit decoder
#define PN512_DEMOD_REG          0x19   // Ĭ��ֵΪ0x4D Defines demodulator settings
#define PN512_FELNFC1_REG        0x1A   // Ĭ��ֵΪ0x00 Defines the length of the valid range for the receive package
#define PN512_FELNFC2_REG        0x1B   // Ĭ��ֵΪ0x00 Defines the length of the valid range for the receive package
#define PN512_MIFNFC_REG         0x1C   // Ĭ��ֵΪ0x62 Controls the communication in ISO 14443/Mifare and NFC target mode at 106 kbit
#define PN512_MANUALRCV_REG      0x1D   // Ĭ��ֵΪ0x00 Allows manual fine tuning of the internal receiver
#define PN512_TYPEB_REG          0x1E   // Ĭ��ֵΪ0x00 Configure the ISO 14443 type B
#define PN512_SERIALSPEED_REG    0x1F   // Ĭ��ֵΪ0xEB Selects the speed of the serial UART interface

#define PN512_CRCRESULTMSB_REG   0x21   // Ĭ��ֵΪ0xFF  Shows the actual MSB and LSB values of the CRC calculation
#define PN512_CRCRESULTLSB_REG   0x22   // Ĭ��ֵΪ0xFF
#define PN512_GSNOFF_REG         0x23   // Ĭ��ֵΪ0x88 Selects the conductance of the antenna driver pins TX1 and TX2 for modulation, when the driver is switched off
#define PN512_MODWIDTH_REG       0x24   // Ĭ��ֵΪ0x26 Controls the setting of the ModWidth
#define PN512_TXBITPHASE_REG     0x25   // Ĭ��ֵΪ0x87 Adjust the TX bit phase at 106 kbit
#define PN512_RFCFG_REG          0x26   // Ĭ��ֵΪ0x48 Configure the receiver gain and RF level
#define PN512_GSNON_REG          0x27   // Ĭ��ֵΪ0x88 Selects the conductance of the antenna driver pins TX1 and TX2 for modulation when the drivers are switched on
#define PN512_CWGSP_REG          0x28   // Ĭ��ֵΪ0x20 Selects the conductance of the antenna driver pins TX1 and TX2 for modulation during times of no modulation
#define PN512_MODGSP_REG         0x29   // Ĭ��ֵΪ0x20 Selects the conductance of the antenna driver pins TX1 and TX2 for modulation during modulation
#define PN512_TMODE_REG          0x2A   // Ĭ��ֵΪ0x00 Defines settings for the internal timers
#define PN512_TPRESCALER_REG     0x2B   // Ĭ��ֵΪ0x00
#define PN512_TRELOAD_H_REG      0x2C   // Describle the 16-bit timer reload value
#define PN512_TRELOAD_L_REG      0x2D
#define PN512_TCOUNTERVAL_H_REG  0x2E   // Shows the 16-bit actual timer value
#define PN512_TCOUNTERVAL_L_REG  0x2F

#define PN512_TESTSEL1_REG       0x31   // General test signal configuration
#define PN512_TESTSEL2_REG       0x32   // General test signal configuration and PRBS control
#define PN512_TESTPINEN_REG      0x33   // Enable pin output driver on 8-bit parallel bus
#define PN512_TESTPINVALUE_REG   0x34   // Defines the values for the 8-bit parallel bus when it is used as I/O bus
#define PN512_TESTBUS_REG        0x35   // Shows the status of the internal testbus
#define PN512_AUTOTEST_REG       0x36   // Controls the digital selftest
#define PN512_VERSION_REG        0x37   // Shows the version
#define PN512_ANALOGTEST_REG     0x38   // Controls the pins AUX1 and AUX2
#define PN512_TESTDAC1_REG       0x39   // Defines the test value for the TestDAC1
#define PN512_TESTDAC2_REG       0x3A   // Defines the test value for the TestDAC2
#define PN512_TESTADC_REG        0x3B   // Shows the actual value of ADC I and Q

/////////////////////////////////////////////////////////////////////////////////
//////////                      ��������                                /////////
/////////////////////////////////////////////////////////////////////////////////

//funtion for MifreCmd.c
int PN512_s_vRFReadReg (ulong ulLen, uchar ucRegAddr, uchar* pucData);
int PN512_s_vRFWriteReg (ulong ulLen, uchar ucRegAddr, uchar* pucData);
void PN512_sHal_EnCpuInt(void);//ʹ��CPU�ж�
void PN512_sHal_MaskCpuInt(void);//��ֹ����CPU�ж�

//funtion for Mifre.c
int PN512_s_RF_Init(void);
uchar PN512_sHal_Module_Disable(void);//RC531ģ���µ�
void PN512_s_PiccLight(unsigned char ucLedIndex, unsigned char ucOnOff);

//function for internal
void PN512_s_InitRFGPIO(void);
void PN512_s_vRFReset(void);
void PN512_s_vRFSwitchNCS(char chHigh);
int PN512_sPcdInit(void);
void PN512_s_InitRFSPI(void);
void PN512_s_RF_SPIProc(void);

#endif

