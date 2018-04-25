//
//  File: sdpdd.c
//

#define	 XSSD_GLOBALS	1

#include   "sdpdd.h"

/*
	GPIO Initialize  - P320
   	// MMCCLK	GPIO_22		���      	���� ��� 4
   	// MMCINS	GPIO_17		�Է�		���� ��� 0
   	// MMC_CD	GPIO_45		�Լ�		���� ��� 0
   	// MMCDAT0	GPIO_18		�����    	���� ��� 1
  	// MMCDAT1	GPIO_19		�����    	���� ��� 1
   	// MMCDAT2	GPIO_20		�����    	���� ��� 1
   	// MMCDAT3	GPIO_21		�����    	���� ��� 1
   	// MMCCMD	GPIO_23		�����    	���� ��� 1
*/   	

#define IO_GPIO22   	(1<<22)	// GPxR0, Bit22
#define IO_GPIO17   	(1<<17)	// GPxR0, Bit17
#define IO_GPIO45   	(1<<13)	// GPxR1, Bit13
#define IO_GPIO18   	(1<<18)	// GPxR0, Bit18
#define IO_GPIO19   	(1<<19)	// GPxR0, Bit19
#define IO_GPIO20	(1<<20)	// GPxR0, Bit20
#define IO_GPIO21 	(1<<21)	// GPxR0, Bit21
#define IO_GPIO23 	(1<<22)	// GPxR0, Bit23

#define MMCCLK		IO_GPIO22
#define MMCINS		IO_GPIO17
#define MMC_CD		IO_GPIO45
#define MMCDAT0 	IO_GPIO18
#define MMCDAT1		IO_GPIO19
#define MMCDAT2		IO_GPIO20
#define MMCDAT3		IO_GPIO21
#define MMCCMD		IO_GPIO23

#define CLKEN_MMC	(1<<12)	// MMC/SD Port Clock Enable

#define ADDR_OF_MMC_WP		0x08000000
#define MMC_WP	(1<<3)


static BOOL   g_1bit_4bit;

// SD/MMC �ʱ� ����
BOOL PDD_SDInitializeHardware()
{

	// Input/Out Mode Set..
	__REG(GPIO_BASE + GPCR1) |= MMCCLK;
	__REG(GPIO_BASE + GPDR1) |= MMCCLK;
	
	__REG(GPIO_BASE + GPSR1) |= MMC_CD;
	__REG(GPIO_BASE + GPDR1) &=  ~MMC_CD;
	
	__REG(GPIO_BASE + GPSR2) |= MMCDAT0;
	__REG(GPIO_BASE + GPDR2) |= MMCDAT0;
	__REG(GPIO_BASE + GPSR3) |= (MMCCMD | MMCDAT1 |MMCDAT2 |MMCDAT3 );
	__REG(GPIO_BASE + GPDR3) |= (MMCCMD | MMCDAT1 |MMCDAT2 |MMCDAT3 );

	// Alternate Function Set..	
	*(unsigned int *)GPIO22_MFPR = 0xC804;		// MMC_CLK
	*(unsigned int *)GPIO17_MFPR = 0xC800;		// MMC_INS
	*(unsigned int *)GPIO45_MFPR = 0xC800;		// MMC_CD
	*(unsigned int *)GPIO18_MFPR = 0xC804;		// MMC_DAT0
	*(unsigned int *)GPIO19_MFPR = 0xC804;		// MMC_DAT1
	*(unsigned int *)GPIO20_MFPR = 0xC804;		// MMC_DAT2	
	*(unsigned int *)GPIO21_MFPR = 0xC804;		// MMC_DAT3
	*(unsigned int *)GPIO23_MFPR = 0xC804;		// MMC_CMD


   	// Start MMC Clock Enable
  	CKENA = CKENA | CLKEN_MMC;
	
    	// ī�� ���� Ȯ�� ( MSINS[GP35] - Level Low )
	if(!(__REG(GPIO_BASE + GPLR1) & MMC_CD)){
		printf("INFO: MMC/SD Card insert..\n");
	}
	else{
		printf("ERROR : MMC/SD Card not insert ??\n");
		return FALSE;
	}

    	// �ʱ� 1��Ʈ ��(MDAT0)�� ����
    	g_1bit_4bit = 0;
		
   	// Ŭ�� �� ���� �ð� ���� 
   	__REG(MMC_BASE + MMC_CLKRT) 	= MMC_CLKRT_0_3125MHZ; /* Ŭ��	*/
	__REG(MMC_BASE + MMC_RESTO) 	= 64;					/* �ִ� ���� �ð� MMC_RES_TO_MAX */
	__REG(MMC_BASE + MMC_RDTO) 	= MMC_READ_TO_MAX;     	/* �ִ� �б� ���� �ð�  */
	__REG(MMC_BASE + MMC_BLKLEN)	= BLOCK_LEN;           		/* ��� ���� */
	__REG(MMC_BASE + MMC_NUMBLK)	= 1;                   			/* ��� ���� */
	__REG(MMC_BASE + MMC_SPI)		= MMC_SPI_DISABLE;     	/* SPI ��� �𽺿��̺� */

    	// ���� �ð� ����
	PDD_SDStallExecute(500);

    	// CMD0, 0, 0,0,0 ����
    	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_INIT) ) {
        	printf("\r\nERROR: PDD_SDInitializeHardware \n");
        	return FALSE;
    	}

    return TRUE;
}

