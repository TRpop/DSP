//
//  Header: sdpdd.h
//
#ifndef __SDPDD_H
#define __SDPDD_H

#include <time.h>
#include <lib.h>
#include <serial.h>
#include <reg.h>
#include <types.h>

#define FALSE 0
#define TRUE  1

#define BLOCK_LEN  512
#define BLOCK_MASK (~(BLOCK_LEN-1))

#define MAXIMUM_SDCLOCK_FREQUENCY 20000000
#define NUM_BYTE_FOR_POLLING_MODE 0x800
#define TRANSFER_TIMEOUT_CONSTANT 10
#define TRANSFER_TIMEOUT_FACTOR 5

#define MMC_RXFIFO_SIZE 32
#define MMC_TXFIFO_SIZE 32

// MMC_RESTO, 응답을 기다리는 최대 클록 수 
#define MMC_RES_TO_MAX          (0x7F) // [6:0]
// MMC_RDTO, 데이터를 수신하기 위해서 기다리는 최대 클록 수  
#define MMC_READ_TO_MAX         (0xFFFF) // [15:0]
// MMC_BLKLEN, 데이터 블록의 바이트 수 
#define MMC_BLK_LEN_MAX         (0x0FFF) // [11:0]
// MMC_PRTBUF, MMC_TXFIFO가 부분적으로 써질때 사용 
#define MMC_PRTBUF_BUF_PART_FULL  1      // 버퍼는 부분적으로 풀
#define MMC_PRTBUF_BUF_FULL		  0      // 버퍼는 부분적으로 풀이 아니다. 
// bit definitions for MMC_STRPCL
#define MMC_STRPCL_START_CLOCK    0x02
#define MMC_STRPCL_STOP_CLOCK     0x01

// bit definitions for MMC_IMASK
#define MMC_IMASK_DATA_TRAN_DONE_INT_MASKED   (1 << 0)
#define MMC_IMASK_PROG_DONE_INT_MASKED        (1 << 1)
#define MMC_IMASK_END_CMD_INT_MASKED          (1 << 2)
#define MMC_IMASK_STOP_CMD_INT_MASKED         (1 << 3)
#define MMC_IMASK_CLOCK_OFF_INT_MASKED        (1 << 4)
#define MMC_IMASK_RXFIFO_REQ_INT_MASKED       (1 << 5)
#define MMC_IMASK_TXFIFO_REQ_INT_MASKED       (1 << 6)
#define MMC_IMASK_TRANSMIT_TIMEOUT_INT_MASKED (1 << 7)
#define MMC_IMASK_DATA_ERROR_INT_MASKED       (1 << 8)
#define MMC_IMASK_RESPONSE_ERROR_INT_MASKED   (1 << 9)
#define MMC_IMASK_STALLED_READ_INT_MASKED     (1 << 10)
#define MMC_IMASK_SDIO_INT_MASKED             (1 << 11)
#define MMC_IMASK_SDIO_SUSPEND_ACK_INT_MASKED (1 << 12)
#define MMC_IMASK_ALL_INTERRUPTS_MASKED      (MMC_IMASK_DATA_TRAN_DONE_INT_MASKED   | \
                                              MMC_IMASK_PROG_DONE_INT_MASKED        | \
                                              MMC_IMASK_END_CMD_INT_MASKED          | \
                                              MMC_IMASK_STOP_CMD_INT_MASKED         | \
                                              MMC_IMASK_CLOCK_OFF_INT_MASKED        | \
                                              MMC_IMASK_RXFIFO_REQ_INT_MASKED       | \
                                              MMC_IMASK_TXFIFO_REQ_INT_MASKED)      | \
                                              MMC_IMASK_TRANSMIT_TIMEOUT_INT_MASKED | \
                                              MMC_IMASK_DATA_ERROR_INT_MASKED       | \
                                              MMC_IMASK_RESPONSE_ERROR_INT_MASKED   | \
                                              MMC_IMASK_STALLED_READ_INT_MASKED     | \
                                              MMC_IMASK_SDIO_INT_MASKED             | \
                                              MMC_IMASK_SDIO_SUSPEND_ACK_INT_MASKED

    // bit definitions for MMC_IREG
