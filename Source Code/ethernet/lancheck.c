/* 
  * pxa27x LAN91C111 Check Example firmware program
  *    2009-05-31
  *    M-Tech Co.,Ltd.
  *    lee byung kwon
  */

#include <time.h>
#include <lib.h>
#include <serial.h>
#include <reg.h>
#include <types.h>

#include "smc91111.h"


/*------------------------------------------------------------
 . Retrieve a LAN91C111 register (u16 access only)
 .-------------------------------------------------------------*/
static int smc_get_reg(int bank, int reg)
{
	SMC_SELECT_BANK(bank);
	return (SMC_inw(reg));
}


void smc_dump_registers()
{
        int oldbank = SMC_inw(BANK_SELECT);

        printf("-----smc_dump-----\n");
        printf("-----BANK0-----\n");
        printf("TCR = %08X\n", smc_get_reg(0, TCR_REG));
        printf("EPH = %08X\n", smc_get_reg(0, EPH_STATUS_REG));
        printf("RCR = %08X\n", smc_get_reg(0,  RCR_REG));
        printf("CNT = %08X\n", smc_get_reg(0, COUNTER_REG));
        printf("MIR = %08X\n", smc_get_reg(0, MIR_REG));
        printf("RPC = %08X\n", smc_get_reg(0, RPC_REG));

        printf("-----BANK1-----\n");
        printf("CFG = %08X\n", smc_get_reg(1, CONFIG_REG));
        printf("BAS = %08X\n", smc_get_reg(1, BASE_REG));
        printf("GPR = %08X\n", smc_get_reg(1, GP_REG));
        printf("CTL = %08X\n", smc_get_reg(1, CTL_REG));

        printf("-----BANK2-----\n");
        printf("ARPN= %08X\n", smc_get_reg(2,PN_REG));
        printf("RXTX= %08X\n", smc_get_reg(2,RXFIFO_REG));
        printf("PTR = %08X\n", smc_get_reg(2, PTR_REG));
        printf("ISR = %08X\n", smc_get_reg(2, SMCINT_REG));

        printf("-----BANK3-----\n");
        printf("REV = %08X\n", smc_get_reg(3, REV_REG));
        printf("ERX = %08X\n", smc_get_reg(3, ERCV_REG));

        SMC_outw(BANK_SELECT,oldbank );
}



void c_main(void)
{

	smc_dump_registers();

	/* Register Status summary */
	printf("\r\n============== Ethernet STATUS Summary ==============\r\n");
	printf("DEVICE : LAN91C111 , Physical Address = 0x%08X\n", SMC_BASE_ADDRESS);
	// Status Reg Check.
	smc_get_reg(0,EPH_STATUS_REG) & ES_LATCOL ? printf("LINK Collision : detected \n") : printf("LINK Collision : clean\n");
	smc_get_reg(0,RPC_REG) & RPC_SPEED ? printf("LINK SPEED : 100Mbps \n") : printf("LINK SPEED : 10Mbps\n");	
	smc_get_reg(0,RPC_REG) & RPC_DPLX ? printf("LINK Duplex : Full Duplex \n") : printf("LINK Duplex : Half Duplex\n");
	smc_get_reg(0,RPC_REG) & RPC_ANEG ? printf("LINK Auto-negotiation mode : On \n") : printf("LINK Auto-negotiation mode : Off\n");
	printf("=====================================================\r\n");


}
