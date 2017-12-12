#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"
#ifndef RCB_FILE_SYSTEM_WRITER_H
#define RCB_FILE_SYSTEM_WRITER_H

#include "reader.h"

writer wrt;

bool prepare_files () {
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

void allocate_rcb_for_file (unsigned short *spaces, unsigned short sectors_needed, FILE *device,
                            unsigned short bytes_per_sector) {
    unsigned short i;
    for (i = 0; i < sectors_needed - 1; i++) {
        write_pos(spaces[i], (unsigned short) (spaces[i] + 1));
    }
    write_pos(spaces[i], RCB_EOF);
    sync_rcb(device, bytes_per_sector);
}

void allocate_root_dir_for_file (unsigned short first_cluster) {
    unsigned int size;
    unsigned int position = root_begin(wrt.boot.bytes_per_sector, wrt.boot.sectors_per_rcb);
    int          i;
    unsigned short cluster;
    if (&wrt.current_dir[1] == NULL) {
        size             = DIR_ENTRY;
    } else {
        size             = wrt.boot.bytes_per_sector;
        for (i = 0; i < size; i++) {
            unsigned char name[sizeof(&wrt.current_dir[1])];
            fseek(wrt.device, position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, sizeof(name), 1, wrt.device);
            if (strcmp((const char *) name,&wrt.current_dir[1]) == 0) {
                break;
            }
        }
        fseek(wrt.device, position + FIRST_CLUSTER_POSITION + (i * ENTRY_SIZE), SEEK_SET);
        fread(&cluster, sizeof(cluster), 1, wrt.device);
        position = data_section_begin(wrt.boot.bytes_per_sector, wrt.boot.sectors_per_rcb,
                                              sectors_per_dir(wrt.boot.bytes_per_sector), cluster);
    }
    for (i = 0; i < size; i++) {
        unsigned char name[sizeof(wrt.dir.file_name)];
        fseek(wrt.device, position + (i * ENTRY_SIZE), SEEK_SET);
        fread(&name, sizeof(name), 1, wrt.device);
        if (strcmp((const char *) name, wrt.target_path) == 0) {
            print_file_name_repetead();
            return;
        }
    }
    for (i                 = 0; i < size; i++) {
        unsigned int value = seek_rcb(wrt.device, position + (i * ENTRY_SIZE));
        fflush(wrt.device);
        if (value == EMPTY_ATTR || value == DELETED_ATTR) break;
    }
    const char   *filename = last_token(wrt.target_path);
    strcpy(wrt.dir.file_name, filename);
    wrt.dir.first_cluster     = first_cluster;
    wrt.dir.size_of_file      = (unsigned int) wrt.target_size;
    wrt.dir.attribute_of_file = FILE_ATTR;
    fseek(wrt.device, (position) + (i * ENTRY_SIZE), SEEK_SET);
    fwrite(&wrt.dir, 1, sizeof(root_dir), wrt.device);
}

bool allocate_space_data (unsigned short sectors_needed, unsigned short *spaces) {
    unsigned int data_position = (unsigned int) (wrt.boot.bytes_per_sector *
                                                 (wrt.boot.sectors_per_rcb + 1 + DIR_ENTRY));
    for (int     i             = 0; i < sectors_needed; i++) {
        unsigned char data[wrt.boot.bytes_per_sector];
        unsigned int  checker;
        fread(&data, 1, sizeof(data), wrt.target);
        fseek(wrt.device, data_position + ((spaces[i] + 1)) * wrt.boot.bytes_per_sector, SEEK_SET);
        checker = (unsigned int) fwrite(&data, 1, sizeof(data), wrt.device);
        if (checker != wrt.boot.bytes_per_sector) return false;
    }
    return true;
}

bool run () {
    unsigned short sectors_needed = (unsigned short) ceil((wrt.target_size / (double) wrt.boot.bytes_per_sector));
    read_rcb(wrt.device, wrt.boot.bytes_per_sector);
    unsigned short *spaces;
    if (free_positions(sectors_needed)) {
        spaces = get_free_spaces(sectors_needed, wrt.boot.reserved_sectors);
        if(cd(wrt.device, wrt.boot.bytes_per_sector, wrt.boot.sectors_per_rcb, wrt.dir_path_rcb, wrt.current_dir)) {
            allocate_rcb_for_file(spaces, sectors_needed, wrt.device, wrt.boot.bytes_per_sector);
            allocate_root_dir_for_file(spaces[0]);
            return allocate_space_data(sectors_needed, spaces);
        }
    } else {
        print_not_enough_space(sectors_needed * wrt.boot.bytes_per_sector, wrt.boot.bytes_per_sector);
    }
    return false;
}

int copy_file (const char *target_path, const char *device_name, char *origin) {
    bool ret;
    wrt.current_dir  = malloc(sizeof(char));
    wrt.device_name  = device_name;
    wrt.target_path  = target_path;
    wrt.dir_path_rcb = &origin[1];
    strcpy(wrt.current_dir, "/");
    ret = prepare_files() && run();
    if (!ret) {
        return 1;
    }
    fclose(wrt.target);
    fclose(wrt.device);
}


#endif

#pragma clang diagnostic pop