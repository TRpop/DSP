//
//  File: sdmdd.c
//
#define		XSSD_GLOBALS	1
#include   "sdmdd.h"
#include   "sdpdd.h"


#define HIGH_SHIFT(x) (((UINT32)(x))<<16)
// LIST OF CARD TYPES
typedef enum {
	CARD_TYPE_NONE,
	CARD_TYPE_MMC,
	CARD_TYPE_SD
} CARD_TYPE;

static UINT16	g_cardRCA;					// Relative Address of card

// SD/MMC �ʱ� ����
BOOL SDInitializeHardware()
{
	CARD_TYPE cardType = CARD_TYPE_NONE;
	int timeOut;
	g_cardRCA = 0;
	
	// SD/MMC �ʱ� ����
	if( !PDD_SDInitializeHardware() )    return FALSE;

	// ī�� Ÿ�� ����
	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_RESPONSE_NONE) ) {
		printf("\r\nERROR : GO_IDLE0 command returned error.\n");
		return FALSE;
	}
	
    	// ī�� Ŀ�ǵ� ����
	if( PDD_SDSendCommand(SEND_OP_COND,0,MMC_CMDAT_RESPONSE_R3) ) {
		cardType = CARD_TYPE_MMC;
	}
	else{
	   if( PDD_SDSendCommand(APPL_CMD,0,MMC_CMDAT_RESPONSE_R1)) {
			if( PDD_SDSendCommand(SD_APP_OP_COND,0x00FF8000, MMC_CMDAT_RESPONSE_R3)) {
				cardType = CARD_TYPE_SD;
			}
	   }
	}
	
	/* Debug Print */
	if(cardType == CARD_TYPE_MMC) printf("Card Type : MMC \n"); 
	   else if(cardType == CARD_TYPE_SD) printf("Card Type : SD \n"); 
		else printf("Card Type : NONE \n"); 
	/* Debug Print */
	
	
	// ī�� Ÿ���� �������� ���� ���
	if( cardType == CARD_TYPE_NONE ) {
		printf("\r\nERROR : No valid SD/MMC cards detected.\n");
		return FALSE;
	}

	// ------ ī�� �ʱ� ���� ------- //
	// ---- 1. CMD0�� �����ؼ� ����  ---- //
	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_RESPONSE_NONE) ) {
		printf("\r\nERROR : GO_IDLE0 command returned error.\n");
		return FALSE;
	}

	/* Debug Print */
	printf("Active : IDLE\n");
	
	// ---- 2. CMD1/ACMD41�� �̿��Ͽ� ���� ���� ---- //
	for( timeOut = 0; timeOut < 100; ++timeOut ) {
		if( cardType == CARD_TYPE_SD ) {
			if( PDD_SDSendCommand(APPL_CMD,0,MMC_CMDAT_RESPONSE_R1)) {
				if( !PDD_SDSendCommand( SD_APP_OP_COND, 0x00FF8000,MMC_CMDAT_RESPONSE_R3 )) {
					printf("\r\nERROR : Issuing SD_APP_OP_COND command.\n");
					return FALSE;
				}
			}
	} else {
		if( !PDD_SDSendCommand( SEND_OP_COND, 0x00FFC000,MMC_CMDAT_RESPONSE_R3 )) {
			printf("\r\nERROR : Issuing SEND_OP_COND command.\n");
			return FALSE;
		}
	}
		
	// ī�� ���� �ʱ� ���� �� �˻� 
	PDD_SDGetResponse(OCR_REGISTER);
       if(XsMMC_OCR.b[4] & OCR_POWER_UP_BUSY)   break;
		PDD_SDStallExecute(100);
	}
	
	// ī�� ���� �ʱ� ������ ���� ���� ���
	if( timeOut >= 100 ) {
		printf("ERROR : Failed to validate card OCR voltage range.\n");
		return FALSE;
	}
	
	// 3. ù��° ī�� ID(��Ƽ�� ī��� �������� �ʴ´�) ---- //
	for( timeOut = 0; timeOut < 30; ++timeOut ) {
		if( PDD_SDSendCommand(ALL_SEND_CID,0,MMC_CMDAT_RESPONSE_R2 ) )   break;
		PDD_SDStallExecute(100);  // �����ð� ����
	}
	
    	// CID ������ ���� ���
	if( timeOut >= 30 ) {
		printf("nERROR : Failed to receive valid CID response from card.\n");
		return FALSE;
	}
	
	// 4. ī�� ��巹���� ��ų� ����
	if( cardType == CARD_TYPE_SD ) {
		if( !PDD_SDSendCommand( SEND_RELATIVE_ADDR,0,MMC_CMDAT_RESPONSE_R1) ) {  // R6 ����
			printf("ERROR : Receiving card relative address.\n");
			return FALSE;
		}
		g_cardRCA = (UINT16) PDD_SDGetResponse(RCA_REGISTER);
	} else {
		if( !PDD_SDSendCommand( SET_RELATIVE_ADDR, 0,MMC_CMDAT_RESPONSE_R1 ) ) { // HIGH_SHIFT(1)
			printf("nERROR : Setting card relative address.\n");
			return FALSE;
		}
		g_cardRCA = PDD_SDGetResponse(RCA_REGISTER);
	}
	
	// RCA.��巹�� ..
	printf("INFO: Card address is 0x%04x \n", g_cardRCA);
	
	// 5. CMD7�� ����Ͽ� �巡 Ŀ�ǵ忡 ���ؼ� ī�带 ����
	if( !PDD_SDSendCommand(SELECT_DESELECT_CARD, HIGH_SHIFT(g_cardRCA),MMC_CMDAT_EXPECT_BUSY
		| MMC_CMDAT_RESPONSE_R1) ) {   // ���� R1B
		printf("ERROR : Selecting card for data transfer.\n");
		return FALSE;
	}
	
	// 6. Ŭ�ϰ� ������ ����
	if( cardType == CARD_TYPE_SD ) {
		if( PDD_SDSendCommand(APPL_CMD,0,MMC_CMDAT_RESPONSE_R1)) {
			if( PDD_SDSendCommand(SET_BUS_WIDTH, 2,MMC_CMDAT_RESPONSE_R1) ) { // 2
				printf("INFO: 4-bit data bus selected.\n");
				PDD_SDSetPDDCapabilities(SET_4BIT_MODE, TRUE);    // CMDAT0-CMDAT3 ���
			}
		} else {
			printf("INFO: 1-bit data bus selected.\n");
			PDD_SDSetPDDCapabilities(SET_4BIT_MODE, FALSE);     // CMDAT0�� ���
		}
        	// SD ī�� Ŭ��
		PDD_SDSetPDDCapabilities(SET_CLOCK_SD, TRUE);
	} else {
		printf("INFO: 1-bit data bus selected.\n");
		// MMC ī�� Ŭ��
		PDD_SDSetPDDCapabilities(SET_MMC_MODE, FALSE);     // CMDAT0�� ���
		PDD_SDSetPDDCapabilities(SET_CLOCK_MMC, TRUE);
	}
	
	// 7. ����Ʈ ��� ���� ����
	if( !PDD_SDSendCommand(SET_BLOCKLEN, BLOCK_LEN,MMC_CMDAT_RESPONSE_R1) ) {
		printf("\r\nERROR : Setting default block transfer length.\n");
		return FALSE;
	}

	/* Debug Print */
	printf("BLOCK Length Set : %d\n",BLOCK_LEN);
	
	return TRUE;
}

