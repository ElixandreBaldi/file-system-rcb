#ifndef RCB_FILE_SYSTEM_WRITER_H
#define RCB_FILE_SYSTEM_WRITER_H

#include <stdio.h>
#include <math.h>
#include "data_structures.h"
#include "rcb_utils.h"

writer wrt;

bool prepare_files() {
    wrt.device = fopen(wrt.device_name, "rb+");
    if (wrt.device == NULL) {
        print_invalid_device(strerror(errno));
        return false;
    }
    wrt.boot = read_boot_record(wrt.boot, wrt.device);
    if (!is_valid_boot_record(wrt.boot)) {
        print_non_rcbfs_device();
        return false;
    }
    wrt.target = fopen(wrt.target_path, "rb+");
    if (wrt.target == NULL) {
        print_invalid_file(strerror(errno));
        return false;
    }
    wrt.device_size = get_size(wrt.device);
    wrt.target_size = get_size(wrt.target);
    return true;
}

void allocate_rcb_for_file(unsigned short *spaces, unsigned short sectors_needed) {
    unsigned short i;
    for (i = 0; i < sectors_needed - 1; i++) {
        write_pos(spaces[i], (unsigned short) (spaces[i] + 1));
    }
    write_pos(spaces[i], RCB_EOF);
    sync_rcb(wrt.device, wrt.boot.bytes_per_sector);
}

void allocate_root_dir_for_file(unsigned short first_sector){
    unsigned int posix = (unsigned int) (wrt.boot.bytes_per_sector * (wrt.boot.sectors_per_rcb + 1) +25);
    int i;
    for(i = 0; i < DIR_ENTRY; i++) {
        unsigned int value = 0; // verificar o erro da primeira posicao
        value = seek_rcb(wrt.device, posix + (i * 32));
        fflush(wrt.device);
        if(value == EMPTY_ATTR) break;
    }

    strcpy(wrt.dir.file_name,"teste.txt"); // falta criar a funcao para pegar apenas o nome do arquivo no wrt.target_path
    wrt.dir.first_cluster = first_sector;
    wrt.dir.size_of_file = (unsigned int) wrt.target_size;
    wrt.dir.attribute_of_file = FILE_ATTR;
    fseek(wrt.device, (posix - 25) + (i * 32), SEEK_SET);
    fwrite(&wrt.dir, 1, sizeof(root_dir), wrt.device);
}

bool allocate_space_data(unsigned short sectors_needed, unsigned short *spaces){
    unsigned int data_position = (unsigned int) (wrt.boot.bytes_per_sector * (wrt.boot.sectors_per_rcb + 1 + DIR_ENTRY));
    for(int i = 0; i < sectors_needed; i++) {
        unsigned char data[wrt.boot.bytes_per_sector];
        unsigned int checker;
        fread(&data, 1, sizeof(data),wrt.target);
        fseek(wrt.device,data_position + ((spaces[i]+1)) * wrt.boot.bytes_per_sector,SEEK_SET);
        checker = (unsigned int) fwrite(&data, 1, sizeof(data), wrt.device);
        if(checker != wrt.boot.bytes_per_sector) return false;
    }
    return true;
}

bool run() {
    unsigned short sectors_needed = (unsigned short) ceil((wrt.target_size / (double) wrt.boot.bytes_per_sector));
    read_rcb(wrt.device, wrt.boot.bytes_per_sector);
    unsigned int available_pos = free_positions(wrt.boot.reserved_sectors);
    unsigned short *spaces;
    if (available_pos >= sectors_needed) {
        spaces = get_free_spaces(sectors_needed, wrt.boot.reserved_sectors);
        allocate_rcb_for_file(spaces, sectors_needed);
        allocate_root_dir_for_file(spaces[0]);
        return allocate_space_data(sectors_needed, spaces);
    } else {
        print_not_enough_space(sectors_needed * wrt.boot.bytes_per_sector, available_pos * wrt.boot.bytes_per_sector);
        return false;
    }
    return true;
}

int copy_file(const char *target_path, const char *device_name) {
    bool ret;
    wrt.device_name = device_name;
    wrt.target_path = target_path;
    ret = prepare_files() && run();
    if (!ret) {
        return 1;
    }
    fclose(wrt.target);
    fclose(wrt.device);
}


#endif