#define MMC_IREG_DATA_TRAN_DONE       	(1 << 0)
#define MMC_IREG_PROG_DONE                  	(1 << 1)
#define MMC_IREG_END_CMD                    	(1 << 2)
#define MMC_IREG_STOP_CMD                   	(1 << 3)
#define MMC_IREG_CLOCK_IS_OFF               	(1 << 4)
#define MMC_IREG_RXFIFO_REQ                 	(1 << 5)
#define MMC_IREG_TXFIFO_REQ                 	(1 << 6)
#define MMC_IREG_TINT                      		(1 << 7)
#define MMC_IREG_DAT_ERR                    	(1 << 8)
#define MMC_IREG_RES_ERR                    	(1 << 9)
#define MMC_IREG_RD_STALLED                 	(1 << 10)
#define MMC_IREG_SDIO_INT                   	(1 << 11)
#define MMC_IREG_SDIO_SUSPEND_ACK    	(1 << 12)
#define MMC_IREG_INTERRUPTS                 	0x1FFF

// MMC_CLKRT, 클록 결정 
#define MMC_CLKRT_20MHZ         	0
#define MMC_CLKRT_10MHZ         	1
#define MMC_CLKRT_5MHZ          	2
#define MMC_CLKRT_2_5MHZ        	3
#define MMC_CLKRT_1_25MHZ       	4
#define MMC_CLKRT_0_625MHZ     	5
#define MMC_CLKRT_0_3125MHZ    	6

// MMC_SPI, SPI 전송 제어 
#define MMC_SPI_CS_ADDRESS      	(1 << 3)   //'1':CS0 인에이블, '0':CS1 인에이블 
#define MMC_SPI_CS_EN           	(1 << 2)   //'1':CS0, CS1 인에이블 
#define MMC_SPI_CRC_ON          	(1 << 1)   //'1':CRC 인에이블, 베리파이 
#define MMC_SPI_DISABLE         	0          // SPI 디스에이블
#define MMC_SPI_EN              		1          // SPI 인에이블 

// MMC_CMDAT 레지스터의 비트 정의
#define MMC_CMDAT_RESPONSE_NONE 	0x00        // no response
#define MMC_CMDAT_RESPONSE_R1   	0x01        // expected R1 response
#define MMC_CMDAT_RESPONSE_R2   	0x02        // expected R2 response
#define MMC_CMDAT_RESPONSE_R3   	0x03        // expected R3 response
#define MMC_CMDAT_DATA_EN       	(1 << 2)    // data transfer to follow
#define MMC_CMDAT_DATA_WRITE    	(1 << 3)    // data transfer is a write
#define MMC_CMDAT_STREAM        		(1 << 4)    // data transfer is stream mode
#define MMC_CMDAT_EXPECT_BUSY   	(1 << 5)    // the command uses busy signalling
#define MMC_CMDAT_INIT          		(1 << 6)    // add init clocks
#define MMC_CMDAT_DMA_ENABLE    	(1 << 7)    // enable DMA
#define MMC_CMDAT_SD_4DAT       		(1 << 8)    // enable 4 bit data transfers
#define MMC_CMDAT_STOP_TRAN     	(1 << 10)   // stop data transmission
#define MMC_CMDAT_SDIO_INT_EN   	(1 << 11)   // enable controller to check for an SDIO interrupt from the card
#define MMC_CMDAT_SDIO_SUSPEND  	(1 << 12)   // SDIO CMD 52, suspend current data transfer
#define MMC_CMDAT_SDIO_RESUME   	(1 << 13)   // SDIO CMD 52, resume a suspended data transfer

