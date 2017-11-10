#ifndef RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H
#define RCB_FILE_SYSTEM_BOOT_RECORD_UTILS_H

typedef struct boot_record{
    unsigned char rbc;
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

#endif