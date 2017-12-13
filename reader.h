#ifndef RCB_FILE_SYSTEM_READER_H
#define RCB_FILE_SYSTEM_READER_H

#include "writer.h"

static navigator nav;

void ls () {
    char           *current         = nav.current_dir;
    char           *dest            = (char *) current[1];
    unsigned int   pointer_position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb);
    unsigned int   size;
    unsigned short cluster;
    if (dest == NULL) {
        size = DIR_ENTRY;
    } else {
        size = nav.boot.bytes_per_sector;
        int i;
        for (i = 0; i < size; i++) {
            unsigned char name[sizeof(&nav.current_dir[1])];
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, sizeof(name), 1, nav.device);
            if (strcmp((const char *) name, &nav.current_dir[1]) == 0x0) {
                break;
            }
        }
        fseek(nav.device, pointer_position + FIRST_CLUSTER_POSITION + (i * ENTRY_SIZE), SEEK_SET);
        fread(&cluster, sizeof(cluster), 1, nav.device);
        pointer_position = data_section_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb,
                                              sectors_per_dir(nav.boot.bytes_per_sector), cluster);

    }
    for (int i = 0; i < size / ENTRY_SIZE; i++) {
        unsigned int  type;
        unsigned char name[sizeof(nav.dir.file_name)];
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
        fread(&type, 1, 1, nav.device);
        if (type != DELETED_ATTR && type != EMPTY_ATTR) {
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, 1, sizeof(name), nav.device);
            if (strcmp((const char *) name, "") != 0) {
                printf("%s\n", name);
            }
        }
    }
}

void pwd () {
    printf("%s\n", nav.current_dir);
}

bool cd (FILE *device, unsigned short bytes_per_sector, unsigned short sectors_per_rcb, const char *dir_path_rcb,
         char *current_dir) {
    unsigned int pointer_position = root_begin(bytes_per_sector, sectors_per_rcb);
    if (strcmp(current_dir, "/") == 0x0) {
        for (int i = 0; i < DIR_ENTRY - 1; i++) {
            unsigned char name[FILE_NAME_SIZE];
            fseek(device, pointer_position, SEEK_SET);
            fread(&name, sizeof(name), 1, device);
            name[strlen(dir_path_rcb) + 1] = '\0';
            if (strcmp((const char *) name, dir_path_rcb) == 0x0) {
                strcat(current_dir, dir_path_rcb);
                return true;
            }
            pointer_position += ENTRY_SIZE;
        }
    } else if (strcmp(dir_path_rcb, "..") == 0x0) {
        for (int i = (int) strlen(current_dir); i >= 0; i--) {
            if (current_dir[i] == '/') {
                current_dir[i + 1] = '\0';
                if (strlen(current_dir) > 1) {
                    current_dir[i] = '\0';
                }
            }
        }
        return true;
    }
    print_no_such_directory();
    return false;
}