// MMC_STAT 레지스터의 비트 정의
#define MMC_STAT_READ_TIMEOUT          		(1 << 0)
#define MMC_STAT_RESPONSE_TIMEOUT      	(1 << 1)
#define MMC_STAT_WRITE_DATA_CRC_ERROR  	(1 << 2)
#define MMC_STAT_READ_DATA_CRC_ERROR   	(1 << 3)
#define MMC_STAT_SPI_READ_TOKEN_ERROR  	(1 << 4)
#define MMC_STAT_RESPONSE_CRC_ERROR    	(1 << 5)
#define MMC_STAT_CLOCK_ENABLED        	 	(1 << 8)
#define MMC_STAT_FLASH_ERROR           		(1 << 9)
#define MMC_STAT_SPI_WR_ERROR          		(1 << 10)
#define MMC_STAT_DATA_TRANSFER_DONE    	(1 << 11)
#define MMC_STAT_PROGRAM_DONE          		(1 << 12)
#define MMC_STAT_END_CMD_RES           		(1 << 13)
#define MMC_STAT_RD_STALLED            		(1 << 14)
#define MMC_STAT_SDIO_INT              			(1 << 15)
#define MMC_STAT_SDIO_SUSPEND_ACK      	(1 << 16)

// 모든 에러 정의 
#define MMC_STAT_ERRORS (MMC_STAT_RESPONSE_CRC_ERROR|MMC_STAT_SPI_READ_TOKEN_ERROR\
        |MMC_STAT_READ_DATA_CRC_ERROR | MMC_STAT_RESPONSE_TIMEOUT\
        |MMC_STAT_READ_TIMEOUT | MMC_STAT_WRITE_DATA_CRC_ERROR)

#define MMC_STAT_READ_ERRORS (MMC_STAT_READ_DATA_CRC_ERROR\
       | MMC_STAT_RESPONSE_TIMEOUT | MMC_STAT_READ_TIMEOUT)
       
// MMC_PRTBUF 레지스터의 비트 정의
#define MMC_PRTBUF_BUFFER_PARTIAL_FULL (1 << 0)

//////////////////////////////////////////////////
// 커맨드 타입
//----------------------------------------------------------------------------------
typedef enum {
	CMD_TYPE_BC   = 0,	// Broadcast Command no response (to all cards)
	CMD_TYPE_BCR  = 1,	// Broadcast Command w/ response (to all cards)
	CMD_TYPE_AC   = 2,	// Addressed Command no data 	 (to selected card)
	CMD_TYPE_ADTC = 3	// Addressed Command w/ data	 (to selected card)
} CMD_TYPE;


//----------------------------------------------------------------------------------
//
//		         			LIST OF RESPONSE TYPES
//
//----------------------------------------------------------------------------------

typedef enum {
	RESP_TYPE_NONE = 0,	// No Response
	RESP_TYPE_R1   = 1,	// Normal Response 
	RESP_TYPE_R1B  = 9,	// Normal Response with Busy
	RESP_TYPE_R2   = 2,	// CID/CSD Register Response (the long response)
	RESP_TYPE_R3   = 3,	// OCR (Operating condition) register
	RESP_TYPE_R4   = 4,	// Fast I/O Response			(MMC only)
	RESP_TYPE_R5   = 5,	// Interrupt Request Response		(MMC only)
	RESP_TYPE_R6   = 6,	// Published RCA Response		(SD only)
} RESP_TYPE;

//----------------------------------------------------------------------------------
//
//					      		 CARD REGISTERS
//
//----------------------------------------------------------------------------------

typedef enum {
	CARD_STATUS_REGISTER,
	OCR_REGISTER,	// OCR: Operating Conditions 
 	CID_REGISTER,   // CID: Card IDentification
	CSD_REGISTER,	// CSD: Card-Specific Data
	RCA_REGISTER,   // RCA: Relative Card Address
	SCR_REGISTER,   // SCR: SD-card Configuration (SD only)
} SD_RESPONSE;

#define CMD_START_BIT      0x40
#define APPL_CMD_BIT       0x100   // apply to SD  only
#define SECURITY_CMD_BIT   0x8000

