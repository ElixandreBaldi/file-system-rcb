#ifndef RCB_FILE_SYSTEM_RBC_ALLOCATION_TABLE_H
#define RCB_FILE_SYSTEM_RBC_ALLOCATION_TABLE_H

typedef struct rcb_allocation_table{
    char file_name[24];
    unsigned short attribute_of_file;
    unsigned short first_cluster;
    unsigned int size_of_file;
} __attribute__((packed)) rcb_allocation_table;

#endif
