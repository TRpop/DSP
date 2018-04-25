#include <lib.h>
#include <config.h>

int testdram(u32 start, u32 end)
{
	unsigned int *pstart = (unsigned int *)start;  // �����ּҸ� ������ ��巹�� ����Ʈ��������
	unsigned int *pend = (unsigned int *)end;   // ���ּҸ� ������ ��巹�� ����Ʈ��������
	unsigned int *p;   // �ּҺ����� �ӽ����� ��巹�� ����Ʈ��������
   /* �����ּҿ� �� �ּұ��� �ּ��ϳ��� �����ϸ鼭 0xAAAA AAAA ���� */
	for (p = pstart; p < pend; p++) 
		*p = 0xaaaaaaaa;

/* �����ּҿ� �� �ּұ��� �ּ��ϳ��� �����ϸ鼭 0xAAAA AAAA �˻� */
	for (p = pstart; p < pend; p++) {
		if (*p != 0xaaaaaaaa) {
			printf("SDRAM test fails at: %08x\n", (unsigned int)p);
			return 1;
		}
	}

/* �����ּҿ� �� �ּұ��� �ּ��ϳ��� �����ϸ鼭 0x5555 5555 ���� */
	for (p = pstart; p < pend; p++)
		*p = 0x55555555;

/* �����ּҿ� �� �ּұ��� �ּ��ϳ��� �����ϸ鼭 0x5555 5555 �˻� */
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
	/* 4byte ������ �޸�, �������Ϳ� ���� �ִ´�. */
	/* ex> write  0x80807000~0x80807fff */
	u32 start_addr;
	u32 end_addr; 
	int ret;

	start_addr = 0x80807000;
	end_addr = 0x80807fff; 


	/* SDRAM �� �����͸� ���� �ٽ� �о �˻��ϴ� testdram() �� ����.
           ��ȯ���� '1' �̸� SDRAM ������ ���������̴�.(SDRAM Test Fail). */
           
	ret = testdram(start_addr, end_addr);
	printf("ret=%d\n", ret);

	if(ret == 1)	{
		printf("SDRAM Test Fail\n");
	}	else	{
		printf("SDRAM Test OK\n");
	}

}

