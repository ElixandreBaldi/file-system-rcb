#ifndef RCB_FILE_SYSTEM_FORMAT_UTILS_H
#define RCB_FILE_SYSTEM_FORMAT_UTILS_H

#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include "struct_utils.h"

#define RCB_TABLE_SIZE 131072


int write_boot_record(FILE *device, int size) {
    boot_record btr;

    fseek(device, 0, SEEK_SET);
    btr.rbc = (unsigned char) "RCB.";
    btr.bytes_per_sector = (unsigned short) size;
    btr.sectors_per_rcb = (unsigned short) (RCB_TABLE_SIZE / btr.bytes_per_sector);
    btr.reserved_sectors = (unsigned short) (1 + btr.sectors_per_rcb);
    btr.entry_directory = (unsigned short) (btr.bytes_per_sector / 32); //32 seria o tamanho da tabela de dados.
    btr.bytes_per_partition = (unsigned int) 20480000; //TODO
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

void write_root_dir(FILE *device, int size, int sectors_per_rcb){
    root_dir dir;

    strcmp(dir.file_name, "root");
    dir.attribute_of_file = 1;
    dir.first_cluster = 0; // Define first cluster of the root
    dir.size_of_file = 0;
    fseek(device, size * (1 + sectors_per_rcb), SEEK_SET);
}

int hard_format(const char *device_name, int size) {
    FILE *device;

    device = fopen(device_name, "rb+");
    if(device == NULL){
        print_invalid_device(strerror(errno));
        return 1;
    }

    while (fgetc (device) != EOF) {
        fseek(device, -1, SEEK_CUR);
        fputc('\0', device);
    }

    int sectors_per_rcb = write_boot_record(device, size);
    write_root_dir(device, size, sectors_per_rcb);

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
