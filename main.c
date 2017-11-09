#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "messages.h"
#include "format_utils.h"

int format_device(const char *device_name) {
    char option;
    printf("Are you sure you want to format the device? [Y/n] ");
    scanf("%c", &option);
    if (option != 'y' && option != 'Y') {
        return 0;
    }
    printf("Do you want to hard format your device (may be slower)? [Y/n] ");
    getchar();
    scanf("%c", &option);
    if (option != 'y' && option != 'Y') {
        return soft_format(device_name);
    } else {
        return hard_format(device_name);
    }
}

int enter_device(const char *device_name) {
    printf("Reading device...\n");
    return 0;
}

int main(int argc, const char **argv) {
    const char *device_name;

    if (argc < 2) {
        print_error();
        return 1;
    }
    if ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)) {
        print_help();
        return 0;
    }
    if ((strcmp(argv[1], "--format") == 0) || (strcmp(argv[1], "-f") == 0)) {
        if (argc < 3) {
            print_error();
            return 1;
        }
        device_name = argv[2];
        return format_device(device_name);
    }
    if ((strcmp(argv[1], "--enter") == 0) || (strcmp(argv[1], "-e") == 0)) {
        if (argc < 3) {
            print_error();
            return 1;
        }
        device_name = argv[2];
        return enter_device(device_name);
    }

    return 0;
}