#ifndef RCB_FILE_SYSTEM_MESSAGES_H
#define RCB_FILE_SYSTEM_MESSAGES_H

void print_valid_arguments () {
    printf("Arguments:\n");
    printf("--format [device]\t\t\t\t\tFormat a device to the RCBFS\n");
    printf("--enter\t [device]\t\t\t\t\tEnter the device listing files and directories\n");
    printf("--copy\t [file path] [device] \t\tCopy a disk file to the device\n");
    printf("--export [device] [location] \t\tExport a file from the device to the local storage\n");
    printf("--help\t\t\t\t\t\t\t\tDisplay this help message\n");
}

void print_help () {
    printf("    ____  __________ \n"
                   "   / __ \\/ ____/ __ )\n"
                   "  / /_/ / /   / __  |\n"
                   " / _, _/ /___/ /_/ / \n"
                   "/_/ |_|\\____/_____/  \n"
                   "                     \n");
    printf("This is the RCB Filesystem\n\n");
    print_valid_arguments();
}

void print_error () {
    printf("⛔ Invalid arguments\n\n");
    print_valid_arguments();
}

void print_invalid_file (char *error) {
    printf("%s\n", error);
}

void print_invalid_device (char *error) {
    print_invalid_file(error);
    printf("To see available devices: lsblk -o NAME,TYPE,SIZE,MODEL\n");
}

void print_navigator_error () {
    printf("⛔ Invalid command\n\n");
    printf("Type 'help' to list available options.\n");
}

void print_navigator_help () {
    printf("Available commands:\n");
    printf("exit\t\t\t\t\tExit navigator\n");
    printf("pwd\t\t\t\t\t\tDisplay the current directory path\n");
    printf("ls\t\t\t\t\t\tList the current directory\n");
    printf("cd [dir]\t\t\t\tEnter a given directory\n");
    printf("rm [dir|file]\t\t\tRemove a given directory or file\n");
    printf("mkdir [dir]\t\t\t\tCreate a new directory\n");
    printf("mv [file] [dir]\t\t\tMove a file or directory to another directory.\n");
    printf("rnm [dir|file] [name]\tChange the name of a directory or file.\n");
    printf("info\t\t\t\t\tDisplay the device information\n");
    printf("help\t\t\t\t\tDisplay this help message\n");
}

void print_non_rcbfs_device () {
    printf("The device is not on RCBFS. If you want to format it, use the --format option.\n");
}

void print_not_enough_space (unsigned int needed, unsigned int available) {
    printf("⛔ Not enough space to allocate file.\n");
    printf("%d bytes needed.\n", needed);
    printf("%d bytes available.\n", available);
}

void print_sect_size_not_power_of_two (unsigned int actual_size) {
    printf("Warning! The sector size is not a power of two and will be decreased.\n");
    printf("Actual sector size: %d\n", actual_size);
}

void print_insufficient_sect_size (unsigned int actual_size) {
    printf("Warning! The sector size is not sufficient and will be increased.\n");
    printf("Actual sector size: %d\n", actual_size);
}

void print_file_name_repetead () {
    printf("This name already used!\n");
}

void print_dir_is_full () {
    printf("This directory is full!\n");
}

void print_no_such_directory () {
    printf("No such directory.\n");
}

void print_no_such_file () {
    printf("No such file.\n");
}

void print_must_be_absolute_path () {
    printf("The target path must be absolute.\n");
}

void print_invalid_name () {
    printf("Invalid name.\n");
}

#endif
