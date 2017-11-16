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

bool transfer() {
    return true;
}

void allocate_rcb_for_file(unsigned short *spaces, unsigned short sectors_needed) {
    unsigned short i;
    for (i = 0; i < sectors_needed - 1; i++) {
        write_pos(i, (unsigned short) (i + 1));
    }
    write_pos(i, RCB_EOF);
    sync_rcb(wrt.device, wrt.boot.bytes_per_sector);
}

bool run() {
    unsigned short sectors_needed = (unsigned short) ceil((wrt.target_size / (double) wrt.boot.bytes_per_sector));
    read_rcb(wrt.device, wrt.boot.bytes_per_sector);
    unsigned int available_pos = free_positions(wrt.boot.reserved_sectors);
    unsigned short *spaces;
    if (available_pos >= sectors_needed) {
        spaces = get_free_spaces(sectors_needed, wrt.boot.reserved_sectors);
        allocate_rcb_for_file(spaces, sectors_needed);
        return transfer();
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
