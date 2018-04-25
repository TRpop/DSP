//
//  File: sdpdd.c
//

#define	 XSSD_GLOBALS	1

#include   "sdpdd.h"

/*
	GPIO Initialize  - P320
   	// MMCCLK	GPIO_22		출력      	보조 기능 4
   	// MMCINS	GPIO_17		입력		보조 기능 0
   	// MMC_CD	GPIO_45		입펵		보조 기능 0
   	// MMCDAT0	GPIO_18		양방향    	보조 기능 1
  	// MMCDAT1	GPIO_19		양방향    	보조 기능 1
   	// MMCDAT2	GPIO_20		양방향    	보조 기능 1
   	// MMCDAT3	GPIO_21		양방향    	보조 기능 1
   	// MMCCMD	GPIO_23		양방향    	보조 기능 1
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

// SD/MMC 초기 설정
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
	
    	// 카드 검출 확인 ( MSINS[GP35] - Level Low )
	if(!(__REG(GPIO_BASE + GPLR1) & MMC_CD)){
		printf("INFO: MMC/SD Card insert..\n");
	}
	else{
		printf("ERROR : MMC/SD Card not insert ??\n");
		return FALSE;
	}

    	// 초기 1비트 핀(MDAT0)로 설정
    	g_1bit_4bit = 0;
		
   	// 클록 및 응답 시간 설정 
   	__REG(MMC_BASE + MMC_CLKRT) 	= MMC_CLKRT_0_3125MHZ; /* 클록	*/
	__REG(MMC_BASE + MMC_RESTO) 	= 64;					/* 최대 응답 시간 MMC_RES_TO_MAX */
	__REG(MMC_BASE + MMC_RDTO) 	= MMC_READ_TO_MAX;     	/* 최대 읽기 응답 시간  */
	__REG(MMC_BASE + MMC_BLKLEN)	= BLOCK_LEN;           		/* 블록 길이 */
	__REG(MMC_BASE + MMC_NUMBLK)	= 1;                   			/* 블록 개수 */
	__REG(MMC_BASE + MMC_SPI)		= MMC_SPI_DISABLE;     	/* SPI 모드 디스에이블 */

    	// 일정 시간 지연
	PDD_SDStallExecute(500);

    	// CMD0, 0, 0,0,0 전송
    	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_INIT) ) {
        	printf("\r\nERROR: PDD_SDInitializeHardware \n");
        	return FALSE;
    	}

    return TRUE;
}

// 커맨드 전송
BOOL PDD_SDSendCommand(UINT8 cmd, UINT32 arg, UINT16 cmdat)
{

	// Stop the MMC clock
	__REG(MMC_BASE + MMC_STRPCL) = MMC_STRPCL_STOP_CLOCK;

	 /* 클록이 정지될 때까지 대기 */
	while(__REG(MMC_BASE + MMC_STAT) & MMC_STAT_CLOCK_ENABLED);

	// 매개 변수 써넣기
	if(g_1bit_4bit) cmdat |= MMC_CMDAT_SD_4DAT;         	/* 4비트 데이터 전송 */

	// 매개변수, CMD, cmdat 써넣기
	__REG(MMC_BASE + MMC_ARGL) = (arg >> 0) & 0xFFFF;	/* 하위 워드 */
	__REG(MMC_BASE + MMC_ARGH) = (arg >> 16) & 0xFFFF;	/* 상위 위드 */
	__REG(MMC_BASE + MMC_CMD) = cmd;
	__REG(MMC_BASE + MMC_CMDAT) = cmdat;
	__REG(MMC_BASE + MMC_STRPCL) = MMC_STRPCL_START_CLOCK; /* MMC Clock start !! */

    	// 커맨드 전송이 끝날때 까지 대기
    	while(!(	__REG(MMC_BASE + MMC_STAT) & MMC_STAT_END_CMD_RES));

	// 타임-아웃 응답인지 검사
    	if (__REG(MMC_BASE + MMC_STAT) & MMC_STAT_RESPONSE_TIMEOUT) {
			return FALSE;
	}
	return TRUE;		
}

// 데이터 수신
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
		// 에러 검사
		if( __REG(MMC_BASE + MMC_STAT) & MMC_STAT_READ_ERRORS ) {
			printf("\r\nERROR: During data transfer(stat = %x)\n", __REG(MMC_BASE + MMC_STAT));
			return FALSE;
		}
		
        	// 데이터 수신
		if(__REG(MMC_BASE + MMC_I_REG) & MMC_IREG_RXFIFO_REQ) {
			for(i=0; i<MMC_RXFIFO_SIZE; i +=4){
				data.dataLong = __REG(MMC_BASE + MMC_RXFIFO);   /* FIFO 읽기 */
				*pBuffer++ = data.dataByte[0];
				*pBuffer++ = data.dataByte[1];
				*pBuffer++ = data.dataByte[2];
				*pBuffer++ = data.dataByte[3];
			}
			bCount += MMC_RXFIFO_SIZE;
		}
	}
	
    	// 인터럽트 마스크
	__REG(MMC_BASE + MMC_I_MASK) = MMC_IMASK_ALL_INTERRUPTS_MASKED;
		
	while(!(__REG(MMC_BASE + MMC_I_REG) & MMC_IREG_DATA_TRAN_DONE));      /* 데이터 전송 끝 */

	return TRUE;
}

// 응답 읽음
UINT32 PDD_SDGetResponse(SD_RESPONSE whichResp)
{
	UINT32 response;

	switch( whichResp ) {
		case RCA_REGISTER:	            /* 16비트 응답 */
			response = (__REG(MMC_BASE + MMC_RES) & 0x00FF) << 8;
			response |= (__REG(MMC_BASE + MMC_RES) & 0xFF00) >> 8;
			return response;

		case CARD_STATUS_REGISTER:
		case OCR_REGISTER:              /* 32비트 응답 */
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

		case SCR_REGISTER:             /*  64비트 응답  */
		case CID_REGISTER:             /* 128비트 응답 */
		default:
			return 0;                  /*  지원하지 않음 */
	}
}

// 
int PDD_SDSetPDDCapabilities(PDD_IOCTL whichAbility, UINT32 Ability)
{

	switch( whichAbility ) {
		case SET_MMC_MODE:
			g_1bit_4bit = FALSE;                /*  MMC 1핀 사용 */
			break;

		case SET_4BIT_MODE:
			if( Ability ) 	g_1bit_4bit = TRUE;  /*  4핀 사용 */
			else 	g_1bit_4bit = FALSE;        /*  1핀 사용 */
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