bool mv (const char *source, const char *target) {
    if (target[0] != '/') {
        print_must_be_absolute_path();
        return false;
    }

    char           *p = strtok((char *) target, "/");
    unsigned int   pointer_position;
    unsigned short target_dir_cluster;
    unsigned short file_being_moved_cluster;
    bool found_target_dir  = false;
    bool found_source_file = false;
    bool moving_to_root    = true;
    int            i  = 0;
    char           *current;
    char           *dest;
    unsigned int   size;
    unsigned short bkp_attr;
    root_dir       bkp_entry;


    // verify if the target dir exists
    pointer_position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb);
    if (p != NULL) {
        moving_to_root = false;
        for (int i = 0; i < DIR_ENTRY - 1; i++) {
            unsigned char name[FILE_NAME_SIZE + 1];
            fseek(nav.device, pointer_position, SEEK_SET);
            fread(&name, sizeof(name), 1, nav.device);
            name[FILE_NAME_SIZE] = '\0';
            if (strcmp((const char *) name, p) == 0x0) {
                found_target_dir = true;
                fseek(nav.device, pointer_position + FIRST_CLUSTER_POSITION, SEEK_SET);
                fread(&target_dir_cluster, sizeof(target_dir_cluster), 1, nav.device);
                break;
            }
            pointer_position += ENTRY_SIZE;
        }
        if (!found_target_dir || strtok(NULL, "/") != NULL) {
            print_no_such_directory();
            return false;
        }
    }

    // verify if the current file exists
    pointer_position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb);
    current          = nav.current_dir;
    dest             = &current[1];
    if (dest[0] == '\0') {
        size = DIR_ENTRY;
    } else {
        size   = nav.boot.bytes_per_sector;
        for (i = 0; i < size; i++) {
            unsigned char name[sizeof(dest)];
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, sizeof(name), 1, nav.device);
            if (strcmp((const char *) name, dest) == 0x0) {
                break;
            }
        }
        fseek(nav.device, pointer_position + FIRST_CLUSTER_POSITION + (i * ENTRY_SIZE), SEEK_SET);
        fread(&file_being_moved_cluster, sizeof(file_being_moved_cluster), 1, nav.device);
        pointer_position = data_section_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb,
                                              sectors_per_dir(nav.boot.bytes_per_sector), file_being_moved_cluster);
    }
    for (i           = 0; i < size / ENTRY_SIZE; i++) {
        unsigned int  type;
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
        fread(&type, 1, 1, nav.device);
        if (type != DELETED_ATTR && type != EMPTY_ATTR && type == FILE_ATTR) {
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&bkp_entry, 1, sizeof(bkp_entry), nav.device);
            if (strcmp((const char *) bkp_entry.file_name, source) == 0x0) {
                bkp_attr = bkp_entry.attribute_of_file;
                bkp_entry.attribute_of_file = DELETED_ATTR;
                fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
                fwrite(&bkp_entry, 1, sizeof(bkp_entry), nav.device);
                bkp_entry.attribute_of_file = bkp_attr;
                found_source_file = true;
                break;
            }
        }
    }
    if (!found_source_file) {
        print_no_such_file();
        return false;
    }

    if (moving_to_root) {
        pointer_position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb)  + TYPE_POSITION;
        for (i           = 0; i < DIR_ENTRY; i++) {
            unsigned int value = 0;
            value = seek_rcb(nav.device, pointer_position + (i * ENTRY_SIZE));
            fflush(nav.device);
            if (value == EMPTY_ATTR || value == DELETED_ATTR) break;
        }
        fseek(nav.device, (pointer_position - TYPE_POSITION) + (i * ENTRY_SIZE), SEEK_SET);
        fwrite(&bkp_entry, 1, sizeof(bkp_entry), nav.device);
        fflush(nav.device);
    }

    return true;
}

bool rnm (const char *current_name, const char *new_name) {
    size_t length            = strlen(new_name);
    if (!length || length > FILE_NAME_SIZE) {
        print_invalid_name();
        return false;
    }

    const char     name_to_write[FILE_NAME_SIZE];
    unsigned int   pointer_position;
    unsigned short file_being_renamed_cluster;
    bool   found_source_file = false;
    int            i         = 0;
    char           *current;
    char           *dest;
    unsigned int   size;

    strcpy((char *) name_to_write, new_name);

    pointer_position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb);
    current          = nav.current_dir;
    dest             = &current[1];
    if (dest[0] == '\0') {
        size = DIR_ENTRY;
    } else {
        size   = nav.boot.bytes_per_sector;
        for (i = 0; i < size; i++) {
            unsigned char name[sizeof(dest)];
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, sizeof(name), 1, nav.device);
            if (strcmp((const char *) name, dest) == 0x0) {
                break;
            }
        }
        fseek(nav.device, pointer_position + FIRST_CLUSTER_POSITION + (i * ENTRY_SIZE), SEEK_SET);
        fread(&file_being_renamed_cluster, sizeof(file_being_renamed_cluster), 1, nav.device);
        pointer_position = data_section_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb,
                                              sectors_per_dir(nav.boot.bytes_per_sector), file_being_renamed_cluster);
    }

    for (i = 0; i < size / ENTRY_SIZE; i++) {
        unsigned int  type;
        unsigned char name[sizeof(nav.dir.file_name)];
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE) + TYPE_POSITION, SEEK_SET);
        fread(&type, 1, 1, nav.device);
        if (type != DELETED_ATTR && type != EMPTY_ATTR) {
            fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
            fread(&name, 1, sizeof(name), nav.device);
            if (strcmp((const char *) name, current_name) == 0x0) {
                fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
                fwrite(name_to_write, 1, sizeof(name_to_write), nav.device);
                found_source_file = true;
                break;
            }
        }
    }
    if (!found_source_file) {
        print_no_such_file();
        return false;
    }

    return true;
}

