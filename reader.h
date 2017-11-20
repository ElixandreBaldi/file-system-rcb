#ifndef RCB_FILE_SYSTEM_READER_H
#define RCB_FILE_SYSTEM_READER_H

#include <stdio.h>
#include "data_structures.h"
#include "boot_utils.h"
#include "rcb_utils.h"

navigator nav;

void ls() {
    char *current = nav.current_dir;
    char *dest = (char *) current[1];
    unsigned int pointer_position;
    if (dest == NULL) {
        pointer_position = (unsigned int) (nav.boot.bytes_per_sector * (nav.boot.sectors_per_rcb + 1));
    } else {
        pointer_position = (unsigned int) (nav.boot.bytes_per_sector * (nav.boot.sectors_per_rcb + 1 + DIR_ENTRY));
    }

    for(int i = 0; i < DIR_ENTRY; i++){
        unsigned int type;
        unsigned char name[sizeof(nav.dir.file_name)];
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
        fread(&type, 1, 1, nav.device);
        if( type != DELETED_ATTR && type != EMPTY_ATTR){
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, 1, sizeof(name), nav.device);
            printf("%s\n", name);
        }
    }
}

void pwd() {
    printf("%s\n",nav.current_dir);
}

void cd(const char *target) {
    //
}

void mkdir(const char *target) {

}

void rm(const char *target) {
    unsigned int pointer_position = (unsigned int) (nav.boot.bytes_per_sector * (nav.boot.sectors_per_rcb + 1));
    unsigned short deleted = DELETED_ATTR;
    unsigned short free = EMPTY_SPACE;
    for(int i = 0; i < DIR_ENTRY - 1; i++) {
        unsigned char name[sizeof(nav.dir.file_name)];
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
        fread(&name, sizeof(name), 1, nav.device);
        if (strcmp((const char *) name, target) == 0) {
            unsigned int type;
            unsigned short first_sector, sector_on_table, current_position;
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
            fread(&type, 1, 1, nav.device);
            if (type == FILE_ATTR) {
                fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + FIRST_CLUSTER_POSITION, SEEK_SET);
                fread(&first_sector, SPACE_SIZE, 1, nav.device);
                fseek(nav.device, nav.boot.bytes_per_sector + first_sector, SEEK_SET);
                fread(&sector_on_table, SPACE_SIZE, 1, nav.device);
                unsigned int j = 0;
                while (true) {
                    current_position = navigate(nav.boot.bytes_per_sector + j, nav.device);
                    fseek(nav.device, nav.boot.bytes_per_sector + j - 2, SEEK_SET);
                    if (current_position != EMPTY_SPACE && current_position != RCB_EOF) {
                        j += 2;
                        fwrite(&free, sizeof(free), 1, nav.device);
                    } else {
                        fwrite(&free, sizeof(free), 1, nav.device);
                        break;
                    }
                }
            }
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
            fwrite(&deleted, DELETED_ATTR, 1, nav.device);
        }
    }
}

void info() {
    printf("Device: %s\n", nav.device_name);
    printf("Device size: %li\n", nav.device_size);
    printf("Sector size: %u\n", nav.boot.bytes_per_sector);
}

void help() {
    print_navigator_help();
}

void parse_command(const char *command) {
    char *command_token;
    char *input_token;
    command_token = strtok(command, " ");
    if (strcmp(command_token, "ls") == 0) {
        ls();
    } else if (strcmp(command_token, "pwd") == 0) {
        pwd();
    } else if (strcmp(command_token, "info") == 0) {
        info();
    } else if (strcmp(command_token, "help") == 0) {
        help();
    } else if (strcmp(command_token, "cd") == 0) {
        input_token = strtok(NULL, " ");
        if (input_token != NULL) {
            cd(input_token);
        } else {
            print_navigator_error();
        }
    } else if (strcmp(command_token, "rm") == 0) {
        input_token = strtok(NULL, " ");
        if (input_token != NULL) {
            rm(input_token);
        } else {
            print_navigator_error();
        }
    } else {
        print_navigator_error();
    }
}

void init_nav(const char *device_name) {
    char command[255];
    nav.current_dir = malloc(sizeof(char) * 1);
    strcpy(nav.current_dir, "/");
    do {
        printf("rcbfs> ");
        scanf("%[^\n]s",command);
        getchar();
        if (strcmp(command, "exit") == 0) {
            break;
        } else {
            parse_command(command);
        }
    } while (true);
}

int enter_device(const char *device_name) {
    printf("Reading device...\n");
    nav.device_name = device_name;
    nav.device = fopen(device_name, "rb+");
    if (nav.device == NULL) {
        print_invalid_device(strerror(errno));
        return 1;
    }
    nav.device_size = get_size(nav.device);
    nav.boot = read_boot_record(nav.boot, nav.device);
    if (!is_valid_boot_record(nav.boot)) {
        print_non_rcbfs_device();
        return 1;
    }
    init_nav(device_name);
    fclose(nav.device);
    return 0;
}

int export_file(const char *device_name, const char *target_path) {
    //
}

#endif
