#ifndef RCB_FILE_SYSTEM_BOOT_UTILS_H
#define RCB_FILE_SYSTEM_BOOT_UTILS_H

#include <stdio.h>
#include "struct_utils.h"

boot_record read_boot_record(boot_record boot, FILE *device) {
    fread(&boot, 1, sizeof(boot), device);
    return boot;
}

#endif
