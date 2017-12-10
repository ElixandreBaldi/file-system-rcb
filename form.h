#ifndef RCB_FILE_SYSTEM_FORM_H
#define RCB_FILE_SYSTEM_FORM_H


unsigned int root_begin (unsigned short bytes_per_sector, unsigned int sectors_per_rcb) {
    return bytes_per_sector * (sectors_per_rcb + 1);
}

unsigned int rcb_goto (unsigned short bytes_per_sector, unsigned short cluster) {
    return (unsigned int) (bytes_per_sector + (cluster * SPACE_SIZE));
}

unsigned short first_cluster (FILE *device, unsigned int pointer_position) {
    unsigned short cluster;
    fseek(device, pointer_position + FIRST_CLUSTER_POSITION, SEEK_SET);
    fread(&cluster, sizeof(cluster), 1, device);
    return cluster;
}

unsigned int
data_section_begin (unsigned short bytes_per_sector, unsigned int sectors_per_rcb, unsigned int sectors_per_dir,
                    unsigned short cluster) {
    return bytes_per_sector * (sectors_per_rcb + 1 + sectors_per_dir + cluster);
}

unsigned int sectors_per_dir (unsigned int bytes_per_sector) {
    return (DIR_ENTRY * RCB_DATA_TABLE) / bytes_per_sector;
}

#endif