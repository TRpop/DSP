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

// SD/MMC 초기 설정
BOOL SDInitializeHardware()
{
	CARD_TYPE cardType = CARD_TYPE_NONE;
	int timeOut;
	g_cardRCA = 0;
	
	// SD/MMC 초기 설정
	if( !PDD_SDInitializeHardware() )    return FALSE;

	// 카드 타입 검출
	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_RESPONSE_NONE) ) {
		printf("\r\nERROR : GO_IDLE0 command returned error.\n");
		return FALSE;
	}
	
    	// 카드 커맨드 전송
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
	
	
	// 카드 타입이 존재하지 앟은 경우
	if( cardType == CARD_TYPE_NONE ) {
		printf("\r\nERROR : No valid SD/MMC cards detected.\n");
		return FALSE;
	}

	// ------ 카드 초기 설정 ------- //
	// ---- 1. CMD0를 전송해서 리셋  ---- //
	if( !PDD_SDSendCommand(GO_IDLE_STATE,0,MMC_CMDAT_RESPONSE_NONE) ) {
		printf("\r\nERROR : GO_IDLE0 command returned error.\n");
		return FALSE;
	}

	/* Debug Print */
	printf("Active : IDLE\n");
	
	// ---- 2. CMD1/ACMD41를 이용하여 전압 검출 ---- //
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
		
	// 카드 전원 초기 설정 끝 검사 
	PDD_SDGetResponse(OCR_REGISTER);
       if(XsMMC_OCR.b[4] & OCR_POWER_UP_BUSY)   break;
		PDD_SDStallExecute(100);
	}
	
	// 카드 전원 초기 설정이 되지 않은 경우
	if( timeOut >= 100 ) {
		printf("ERROR : Failed to validate card OCR voltage range.\n");
		return FALSE;
	}
	
	// 3. 첫번째 카드 ID(멀티플 카드는 지원하지 않는다) ---- //
	for( timeOut = 0; timeOut < 30; ++timeOut ) {
		if( PDD_SDSendCommand(ALL_SEND_CID,0,MMC_CMDAT_RESPONSE_R2 ) )   break;
		PDD_SDStallExecute(100);  // 일정시간 지연
	}
	
    	// CID 응답이 없는 경우
	if( timeOut >= 30 ) {
		printf("nERROR : Failed to receive valid CID response from card.\n");
		return FALSE;
	}
	
	// 4. 카드 어드레스를 얻거나 설정
	if( cardType == CARD_TYPE_SD ) {
		if( !PDD_SDSendCommand( SEND_RELATIVE_ADDR,0,MMC_CMDAT_RESPONSE_R1) ) {  // R6 응답
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
	
	// RCA.어드레스 ..
	printf("INFO: Card address is 0x%04x \n", g_cardRCA);
	
	// 5. CMD7을 사용하여 장래 커맨드에 대해서 카드를 선택
	if( !PDD_SDSendCommand(SELECT_DESELECT_CARD, HIGH_SHIFT(g_cardRCA),MMC_CMDAT_EXPECT_BUSY
		| MMC_CMDAT_RESPONSE_R1) ) {   // 응답 R1B
		printf("ERROR : Selecting card for data transfer.\n");
		return FALSE;
	}
	
	// 6. 클록과 버스폭 결정
	if( cardType == CARD_TYPE_SD ) {
		if( PDD_SDSendCommand(APPL_CMD,0,MMC_CMDAT_RESPONSE_R1)) {
			if( PDD_SDSendCommand(SET_BUS_WIDTH, 2,MMC_CMDAT_RESPONSE_R1) ) { // 2
				printf("INFO: 4-bit data bus selected.\n");
				PDD_SDSetPDDCapabilities(SET_4BIT_MODE, TRUE);    // CMDAT0-CMDAT3 사용
			}
		} else {
			printf("INFO: 1-bit data bus selected.\n");
			PDD_SDSetPDDCapabilities(SET_4BIT_MODE, FALSE);     // CMDAT0만 사용
		}
        	// SD 카드 클록
		PDD_SDSetPDDCapabilities(SET_CLOCK_SD, TRUE);
	} else {
		printf("INFO: 1-bit data bus selected.\n");
		// MMC 카드 클록
		PDD_SDSetPDDCapabilities(SET_MMC_MODE, FALSE);     // CMDAT0만 사용
		PDD_SDSetPDDCapabilities(SET_CLOCK_MMC, TRUE);
	}
	
	// 7. 디폴트 블록 길이 설정
	if( !PDD_SDSendCommand(SET_BLOCKLEN, BLOCK_LEN,MMC_CMDAT_RESPONSE_R1) ) {
		printf("\r\nERROR : Setting default block transfer length.\n");
		return FALSE;
	}

	/* Debug Print */
	printf("BLOCK Length Set : %d\n",BLOCK_LEN);
	
	return TRUE;
}

// 블록 읽기
BOOL SDReadDataBlock(PVOID pBuffer, DWORD dwLength, DWORD dwAddress)
{
	// 스피드를 상승시키기 위하여 캐시 사용
	static DWORD CacheAddress = -1;
	static BYTE  CacheBuffer[BLOCK_LEN];

	while(dwLength){
		
		// 1. 캐시 사용 검사
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
		
		// 2.한 블록 읽기 커맨드 발생
		if( !PDD_SDSendCommand(READ_SINGLE_BLOCK, dwAddress&BLOCK_MASK,
			  MMC_CMDAT_DATA_EN | MMC_CMDAT_RESPONSE_R1) ) {
			printf("\r\nERROR : READ_SINGLE_BLOCK command returned error.");
			return FALSE;
		}
		
		// 3.1개 블록 데이터 읽기
		if(dwLength<=BLOCK_LEN || dwAddress%BLOCK_LEN){
			// 캐시는 다음 조건일 경우 사용한다.
			// 1.1개 블록 길이보다 작은 경우
			// 2.어드레스가 1개 블록 크기에 맞지 않은 경우
			if(PDD_SDReceiveData(CacheBuffer)) {
				CacheAddress = dwAddress&BLOCK_MASK; // 캐시 시작 어드레스 업데이트
			}
			else{
				printf("@@@ SDReadDataBlock() 1 Return FALSE \n");
				return FALSE;
			}
		}
		else{
			// 버퍼 공간이 충분한 경우 직접 읽는다.
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
