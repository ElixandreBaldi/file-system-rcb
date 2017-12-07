#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "messages.h"
#include "generic_utils.h"
#include "form.h"
#include "data_structures.h"
#include "boot_utils.h"
#include "rcb_utils.h"
#include "format_utils.h"
#include "writer.h"


int main(int argc, const char **argv) {
    const char *device_name;
    const char *target_file;
    const char *origin;
    const char *path;

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
    if ((strcmp(argv[1], "--export") == 0) || (strcmp(argv[1], "-x") == 0)) {
        if (argc < 4) {
            print_error();
            return 1;
        }
        device_name = argv[2];
        target_file = argv[3];
        return export_file(device_name, target_file);
    }
    if ((strcmp(argv[1], "--enter") == 0) || (strcmp(argv[1], "-e") == 0)) {
        if (argc < 3) {
            print_error();
            return 1;
        }
        device_name = argv[2];
        return enter_device(device_name);
    }
    if ((strcmp(argv[1], "--copy") == 0) || (strcmp(argv[1], "-c") == 0)) {
        if (argc < 4) {
            print_error();
            return 1;
        }
        target_file = argv[2];
        device_name = argv[3];
        if(argc >= 5) {
            origin = argv[4];
        } else {
            origin = "/";
        }
        return copy_file(target_file, device_name, (char *) origin);
    }

    return 0;
}