/* ------------------------- CMD's names: ---------------------------- */
#define GO_IDLE_STATE            	0x0 
#define SEND_OP_COND             	0x1
#define ALL_SEND_CID             	0x2
#define SET_RELATIVE_ADDR        	0x3   // R1 응답
#define SEND_RELATIVE_ADDR       0x3   // R6 응답
#define SET_DSR                  		0x4
#define SELECT_DESELECT_CARD  	0x7
#define SEND_CSD                 		0x9
#define SEND_CID                 		0xa
#define READ_DAT_UNTIL_STOP   	0xb
#define STOP_TRANSMISSION        	0xc
#define SEND_STATUS         		0xd
#define SET_BUS_WIDTH_REGISTER   0xe
#define GO_INACTIVE_STATE        	0xf
#define SET_BLOCKLEN             	0x10
#define READ_SINGLE_BLOCK        	0x11
#define READ_MULTIPLE_BLOCK      0x12
#define WRITE_DAT_UNTIL_STOP   0x14
#define WRITE_BLOCK              		0x18
#define WRITE_MULTIPLE_BLOCK   	0x19
#define PROGRAM_CID              	0x1a
#define PROGRAM_CSD              	0x1b
#define SET_WRITE_PROT           	0x1c
#define CLR_WRITE_PROT           	0x1d
#define SEND_WRITE_PROT          	0x1e
#define TAG_SECTOR_START         	0x20
#define TAG_SECTOR_END           	0x21
#define UNTAG_SECTOR             	0x22
#define TAG_ERASE_GROUP_START    0x23
#define TAG_ERASE_GROUP_END 	0x24
#define UNTAG_ERASE_GROUP     	0x25
#define ERASE_SECTORS            	0x26
#define CRC_ON_OFF               		0x3b

#define APPL_CMD                 		55// + CMD_START_BIT + APPL_CMD_BIT)

/* ONLY for SD card in MMC mode  */
#define SET_BUS_WIDTH            	6  //  + CMD_START_BIT + APPL_CMD_BIT)

#define SD_STATUS                		13 // + CMD_START_BIT + APPL_CMD_BIT)
#define SD_NUM_WR_SECTORS        22 //+ CMD_START_BIT + APPL_CMD_BIT)
#define SD_SECTOR_ERASE_COUNT    23 //+ CMD_START_BIT + APPL_CMD_BIT)
#define SD_APP_OP_COND           	41 //+ CMD_START_BIT + APPL_CMD_BIT)
#define SD_SET_CLR_DETECT        	42 //+ CMD_START_BIT + APPL_CMD_BIT)
#define SD_SEND_SCR              		51 //+ CMD_START_BIT + APPL_CMD_BIT)

//----------------------------------------------------------------------------------
//
//							CARD STATUS REGISTER BITS
//
//----------------------------------------------------------------------------------

#define CSTAT_OUT_OF_RANGE		0x80000000
#define CSTAT_ADDRESS_ERROR		0x40000000
#define CSTAT_BLOCK_LEN_ERROR		0x20000000
#define CSTAT_ERASE_SEQ_ERROR		0x10000000
#define CSTAT_ERASE_PARAM			0x08000000
#define CSTAT_WP_VIOLATION		0x04000000
#define CSTAT_CARD_IS_LOCKED		0x02000000
#define CSTAT_LOCK_UNLOCK_FAILED	0x01000000
#define CSTAT_COM_CRC_ERROR		0x00800000
#define CSTAT_ILLEGAL_COMMAND		0x00400000
#define CSTAT_CARD_ECC_FAILED		0x00200000
#define CSTAT_CC_ERROR				0x00100000
#define CSTAT_ERROR					0x00080000
#define CSTAT_UNDERRUN				0x00040000
#define CSTAT_OVERRUN				0x00020000
#define CSTAT_CIDCSD_OVERWRITE	0x00010000
#define CSTAT_WP_ERASE_SKIP		0x00008000
#define CSTAT_CARD_ECC_DISABLED	0x00004000
#define CSTAT_CARD_ERASE_RESET	0x00002000
#define CSTAT_CURRENT_STATE		0x00001E00
#define CSTAT_READY_FOR_DATA		0x00000100
#define CSTAT_APP_CMD				0x00000020
#define CSTAT_AKE_SEQ_ERROR		0x00000008


//----------------------------------------------------------------------------------
//
//					OPERATING CONDITIONS (OCR) REGISTER BITS
//
//----------------------------------------------------------------------------------