// ��� �б�
BOOL SDReadDataBlock(PVOID pBuffer, DWORD dwLength, DWORD dwAddress)
{
	// ���ǵ带 ��½�Ű�� ���Ͽ� ĳ�� ���
	static DWORD CacheAddress = -1;
	static BYTE  CacheBuffer[BLOCK_LEN];

	while(dwLength){
		
		// 1. ĳ�� ��� �˻�
		if(dwAddress>=CacheAddress && dwAddress<CacheAddress+BLOCK_LEN){
			DWORD dwOffset  = dwAddress-CacheAddress;
			DWORD dwCopyLen = BLOCK_LEN-dwOffset;
			if(dwLength > dwCopyLen){
				memcpy(pBuffer, CacheBuffer+dwOffset, dwCopyLen);
				*(BYTE *)pBuffer += dwCopyLen;				
				dwAddress        += dwCopyLen;
				dwLength         -= dwCopyLen;
			}
			else{
				memcpy(pBuffer, CacheBuffer+dwOffset, dwLength);
				break;
			}
		}
		
		// 2.�� ��� �б� Ŀ�ǵ� �߻�
		if( !PDD_SDSendCommand(READ_SINGLE_BLOCK, dwAddress&BLOCK_MASK,
			  MMC_CMDAT_DATA_EN | MMC_CMDAT_RESPONSE_R1) ) {
			printf("\r\nERROR : READ_SINGLE_BLOCK command returned error.");
			return FALSE;
		}
		
		// 3.1�� ��� ������ �б�
		if(dwLength<=BLOCK_LEN || dwAddress%BLOCK_LEN){
			// ĳ�ô� ���� ������ ��� ����Ѵ�.
			// 1.1�� ��� ���̺��� ���� ���
			// 2.��巹���� 1�� ��� ũ�⿡ ���� ���� ���
			if(PDD_SDReceiveData(CacheBuffer)) {
				CacheAddress = dwAddress&BLOCK_MASK; // ĳ�� ���� ��巹�� ������Ʈ
			}
			else{
				printf("@@@ SDReadDataBlock() 1 Return FALSE \n");
				return FALSE;
			}
		}
		else{
			// ���� ������ ����� ��� ���� �д´�.
			if(PDD_SDReceiveData((UINT8*)pBuffer)){
				*(BYTE *)pBuffer += BLOCK_LEN;				
				dwAddress        += BLOCK_LEN;
				dwLength         -= BLOCK_LEN;
			}
			else{
				printf("@@@ SDReadDataBlock() 2 Return FALSE \n");				
				return FALSE;
			}
		}
	}

	return TRUE;
}
