/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_DRV_H__
#define __MFLASH_DRV_H__

#include <stdbool.h>
#include <stdint.h>

#include "fsl_common.h"

/* Flash constants */
#ifndef MFLASH_SECTOR_SIZE
#define MFLASH_SECTOR_SIZE (0x1000)
#endif

#ifndef MFLASH_PAGE_SIZE
#define MFLASH_PAGE_SIZE (256)
#endif

#ifndef MFLASH_FLEXSPI
#define MFLASH_FLEXSPI FLEXSPI
#endif

#ifndef MFLASH_BASE_ADDRESS
#define MFLASH_BASE_ADDRESS FlexSPI_AMBA_BASE
#endif

/* Allow incremental writes without erase (enabled by default).
 * This method cannot be used in certain cases, e.g. when page checksums are used
 */
#ifndef MFLASH_INC_WRITE
#define MFLASH_INC_WRITE 1
#endif

static inline uint32_t mflash_drv_is_page_aligned(uint32_t addr)
{
    return ((addr) & (MFLASH_PAGE_SIZE - 1)) == 0 ? true : false;
}

static inline uint32_t mflash_drv_is_sector_aligned(uint32_t addr)
{
    return ((addr) & (MFLASH_SECTOR_SIZE - 1)) == 0 ? true : false;
}

int32_t mflash_drv_init(void);

int32_t mflash_drv_page_program(uint32_t page_addr, uint32_t *data);
int32_t mflash_drv_sector_erase(uint32_t sector_addr);

int32_t mflash_drv_read(uint32_t addr, uint32_t *buffer, uint32_t len);
int32_t mflash_drv_write(uint32_t addr, const uint8_t *data, uint32_t data_len);

#endif