#define OCR_POWER_UP_BUSY			0x80
#define OCR_35_36_V					(1 << 23)
#define OCR_34_35_V					(1 << 22)
#define OCR_33_34_V					(1 << 21)
#define OCR_32_33_V					(1 << 20)
#define OCR_31_32_V					(1 << 19)
#define OCR_30_31_V					(1 << 18)
#define OCR_29_30_V					(1 << 17)
#define OCR_28_29_V					(1 << 16)
#define OCR_27_28_V					(1 << 15)
#define OCR_26_27_V					(1 << 14)
#define OCR_25_26_V					(1 << 13)
#define OCR_24_25_V					(1 << 12)
#define OCR_23_24_V					(1 << 11)
#define OCR_22_23_V					(1 << 10)
#define OCR_21_22_V					(1 << 9)
#define OCR_20_21_V					(1 << 8)
#define OCR_19_20_V					(1 << 7)
#define OCR_18_19_V					(1 << 6)
#define OCR_17_18_V					(1 << 5)
#define OCR_16_17_V					(1 << 4)


//----------------------------------------------------------------------------------
//
//					LIST OF SUPPORTED CAPABLILITIES
//
//----------------------------------------------------------------------------------

typedef enum {
	SET_MMC_MODE			= 1,
	SET_4BIT_MODE			= 2,
	SET_CLOCK_SD			= 3,
	SET_CLOCK_MMC			= 4,
	GET_SUPPORTED_OCR_MMC	= 5,
	GET_SUPPORTED_OCR_SD	= 6,
} PDD_IOCTL;

////////////////////////////
#undef EXTERN
#ifdef XSSD_GLOBALS
#define EXTERN
#else
#define EXTERN extern
#endif // XSSD_GLOBALS
EXTERN union {
    UINT8 b[16];
    UINT16 db[8];
    struct {
        unsigned ECC : 2;               // Bits 0-31
        unsigned RES1 : 2;
        unsigned TMP_WRITE_PROTECT : 1;
        unsigned PERM_WRITE_PROTECT : 1;
        unsigned COPY : 1;
        unsigned RES2 : 1;
        unsigned RES3 : 5;
        unsigned WRITE_BL_PARTIAL : 1;
        unsigned WRITE_BL_LEN : 4;
        unsigned R2W_FACTOR : 3;
        unsigned DEFAULT_ECC : 2;
        unsigned WP_GRP_ENABLE : 1;
        unsigned WP_GRP_SIZE : 5;
        unsigned ERASE_GRP_MULT : 3;

        unsigned ERASE_GRP_MULTx : 2;   // Bits 32-63
        unsigned ERASE_GRP_SIZE : 5;
        unsigned C_SIZE_MULT : 3;
        unsigned VDD_W_CURR_MAX : 3;
        unsigned VDD_W_CURR_MIN : 3;
        unsigned VDD_R_CURR_MAX : 3;
        unsigned VDD_R_CURR_MIN : 3;
        unsigned C_SIZE : 10;

        unsigned C_SIZEx : 2;           // Bits 64-95
        unsigned RES4 : 2;
        unsigned DSR_IMP : 1;
        unsigned READ_BLK_MISALIGN : 1;
        unsigned WRITE_BLK_MISALIGN : 1;
        unsigned READ_BL_PARTIAL : 1;
        unsigned READ_BL_LEN : 4;
        unsigned CCC : 12;
        unsigned TRAN_SPEED : 8;

        unsigned NSAC : 8;              // Bits 96-127
        unsigned TAAC : 8;
        unsigned RES5 : 2;
        unsigned SPEC_VERS : 4;
        unsigned CSD_STRUCTURE : 2;
        unsigned RES6 : 8;
    } CSD;
} XsMMC_CID, XsMMC_CSD;

EXTERN union {
    UINT8 b[6];
    UINT16 db[3];
} XsMMC_OCR;
#undef EXTERN

//----------------------------------------------------------------------------------
//
//					LIST OF MDD<==>PDD INTERFACE
//
//----------------------------------------------------------------------------------
BOOL   PDD_SDInitializeHardware();
BOOL PDD_SDSendCommand(UINT8 cmd, UINT32 arg, UINT16 cmdat);
UINT32 PDD_SDGetResponse(SD_RESPONSE whichResp);
int   PDD_SDSetPDDCapabilities(PDD_IOCTL whichAbility, UINT32 Ability);
BOOL   PDD_SDReceiveData(UINT8* pBuffer);
void PDD_SDStallExecute(unsigned int waitMS);

#endif

