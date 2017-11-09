#include <stdio.h>
#include <string.h>
#include "messages.h"

int format_device(const char *device) {
    printf("Are you sure you want to format the device? [Y/n] ");
    return 0;
}

int enter_device(const char *device) {
    printf("Reading device...\n");
    return 0;
}

int main(int argc, const char **argv) {
    const char *device;

    if (argc < 2) {
        print_error();
        return 1;
    }
    if (strcmp(argv[1], "--help") || strcmp(argv[1], "-h")) {
        print_help();
        return 0;
    }
    if (strcmp(argv[1], "--format") || strcmp(argv[1], "-f")) {
        if (argc < 3) {
            print_error();
            return 1;
        }
        device = argv[2];
        return format_device(device);
    }
    if (strcmp(argv[1], "--enter") || strcmp(argv[1], "-e")) {
        if (argc < 3) {
            print_error();
            return 1;
        }
        device = argv[2];
        return enter_device(device);
    }

    return 0;
}
