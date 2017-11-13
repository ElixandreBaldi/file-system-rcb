#ifndef RCB_FILE_SYSTEM_FORMAT_UTILS_H
#define RCB_FILE_SYSTEM_FORMAT_UTILS_H

#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include "struct_utils.h"

#define RCB_TABLE_SIZE 131072
#define RCB_DATA_TABLE 32


int write_boot_record(FILE *device, int sect_size, long device_size) {
    printf("%li", device_size);
    boot_record btr;

    fseek(device, 0, SEEK_SET);
    btr.rbc = (unsigned char) "RCB.";
    btr.bytes_per_sector = (unsigned short) sect_size;
    btr.sectors_per_rcb = (unsigned short) (RCB_TABLE_SIZE / btr.bytes_per_sector);
    btr.reserved_sectors = (unsigned short) (1 + btr.sectors_per_rcb);
    btr.entry_directory = (unsigned short) (btr.bytes_per_sector / RCB_DATA_TABLE);
    btr.bytes_per_partition = (unsigned int) device_size;
    btr.sectors_per_disk = (unsigned short) (btr.bytes_per_partition / btr.bytes_per_sector);

    fwrite(&btr.rbc, 1, sizeof(char), device);
    fwrite(&btr.bytes_per_sector, 1, sizeof(short), device);
    fwrite(&btr.reserved_sectors, 1, sizeof(short), device);
    fwrite(&btr.entry_directory, 1, sizeof(short), device);
    fwrite(&btr.bytes_per_partition, 1, sizeof(int), device);
    fwrite(&btr.sectors_per_rcb, 1, sizeof(short), device);
    fwrite(&btr.sectors_per_disk, 1, sizeof(short), device);

    return btr.sectors_per_rcb;
}

void write_root_dir(FILE *device, int sect_size, int sectors_per_rcb){
    root_dir dir;
    strcmp(dir.file_name, "root");
    dir.attribute_of_file = 1;
    dir.first_cluster = 0; // Define first cluster of the root
    dir.size_of_file = 0;
    fseek(device, sect_size * (1 + sectors_per_rcb), SEEK_SET);
}

long get_device_size(FILE *device) {
    long size;
    fseek(device, 0L, SEEK_END);
    size = ftell(device);
    rewind(device);

    return size;
}

int hard_format(const char *device_name, int sect_size) {
    FILE *device;
    long device_size;

    device = fopen(device_name, "rb+");
    if(device == NULL){
        print_invalid_device(strerror(errno));
        return 1;
    }
    device_size = get_device_size(device);

    while (fgetc (device) != EOF) {
        fseek(device, -1, SEEK_CUR);
        fputc('\0', device);
    }

    int sectors_per_rcb = write_boot_record(device, sect_size, device_size);
    write_root_dir(device, sect_size, sectors_per_rcb);

    fclose(device);
    return 0;
}

int soft_format(const char *device_name, int size) {
//    FILE *device;
//    device = fopen(device_name, "rb");
//    if(device == NULL){
//        return 1;
//    }
//
//    return 0;
}

#endif
