#include <lib.h>
#include <serial.h>
#include <time.h>
#include <reg.h>
#include <config.h>

unsigned short ext_port2;

#define CFCON_BASE	0x14100004
#define CFRST		(1<<2)  // CF Card reset - Active Low
#define CF_VCC5EN	(1<<1)
#define CF_VCC3EN	(1<<0)

#define PCMCON_BASE	0x14100004
#define PCMRST		(1<<3)
#define PCM_VCC5EN	(1<<7)
#define PCM_VCC3EN	(1<<6)
#define PCM_EN1		(1<<5)
#define PCM_EN0		(1<<4)

/* PCMCIA Socket#0 address */
#define SOC0_IO_BASE	0x20000000
#define SOC0_ATTR_BASE  0x28000000
#define SOC0_COMM_BASE  0x2C000000
/* PCMCIA Socket#1 address */
#define SOC1_IO_BASE	0x30000000
#define SOC1_ATTR_BASE	0x38000000
#define SOC1_COMM_BASE	0x3C000000

/* PCMCIA CIS Code define */
#define CIS_DEVICE			0x01	/* Device Information (comon) */
#define CIS_LONGLINK_MFC	0x06	/* Long-Link for Multi-Function Card */
#define CIS_CHKECKSUM		0x10	/* Checksum Control */
#define CIS_LONGLINK_A		0x11	/* Long-Link control to Attribute mem */
#define CIS_LONGLINK_C		0x12	/* Long-Link control to Common mem */
#define CIS_LINKTARGET		0x13	/* Link Target */
#define CIS_NO_LINK			0x14
#define CIS_VERS_1			0x15	/* Layer 1 Version Information */
#define CIS_ALTSTR			0x16	/* Alternate Language String */
#define CIS_DEVICE_A		0x17	/* Device Information to attribute mem */
#define CIS_JEDEC_C			0x18	/* Specifies the JEDEC */
#define CIS_JEDEC_A			0x19	/* Specifies the JEDEC */
#define CIS_CONFIG			0x1A	/* Configuration tuple */
#define CIS_CFTABLE_ENTRY	0x1B	/* Configuration Table Entry (common) */
#define CIS_DEVICE_OC		0x1C	/* Other Conditions Device info(common mem) */
#define CIS_DEVICE_OA		0x1D	/* Other Conditions Device info(attribute mem) */
#define CIS_DEVICEGEO		0x1E	/* Device Geometry(common mem) */
#define CIS_DEVICEGEO_A	0x1F	/* Device Geometry(attribute mem) */
#define CIS_MANFID			0x20	/* Manufacturers Identification */
#define CIS_FUNCID			0x21	/* Function Identification */
#define CIS_FUNCE			0x22	/* Function Extension */
#define CIS_END				0x0FF  	/* tuple end */


int PCMCIA_CIS(unsigned char * pAttrAddr);


void c_main(void)
{

	unsigned char * pAttrAddr;
	ext_port2 = 0;

	ext_port2 &= ~(CFRST | PCMRST);
	__REG16(CFCON_BASE) |= ext_port2;
	__REG16(CFCON_BASE) |= ext_port2 & ~(PCM_EN0|PCM_EN1|PCM_VCC3EN|PCM_VCC5EN|CF_VCC3EN|CF_VCC5EN);

	pAttrAddr = (unsigned char*)SOC0_ATTR_BASE;		 
	PCMCIA_CIS(pAttrAddr);		/* CIS Print function */
	printf("\n");


}


int PCMCIA_CIS(unsigned char * pAttrAddr)
{
	int i,j,k;
	int cis_code, code_length;
	unsigned int man_fid1,man_fid2;
		
	for(j=0;j<30;j++) {
		cis_code = *pAttrAddr;
		if(cis_code == 0xff) break;      /* Tuple end */
		if (cis_code == CIS_VERS_1 ) 	{
			/* Version Information Print */
			printf("\nProduce info : ");
			pAttrAddr+=2 ; code_length = *pAttrAddr;
			for (i=code_length ;i>0;i--) {
				pAttrAddr+=2 ;
				if(*pAttrAddr != 0xFF) printf("%c", *pAttrAddr);
			}

		}else if (cis_code == CIS_FUNCID ){
			pAttrAddr+=2 ; 		/* code length */
			pAttrAddr+=2;		/* function code */
			k=*pAttrAddr;
			/* Function Print */
			printf("\nFunction %d ",*pAttrAddr);
			switch(k) {
				case 0 : printf ("(Multi-Function)"); break;
				case 1 : printf ("(memory)"); break;
				case 2 : printf ("(serial)"); break;
				case 3 : printf ("(parallel)"); break;
				case 4 : printf ("(fixed disk)"); break;
				case 5 : printf ("(video adapger)"); break;
				case 6 : printf ("(nework)"); break;
				case 7 : printf ("(AIMS)"); break;
				default: break;
			}
			pAttrAddr+=2;
		} else if (cis_code == CIS_MANFID) {
			/* Manufacture ID */
			printf("\nManufacture ID : ");
			pAttrAddr+=2 ;		   /*code length value */
			code_length = *pAttrAddr;
			/* Manufacture ID 1 */
			pAttrAddr+=2 ;		   /* manfid 1 low byte */
			man_fid1 = *pAttrAddr;
			pAttrAddr+=2;
			man_fid1 += *pAttrAddr <<8; /* manfid 1 High byte */
			/* Manufacture ID 2 */			
			pAttrAddr+=2 ;		   /* manfid 1 low byte */
			man_fid2 = *pAttrAddr;
			pAttrAddr+=2;
			man_fid2 += *pAttrAddr << 8; /* manfid 1 High byte */
			printf("0x%04x , 0x%04x", man_fid1, man_fid2);
		} else if (cis_code == CIS_CONFIG) {
			/* CONFIG */
			printf("\nConfig  : ");
			pAttrAddr+=2 ; code_length = *pAttrAddr;
			for (i=code_length ;i>0;i--) {
				pAttrAddr+=2 ;
				if(*pAttrAddr != 0xFF) printf(" %02x", *pAttrAddr);
			}
		} else if (cis_code == CIS_CFTABLE_ENTRY) {
			/* CFTABLE_ENTIRY */
			printf("\nCF Table_Entry : ");
			pAttrAddr+=2 ; code_length = *pAttrAddr;
			for (i=code_length ;i>0;i--) {
				pAttrAddr+=2 ;
				if(*pAttrAddr != 0xFF) printf(" %02x", *pAttrAddr);
			}
		}else {
			//			printf("\nCode [%02x] ",*pAttrAddr);
			pAttrAddr+=2 ;
			code_length = *pAttrAddr;
			//			printf("\n[%02x]", *pAttrAddr);
			for (i=code_length ;i>0;i--) {
				pAttrAddr+=2 ;
				//				if(*pAttrAddr != 0xFF) printf("%02x", *pAttrAddr);
			}
		}

		pAttrAddr+=2;
	}
	return 0;
}