void mkdir (const char *target) { // TODO criar funcao para nao inserir nomes iguais
    if (strlen(target) > FILE_NAME_SIZE) {
        print_invalid_name();
        return;
    }
    const char name_to_write[FILE_NAME_SIZE];
    strcpy((char *) name_to_write, target);
    read_rcb(nav.device, nav.boot.bytes_per_sector);
    unsigned short *spaces;
    unsigned int   position = root_begin(nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb) + TYPE_POSITION;
    if (free_positions(1)) {
        spaces = get_free_spaces(1, nav.boot.reserved_sectors);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"
        allocate_rcb_for_file(spaces, 1, nav.device, nav.boot.bytes_per_sector);
#pragma clang diagnostic pop
        int i;
        for (i = 0; i < DIR_ENTRY; i++) {
            unsigned int value = 0;
            value = seek_rcb(nav.device, position + (i * ENTRY_SIZE));
            fflush(nav.device);
            if (value == EMPTY_ATTR || value == DELETED_ATTR) break;
        }
        strcpy(nav.dir.file_name, name_to_write);
        nav.dir.first_cluster     = spaces[0];
        nav.dir.size_of_file      = 0;
        nav.dir.attribute_of_file = DIRECTORY_ATTR;
        fseek(nav.device, (position - TYPE_POSITION) + (i * ENTRY_SIZE), SEEK_SET);
        fwrite(&nav.dir, 1, sizeof(root_dir), nav.device);
        fflush(nav.device);
    }
}

void rm (const char *target) {
    unsigned int   pointer_position = (unsigned int) (nav.boot.bytes_per_sector * (nav.boot.sectors_per_rcb + 1));
    unsigned short deleted          = DELETED_ATTR;
    unsigned short free             = EMPTY_SPACE;
    for (int       i                = 0; i < DIR_ENTRY - 1; i++) {
        unsigned char name[sizeof(nav.dir.file_name)];
        fseek(nav.device, pointer_position + (i * ENTRY_SIZE), SEEK_SET);
        fread(&name, sizeof(name), 1, nav.device);
        if (strcmp((const char *) name, target) == 0) {
            unsigned int   type;
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

void info () {
    printf("Device: %s\n", nav.device_name);
    printf("Device size: %li\n", nav.device_size);
    printf("Sector size: %u\n", nav.boot.bytes_per_sector);
}

void help () {
    print_navigator_help();
}

void parse_command (const char *command) {
    char *command_token;
    char *input_token;
    char *target_token;
    command_token = strtok((char *) command, " ");
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
            cd(nav.device, nav.boot.bytes_per_sector, nav.boot.sectors_per_rcb, input_token,
               nav.current_dir);
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
    } else if (strcmp(command_token, "mkdir") == 0) {
        input_token = strtok(NULL, " ");
        if (input_token != NULL) {
            mkdir(input_token);
        } else {
            print_navigator_error();
        }
    } else if (strcmp(command_token, "mv") == 0) {
        input_token = strtok(NULL, " ");
        if (input_token != NULL) {
            target_token = strtok(NULL, " ");
            if (target_token != NULL) {
                mv(input_token, target_token);
            } else {
                print_navigator_error();
            }
        } else {
            print_navigator_error();
        }
    } else if (strcmp(command_token, "rnm") == 0) {
        input_token = strtok(NULL, " ");
        if (input_token != NULL) {
            target_token = strtok(NULL, " ");
            if (target_token != NULL) {
                rnm(input_token, target_token);
            } else {
                print_navigator_error();
            }
        } else {
            print_navigator_error();
        }
    } else {
        print_navigator_error();
    }
}

void init_nav () {
    char command[255];
    nav.current_dir = malloc(sizeof(char[2]));
    strcpy(nav.current_dir, "/\0");
    do {
        printf("rcbfs> ");
        scanf("%[^\n]s", command);
        getchar();
        if (strcmp(command, "exit") == 0) {
            break;
        } else {
            parse_command(command);
        }
    } while (true);
}

int enter_device (const char *device_name) {
    printf("Reading device...\n");
    nav.device_name = device_name;
    nav.device      = fopen(device_name, "rb+");
    if (nav.device == NULL) {
        print_invalid_device(strerror(errno));
        return 1;
    }
    nav.device_size = get_size(nav.device);
    nav.boot        = read_boot_record(nav.boot, nav.device);
    if (!is_valid_boot_record(nav.boot)) {
        print_non_rcbfs_device();
        return 1;
    }
    init_nav();
    fclose(nav.device);
    return 0;
}

int export_file (const char *device_name, const char *target_path) {
    //
}

#endif
