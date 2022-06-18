#pragma once
#include <stdint.h>

struct RSDPtr
{
	char Signature[8];
	char CheckSum;
	char OemID[6];
	char Revision;
	uint32_t *RsdtAddress;
};

struct FACP
{
	char Signature[4];
	uint32_t Length;
	char unneded1[40 - 8];
	uint32_t *DSDT;
	char unneded2[48 - 44];
	uint32_t *SMI_CMD;
	char ACPI_ENABLE;
	char ACPI_DISABLE;
	char unneded3[64 - 54];
	uint32_t *PM1a_CNT_BLK;
	uint32_t *PM1b_CNT_BLK;
	char unneded4[89 - 72];
	char PM1_CNT_LEN;
};

namespace ACPI
{
	unsigned int *acpiCheckRSDPtr(unsigned int *ptr);
	unsigned int *acpiGetRSDPtr(void);
	int acpiCheckHeader(unsigned int *ptr, char *sig);
	int acpiEnable(void);
	int init();
	void poweroff(void);
};