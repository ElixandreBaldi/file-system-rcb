#include "generic_utils.h"

#ifndef RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H
#define RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H

typedef struct boot_record {
    char rcb[2];
    unsigned short bytes_per_sector;
    unsigned short reserved_sectors;
    unsigned short entry_directory;
    unsigned int bytes_per_partition;
    unsigned short sectors_per_rcb;
    unsigned short sectors_per_disk;
} __attribute__((packed)) boot_record;

typedef struct root_dir {
    char file_name[24];
    unsigned short attribute_of_file;
    unsigned short first_cluster;
    unsigned int size_of_file;
} __attribute__((packed)) root_dir;

typedef struct rcb_table {
    unsigned short content[RCB_TABLE_SIZE];
} __attribute__((packed)) rcb_table;

typedef struct writer {
    const char *device_name;
    const char *target_path;
    FILE *device;
    FILE *target;
    unsigned long device_size;
    unsigned long target_size;
    struct boot_record boot;
    struct root_dir;
} writer;

typedef struct navigator {
    const char *device_name;
    FILE *device;
    unsigned long device_size;
    struct boot_record boot;
    struct root_dir;
} navigator;

#endif