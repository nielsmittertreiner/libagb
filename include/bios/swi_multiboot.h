#ifndef GUARD_AGB_BIOS_MULTIBOOT_H
#define GUARD_AGB_BIOS_MULTIBOOT_H

#include "agb.h"

#define MULTIBOOT_NCHILD 3

typedef struct multi_boot_param
{
    uint32_t system_work[5]; // 00
    uint8_t handshake_data;  // 14
    uint8_t padding;         // 15
    uint16_t handshake_timeout; // 16
    uint8_t probe_count; // 18
    uint8_t client_data[MULTIBOOT_NCHILD]; // 19
    uint8_t palette_data; // 1c
    uint8_t response_bit; // 1d
    uint8_t client_bit; // 1e
    uint8_t reserved_1; // 1f
    const uint8_t *boot_src_ptr; // 20
    const uint8_t *boot_end_ptr; // 24
    const uint8_t *master_ptr;
    uint8_t *reserved_2[MULTIBOOT_NCHILD];
    uint32_t system_work_2[4];
    uint8_t send_flag;
    uint8_t probe_target_bit;
    uint8_t check_wait;
    uint8_t server_type;
} multi_boot_param;

typedef enum
{
    MODE32_NORMAL,
    MODE16_MULTI,
    MODE32_2MHZ,
} multi_boot_mode;

extern uint32_t multi_boot(multi_boot_param *param);

#endif // GUARD_AGB_BIOS_MULTIBOOT_H