/* DreamShell ##version##

	devices.c

	(c)2009-2014 SWAT
	http://www.dc-swat.ru
*/

#include <assert.h>
#include <stdio.h>
#include "ds.h"
#include "drivers/bflash.h"
#include "flash_if.h"

const bflash_manufacturer_t bflash_manufacturers[] =
{
	{ "AMD",                0x0001 },
	{ "STMicroelectronics", 0x0020 },
	{ "Macronix",           0x00C2 },
	{ "AMIC",               0x0037 },
	{ "ESMT",               0x008C },
	{ "Sega",               0x00FF },
	{ "Unknown",            0x0000 }
};

static uint32 sectors_29_800_t[] = {
	0x00000, 0x10000, 0x20000, 0x30000, 
	0x40000, 0x50000, 0x60000, 0x70000, 
	0x80000, 0x90000, 0xA0000, 0xB0000, 
	0xC0000, 0xD0000, 0xE0000, 0xF0000, 
	0xF8000, 0xFA000, 0xFC000
};

static uint32 sectors_29_800_b[] = {
	0x00000, 0x04000, 0x06000, 0x08000,
	0x10000, 0x20000, 0x30000, 0x40000,
	0x50000, 0x60000, 0x70000, 0x80000, 
	0x90000, 0xA0000, 0xB0000, 0xC0000, 
	0xD0000, 0xE0000, 0xF0000
};

static uint32 sectors_29_160_t[] = {
	0x000000, 0x010000, 0x020000, 0x030000, 
	0x040000, 0x050000, 0x060000, 0x070000, 
	0x080000, 0x090000, 0x0A0000, 0x0B0000, 
	0x0C0000, 0x0D0000, 0x0E0000, 0x0F0000, 
	0x100000, 0x110000, 0x120000, 0x130000, 
	0x140000, 0x150000, 0x160000, 0x170000, 
	0x180000, 0x190000, 0x1A0000, 0x1B0000, 
	0x1C0000, 0x1D0000, 0x1E0000, 0x1F0000, 
	0x1F8000, 0x1FA000, 0x1FC000
};

static uint32 sectors_29_160_b[] = {
	0x000000, 0x004000, 0x006000, 0x008000, 
	0x010000, 0x020000, 0x030000, 0x040000, 
	0x050000, 0x060000, 0x070000, 0x080000, 
	0x090000, 0x0A0000, 0x0B0000, 0x0C0000, 
	0x0D0000, 0x0E0000, 0x0F0000, 0x100000, 
	0x110000, 0x120000, 0x130000, 0x140000, 
	0x150000, 0x160000, 0x170000, 0x180000, 
	0x190000, 0x1A0000, 0x1B0000, 0x1C0000, 
	0x1D0000, 0x1E0000, 0x1F0000
};


/* Command unlock addrs */
#define CMD_UNLOCK_BM {ADDR_UNLOCK_1_BM, ADDR_UNLOCK_2_BM}
#define CMD_UNLOCK_WM {ADDR_UNLOCK_1_WM, ADDR_UNLOCK_2_WM}
#define CMD_UNLOCK_JEDEC {ADDR_UNLOCK_1_JEDEC, ADDR_UNLOCK_2_JEDEC}


/* Short flags */
#define FB_RD F_FLASH_READ
#define FB_PR F_FLASH_PROGRAM
#define FB_ER F_FLASH_ERASE_SECTOR | F_FLASH_ERASE_ALL | F_FLASH_ERASE_SUSPEND
#define FB_3V F_FLASH_LOGIC_3V
#define FB_5V F_FLASH_LOGIC_5V


