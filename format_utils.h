#ifndef RCB_FILE_SYSTEM_FORMAT_UTILS_H
#define RCB_FILE_SYSTEM_FORMAT_UTILS_H

#include <stdio.h>
#include <errno.h>
#include "messages.h"

int hard_format(const char *device_name) {
    FILE *device;
    int nil;
    device = fopen(device_name, "w+");
    if (device == NULL) {
        print_invalid_device(strerror(errno));
        return 1;
    }
    fclose(device);
    return 0;
}

int soft_format(const char *device_name) {
    FILE *device;
    device = fopen(device_name, "w+");
    if (device == NULL) {
        print_invalid_device(strerror(errno));
        return 1;
    }
    fclose(device);
    return 0;
}

#endif
