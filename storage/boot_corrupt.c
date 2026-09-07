/**
 * @file boot_corrupt.c
 * @brief Bootloader Partition and LUN Corruption Engine for PKM Duress Protocol.
 * 
 * Responsible for securely and irreversibly corrupting critical boot structures
 * to trigger permanent hardware Boot ROM rejection during duress events.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* External hardware abstraction interface declarations */
extern int32_t storage_write_raw(uint32_t lun_id, uint64_t offset, const void *buffer, size_t length);
extern void    storage_sync_cache(uint32_t lun_id);

/**
 * @brief Configuration parameters for target boot structures.
 */
typedef struct {
    uint32_t target_lun;
    uint64_t critical_offset;
    size_t   corruption_size;
} boot_target_descriptor_t;

/**
 * @brief Generates a pseudo-random entropy garbage pattern to invalidate boot structures.
 * 
 * @param[out] buffer Pointer to the destination buffer.
 * @param[in]  length Number of bytes to populate.
 */
static void generate_entropy_garbage(uint8_t *buffer, size_t length) {
    static const uint8_t ENTROPY_SEED_PATTERN[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x55, 0xAA, 0xFF, 0x00 };
    
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = ENTROPY_SEED_PATTERN[i % sizeof(ENTROPY_SEED_PATTERN)] ^ (uint8_t)(i & 0xFF);
    }
}

/**
 * @brief Overwrites key boot structures with unrecoverable garbage data.
 * 
 * Executed strictly during the PKM duress sequence to ensure permanent 
 * hardware-level rejection by the Boot ROM.
 * 
 * @return int32_t Returns 0 on successful corruption execution, or a negative error code.
 */
int32_t boot_corrupt_execute_duress_sequence(void) {
    enum { PRIMARY_BOOT_LUN = 0x00, PRIMARY_BOOT_BLOCK_SIZE = 4096 };

    const boot_target_descriptor_t primary_boot_partition = {
        .target_lun      = PRIMARY_BOOT_LUN,
        .critical_offset = 0x00000000ULL,
        .corruption_size = PRIMARY_BOOT_BLOCK_SIZE
    };

    uint8_t corruption_payload[PRIMARY_BOOT_BLOCK_SIZE];
    
    generate_entropy_garbage(corruption_payload, sizeof(corruption_payload));

    const int32_t write_status = storage_write_raw(
        primary_boot_partition.target_lun,
        primary_boot_partition.critical_offset,
        corruption_payload,
        primary_boot_partition.corruption_size
    );

    if (write_status != 0) {
        return write_status;
    }

    storage_sync_cache(primary_boot_partition.target_lun);

    return 0;
}