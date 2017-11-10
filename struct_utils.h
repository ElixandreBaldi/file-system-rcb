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

typedef struct rcb_table{
    char file_name[24];
    unsigned short attribute_of_file;
    unsigned short first_cluster;
    unsigned int size_of_file;
} __attribute__((packed)) rcb_table;

#endif