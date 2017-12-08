#ifndef RCB_FILE_SYSTEM_FORM_H
#define RCB_FILE_SYSTEM_FORM_H


unsigned int root_begin (unsigned short bytes_per_sector, unsigned int sectors_per_rcb) {
    return bytes_per_sector * (sectors_per_rcb + 1);
}

unsigned int rcb_goto (unsigned short bytes_per_sector, unsigned short cluster) {
    return (unsigned int) (bytes_per_sector + (cluster * SPACE_SIZE));
}

unsigned int
data_section_begin (unsigned short bytes_per_sector, unsigned int sectors_per_rcb, unsigned int sectors_per_dir,
                    unsigned short cluster) {
    printf("Bytes per sector %d\n", bytes_per_sector);
    printf("Sector per rcb %d\n", sectors_per_rcb);
    printf("Sector per dir %d\n", sectors_per_dir);
    printf("Cluster form %d\n", cluster);
    return bytes_per_sector * (sectors_per_rcb + 1 + sectors_per_dir + cluster);
}

unsigned int sectors_per_dir (unsigned int bytes_per_sector) {
    return (DIR_ENTRY * RCB_DATA_TABLE) / bytes_per_sector;
}

#endif