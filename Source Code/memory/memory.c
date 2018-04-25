#include <lib.h>
#include <config.h>

int testdram(u32 start, u32 end)
{
	unsigned int *pstart = (unsigned int *)start;  // 시작주소를 저장한 어드레스 포인트변수설정
	unsigned int *pend = (unsigned int *)end;   // 끝주소를 저장한 어드레스 포인트변수설정
	unsigned int *p;   // 주소변경을 임시저장 어드레스 포인트변수설정
   /* 시작주소에 끝 주소까지 주소하나씩 증가하면서 0xAAAA AAAA 쓰기 */
	for (p = pstart; p < pend; p++) 
		*p = 0xaaaaaaaa;

/* 시작주소에 끝 주소까지 주소하나씩 증가하면서 0xAAAA AAAA 검사 */
	for (p = pstart; p < pend; p++) {
		if (*p != 0xaaaaaaaa) {
			printf("SDRAM test fails at: %08x\n", (unsigned int)p);
			return 1;
		}
	}

/* 시작주소에 끝 주소까지 주소하나씩 증가하면서 0x5555 5555 쓰기 */
	for (p = pstart; p < pend; p++)
		*p = 0x55555555;

/* 시작주소에 끝 주소까지 주소하나씩 증가하면서 0x5555 5555 검사 */
	for (p = pstart; p < pend; p++) {
		if (*p != 0x55555555) {
			printf("SDRAM test fails at: %08x\n", (unsigned int)p);
			return 1;
		}
	}

	return 0;
}

void c_main(void)
{
	/* 4byte 단위로 메모리, 래지스터에 값을 넣는다. */
	/* ex> write  0x80807000~0x80807fff */
	u32 start_addr;
	u32 end_addr; 
	int ret;

	start_addr = 0x80807000;
	end_addr = 0x80807fff; 


	/* SDRAM 에 데이터를 쓰고 다시 읽어서 검사하는 testdram() 을 실행.
           반환값이 '1' 이면 SDRAM 동작이 비정상적이다.(SDRAM Test Fail). */
           
	ret = testdram(start_addr, end_addr);
	printf("ret=%d\n", ret);

	if(ret == 1)	{
		printf("SDRAM Test Fail\n");
	}	else	{
		printf("SDRAM Test OK\n");
	}

}

