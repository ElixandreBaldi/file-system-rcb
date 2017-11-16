#ifndef RCB_FILE_SYSTEM_READER_H
#define RCB_FILE_SYSTEM_READER_H

#include <stdio.h>
#include "struct_utils.h"

typedef struct nav{
    const char *device_name;
    FILE *device;
    long device_size;
    struct boot_record boot;
    struct root_dir;
} navigator;

navigator nav;

void read_boot(){
    fread(&nav.boot, 1, sizeof(nav.boot), nav.device);
}

bool is_valid_boot_record() {
    if( strcmp(nav.boot.rcb, SIGNATURE) != 0){
        return false;
    }
    return nav.boot.sectors_per_rcb && nav.boot.bytes_per_partition && nav.boot.entry_directory &&
           nav.boot.reserved_sectors && nav.boot.sectors_per_disk && nav.boot.bytes_per_sector;
}

void ls() {
    //
}

void pwd() {
    //
}

void cd(const char* target) {
    //
}

void rm(const char* target) {
    //
}

void info() {
    printf("Device: %s\n", nav.device_name);
    printf("Device size: %li\n", nav.device_size);
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
        input_token = strtok (NULL, " ");
        if (input_token != NULL) {
            cd(input_token);
        } else {
            print_navigator_error();
        }
    } else if (strcmp(command_token, "rm") == 0) {
        input_token = strtok (NULL, " ");
        if (input_token != NULL) {
            rm(input_token);
        } else {
            print_navigator_error();
        }
    } else {
        print_navigator_error();
    }
}

void init_nav() {
    char command[255];
    do {
        printf("rcbfs> ");
        scanf("%s", command);
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
    if(nav.device == NULL){
        print_invalid_device(strerror(errno));
        return 1;
    }
    nav.device_size = get_device_size(nav.device);
    read_boot();
    if (!is_valid_boot_record()) {
        printf("The device is not on RCBFS. If you want to format it, use the --format option.\n");
        return 1;
    }
    init_nav();
    return 0;
}

#endif
