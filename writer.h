#ifndef RCB_FILE_SYSTEM_WRITER_H
#define RCB_FILE_SYSTEM_WRITER_H

#include <stdio.h>
#include <math.h>
#include "struct_utils.h"

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

bool run() {
    unsigned long sectors_needed = (unsigned long) ceil(wrt.target_size / wrt.boot.bytes_per_sector);
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
