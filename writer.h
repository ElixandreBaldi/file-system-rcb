#ifndef RCB_FILE_SYSTEM_WRITER_H
#define RCB_FILE_SYSTEM_WRITER_H

#include <stdio.h>
#include "struct_utils.h"

typedef struct writer {
    const char *device_name;
    FILE *device;
    long device_size;
    struct boot_record boot;
    struct root_dir;
} writer;

int copy_file(const char *target_file, const char *device_name) {
    FILE *file;
}


#endif
