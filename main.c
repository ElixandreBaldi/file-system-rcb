#include <string.h>
#include "messages.h"
#include "format_utils.h"
#include "reader.h"
#include "writer.h"

int main(int argc, const char **argv) {
    const char *device_name;
    const char *target_file;
    const char *origin;

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
        if(argv[4]) {
            origin = argv[4];
        } else {
            origin = "/";
        }
        return copy_file(target_file, device_name, origin);
    }

    return 0;
}
