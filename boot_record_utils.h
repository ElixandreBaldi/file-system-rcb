#ifndef RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H
#define RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H

typedef struct boot_record{
    unsigned int rbc;
    unsigned short bytes_per_sector;
    unsigned short reserved_sectors;
    unsigned short entry_directory;
    unsigned short bytes_per_partition;
    unsigned short sectors_per_rcb;
    unsigned short sectors_per_disk;
} __attribute__((packed)) boot_record;

#endif