const bflash_dev_t bflash_devs[] =
{
	
	/* STMicroelectronics */
	{ "M29W800T",  0x20D7, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 1024, 1, 20, sectors_29_800_t},
	{ "M29W800B",  0x205B, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 1024, 1, 20, sectors_29_800_b},
	{ "M29W160BT", 0x20C4, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "M29W160BB", 0x2049, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},

	/* AMD */
	{ "Am29LV800T",  0x01DA, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 1024, 1, 20, sectors_29_800_t},
	{ "Am29LV800B",  0x015B, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 1024, 1, 20, sectors_29_800_b},
	{ "Am29LV160DT", 0x01C4, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "Am29LV160DB", 0x0149, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},
	{ "Am29F160DT",  0x01D2, (FB_RD | FB_PR | FB_ER | FB_5V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "Am29F160DB",  0x01D8, (FB_RD | FB_PR | FB_ER | FB_5V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},

	/* Macronix */
	{ "MX29LV160T", 0xC2C4, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "MX29LV160B", 0xC249, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},
	{ "MX29F1610",  0xC2F1, (FB_RD | FB_PR | FB_ER | FB_5V), CMD_UNLOCK_JEDEC, 2048, 128, 16, (uint32 []) { 
		0x000000, 0x020000, 0x040000, 0x060000,
		0x080000, 0x0A0000, 0x0C0000, 0x0E0000,
		0x100000, 0x120000, 0x140000, 0x160000, 
		0x180000, 0x1A0000, 0x1C0000, 0x1E0000
	}},
	{ "MX29F016",  0xC2AD, (FB_RD | FB_PR | FB_ER | FB_5V), CMD_UNLOCK_WM, 2048, 1, 32, (uint32 []) { 
		0x000000, 0x010000, 0x020000, 0x030000, 
		0x040000, 0x050000, 0x060000, 0x070000, 
		0x080000, 0x090000, 0x0A0000, 0x0B0000, 
		0x0C0000, 0x0D0000, 0x0E0000, 0x0F0000, 
		0x100000, 0x110000, 0x120000, 0x130000, 
		0x140000, 0x150000, 0x160000, 0x170000, 
		0x180000, 0x190000, 0x1A0000, 0x1B0000, 
		0x1C0000, 0x1D0000, 0x1E0000, 0x1F0000
	}},
	{ "MX29F400", 0xC299, (FB_RD | FB_PR | FB_ER | FB_5V), CMD_UNLOCK_BM, 512, 1, 8, (uint32 []) { 
		0x000000, 0x010000, 0x020000, 0x030000,
		0x040000, 0x050000, 0x060000, 0x070000
	}},	
	{ "MX29LV320T", 0xC2A7, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 4096, 1, 24, (uint32 []) { 
		0x000000, 0x010000, 0x020000, 0x030000, 
		0x040000, 0x050000, 0x060000, 0x070000, 
		0x080000, 0x090000, 0x0A0000, 0x0B0000, 
		0x0C0000, 0x0D0000, 0x0E0000, 0x0F0000, 
		0x100000, 0x110000, 0x120000, 0x130000, 
		0x140000, 0x150000, 0x160000, 0x170000, 
		0x180000, 0x190000, 0x1A0000, 0x1B0000, 
		0x1C0000, 0x1D0000, 0x1E0000, 0x1F0000,
		0x200000, 0x210000, 0x220000, 0x230000, 
		0x240000, 0x250000, 0x260000, 0x270000, 
		0x280000, 0x290000, 0x2A0000, 0x2B0000,
		0x2C0000, 0x2D0000, 0x2E0000, 0x2F0000,  
		0x300000, 0x310000, 0x320000, 0x330000, 
		0x340000, 0x350000, 0x360000, 0x370000, 
		0x380000, 0x390000, 0x3A0000, 0x3B0000,
		0x3C0000, 0x3D0000, 0x3E0000, 0x3F0000, 
		0x3F2000, 0x3F4000, 0x3F6000, 0x3F8000, 
		0x3FA000, 0x3FC000, 0x3FE000
	}},
	{ "MX29LV320B", 0xC2A8, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 4096, 1, 24, (uint32 []) { 
		0x000000, 0x002000, 0x004000, 0x006000, 
		0x008000, 0x00A000, 0x00C000, 0x00E000, 
		0x010000, 0x020000, 0x030000, 0x040000, 
		0x050000, 0x060000, 0x070000, 0x080000, 
		0x090000, 0x0A0000, 0x0B0000, 0x0C0000, 
		0x0D0000, 0x0E0000, 0x0F0000, 0x100000, 
		0x110000, 0x120000, 0x130000, 0x140000, 
		0x150000, 0x160000, 0x170000, 0x180000, 
		0x190000, 0x1A0000, 0x1B0000, 0x1C0000, 
		0x1D0000, 0x1E0000, 0x1F0000, 0x200000, 
		0x210000, 0x220000, 0x230000, 0x240000, 
		0x250000, 0x260000, 0x270000, 0x280000, 
		0x290000, 0x2A0000, 0x2B0000, 0x2C0000, 
		0x2D0000, 0x2E0000, 0x2F0000, 0x300000, 
		0x310000, 0x320000, 0x330000, 0x340000, 
		0x350000, 0x360000, 0x370000, 0x380000, 
		0x390000, 0x3A0000, 0x3B0000, 0x3C0000, 
		0x3D0000, 0x3E0000, 0x3F0000
	}},
	{ "MX29L3211",  0xC2F9, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_JEDEC, 4096, 256, 32, (uint32 []) { 
		0x000000, 0x020000, 0x040000, 0x060000,
		0x080000, 0x0A0000, 0x0C0000, 0x0E0000,
		0x100000, 0x120000, 0x140000, 0x160000, 
		0x180000, 0x1A0000, 0x1C0000, 0x1E0000,
		0x200000, 0x220000, 0x240000, 0x260000,
		0x280000, 0x2A0000, 0x2C0000, 0x2E0000,
		0x300000, 0x320000, 0x340000, 0x360000, 
		0x380000, 0x3A0000, 0x3C0000, 0x3E0000
	}},
	
	/* AMIC */
	{ "A29L160AT", 0x37C4, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "A29L160AB", 0x3749, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},
	
	/* ESMT */
	{ "F49L160UA", 0x8CC4, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_t},
	{ "F49L160BA", 0x8C49, (FB_RD | FB_PR | FB_ER | FB_3V), CMD_UNLOCK_BM, 2048, 1, 35, sectors_29_160_b},
	
	/* Sega */
	{ "MPR-XXXXX", SEGA_FLASH_DEVICE_ID, (FB_RD | FB_3V | FB_5V), CMD_UNLOCK_BM, 2048, 1, 1, (uint32 []) { 0 }},
	
	/* Default entry */
	{ "Unknown", 0x0000, 0, {0, 0}, 0, 0, 0, (uint32 []) { 0 }}
};

