#ifndef RCB_FILE_SYSTEM_MESSAGES_H
#define RCB_FILE_SYSTEM_MESSAGES_H

#include<stdio.h>
#include <stdlib.h>

void print_help() {
    printf("    ____  __________ \n"
                   "   / __ \\/ ____/ __ )\n"
                   "  / /_/ / /   / __  |\n"
                   " / _, _/ /___/ /_/ / \n"
                   "/_/ |_|\\____/_____/  \n"
                   "                     \n");
    printf("This is the RCB Filesystem\n\n");
    printf("Arguments:\n");
    printf("--format [device]\t\tFormat a device to the RCBFS\n");
    printf("--enter\t [device]\t\tEnter the device listing files and directories\n");
    printf("--help\t\t\t\t\tDisplay this help message\n");
}

void print_error() {
    printf("⛔ Invalid arguments\n\n");
    printf("Arguments:\n");
    printf("--format [device]\t\tFormat a device to the RCBFS\n");
    printf("--enter\t [device]\t\tEnter the device listing files and directories\n");
    printf("--help\t\t\t\t\tDisplay help message\n");
}

void print_invalid_device() {
    printf("Invalid device\n\n");
    printf("Available devices:\n");
    system("lsblk -o NAME,TYPE,SIZE,MODEL");
}

#endif
