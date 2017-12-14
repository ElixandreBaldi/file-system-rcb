#ifndef RCB_FILE_SYSTEM_GENERIC_UTILS_H
#define RCB_FILE_SYSTEM_GENERIC_UTILS_H


#define RCB_TABLE_SIZE 65536
#define RCB_DATA_TABLE 32
#define SIGNATURE ":3"
#define EMPTY_SPACE 0xFFFE
#define RCB_EOF 0xFFFF
#define SPACE_SIZE 2
#define BTR_SIZE 16
#define DIR_ENTRY 512
#define ENTRY_SIZE 32
#define FILE_ATTR 0x08
#define DIRECTORY_ATTR 0x01
#define HIDDEN_ATTR 0x03
#define DELETED_ATTR 0x07
#define EMPTY_ATTR 0x00
#define TYPE_POSITION 25
#define FILE_NAME_SIZE 25
#define FIRST_CLUSTER_POSITION 27


unsigned long get_size (FILE *stream) {
    unsigned long size;
    fseek(stream, 0L, SEEK_END);
    size = (unsigned long) ftell(stream);
    rewind(stream);

    return size;
}

bool is_power_of_2 (unsigned int x) {
    return ((x != 0) && !(x & (x - 1)));
}

unsigned int parse_sect (unsigned int sect_size) {
    if (sect_size < BTR_SIZE) {
        print_insufficient_sect_size(BTR_SIZE);
        return BTR_SIZE;
    }
    if (!is_power_of_2(sect_size)) {
        unsigned int res;
        res = (unsigned int) pow(2, floor(log2(sect_size)));
        print_sect_size_not_power_of_two(res);
        return res;
    }

    return sect_size;
}

const char *last_token (const char *content) {
    int      divider_index = -1;
    int      reslen        = 0;
    for (int i             = (int) (strlen(content) - 1); i >= 0; i--) {
        reslen++;
        if (content[i] == '/') {
            divider_index = i;
            break;
        }
    }

    return &content[divider_index + 1];
}

unsigned int seek_rcb (FILE *device, unsigned int position) {
    unsigned int value;
    fseek(device, position, SEEK_SET);
    fread(&value, 1, 1, device);
    return value;
}

#endif