// Ŀ�ǵ� ����
BOOL PDD_SDSendCommand(UINT8 cmd, UINT32 arg, UINT16 cmdat)
{

	// Stop the MMC clock
	__REG(MMC_BASE + MMC_STRPCL) = MMC_STRPCL_STOP_CLOCK;

	 /* Ŭ���� ������ ������ ��� */
	while(__REG(MMC_BASE + MMC_STAT) & MMC_STAT_CLOCK_ENABLED);

	// �Ű� ���� ��ֱ�
	if(g_1bit_4bit) cmdat |= MMC_CMDAT_SD_4DAT;         	/* 4��Ʈ ������ ���� */

	// �Ű�����, CMD, cmdat ��ֱ�
	__REG(MMC_BASE + MMC_ARGL) = (arg >> 0) & 0xFFFF;	/* ���� ���� */
	__REG(MMC_BASE + MMC_ARGH) = (arg >> 16) & 0xFFFF;	/* ���� ���� */
	__REG(MMC_BASE + MMC_CMD) = cmd;
	__REG(MMC_BASE + MMC_CMDAT) = cmdat;
	__REG(MMC_BASE + MMC_STRPCL) = MMC_STRPCL_START_CLOCK; /* MMC Clock start !! */

    	// Ŀ�ǵ� ������ ������ ���� ���
    	while(!(	__REG(MMC_BASE + MMC_STAT) & MMC_STAT_END_CMD_RES));

	// Ÿ��-�ƿ� �������� �˻�
    	if (__REG(MMC_BASE + MMC_STAT) & MMC_STAT_RESPONSE_TIMEOUT) {
			return FALSE;
	}
	return TRUE;		
}

// ������ ����
BOOL PDD_SDReceiveData(UINT8* pBuffer)
{
	UINT32 bCount,i;

	union{
		UINT8   dataByte[4];
		UINT32  dataLong;
	}data;

	bCount = 0;

	__REG(MMC_BASE + MMC_I_MASK) = ~MMC_IMASK_RXFIFO_REQ_INT_MASKED;
		
	while( bCount < BLOCK_LEN ) 
	{
		// ���� �˻�
		if( __REG(MMC_BASE + MMC_STAT) & MMC_STAT_READ_ERRORS ) {
			printf("\r\nERROR: During data transfer(stat = %x)\n", __REG(MMC_BASE + MMC_STAT));
			return FALSE;
		}
		
        	// ������ ����
		if(__REG(MMC_BASE + MMC_I_REG) & MMC_IREG_RXFIFO_REQ) {
			for(i=0; i<MMC_RXFIFO_SIZE; i +=4){
				data.dataLong = __REG(MMC_BASE + MMC_RXFIFO);   /* FIFO �б� */
				*pBuffer++ = data.dataByte[0];
				*pBuffer++ = data.dataByte[1];
				*pBuffer++ = data.dataByte[2];
				*pBuffer++ = data.dataByte[3];
			}
			bCount += MMC_RXFIFO_SIZE;
		}
	}
	
    	// ���ͷ�Ʈ ����ũ
	__REG(MMC_BASE + MMC_I_MASK) = MMC_IMASK_ALL_INTERRUPTS_MASKED;
		
	while(!(__REG(MMC_BASE + MMC_I_REG) & MMC_IREG_DATA_TRAN_DONE));      /* ������ ���� �� */

	return TRUE;
}

// ���� ����
UINT32 PDD_SDGetResponse(SD_RESPONSE whichResp)
{
	UINT32 response;

	switch( whichResp ) {
		case RCA_REGISTER:	            /* 16��Ʈ ���� */
			response = (__REG(MMC_BASE + MMC_RES) & 0x00FF) << 8;
			response |= (__REG(MMC_BASE + MMC_RES) & 0xFF00) >> 8;
			return response;

		case CARD_STATUS_REGISTER:
		case OCR_REGISTER:              /* 32��Ʈ ���� */
			XsMMC_OCR.db[2] = __REG(MMC_BASE + MMC_RES);
			XsMMC_OCR.db[1] = __REG(MMC_BASE + MMC_RES);
			XsMMC_OCR.db[0] = __REG(MMC_BASE + MMC_RES);
			return 0;

		case CSD_REGISTER:
			XsMMC_CSD.db[7] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[6] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[5] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[4] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[3] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[2] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[1] = __REG(MMC_BASE + MMC_RES);
			XsMMC_CSD.db[0] = __REG(MMC_BASE + MMC_RES);
			return 0;

		case SCR_REGISTER:             /*  64��Ʈ ����  */
		case CID_REGISTER:             /* 128��Ʈ ���� */
		default:
			return 0;                  /*  �������� ���� */
	}
}

// 
int PDD_SDSetPDDCapabilities(PDD_IOCTL whichAbility, UINT32 Ability)
{

	switch( whichAbility ) {
		case SET_MMC_MODE:
			g_1bit_4bit = FALSE;                /*  MMC 1�� ��� */
			break;

		case SET_4BIT_MODE:
			if( Ability ) 	g_1bit_4bit = TRUE;  /*  4�� ��� */
			else 	g_1bit_4bit = FALSE;        /*  1�� ��� */
			break;

		case SET_CLOCK_SD:
			__REG(MMC_BASE + MMC_CLKRT)  = MMC_CLKRT_20MHZ;
			break;

		case SET_CLOCK_MMC:
			__REG(MMC_BASE + MMC_CLKRT) = MMC_CLKRT_10MHZ;
			break;
		case GET_SUPPORTED_OCR_MMC:
			break;
		case GET_SUPPORTED_OCR_SD:
			break;
	}

	return 0;
}


void PDD_SDStallExecute(unsigned int waitMS)
{
	Wait(waitMS);
}



