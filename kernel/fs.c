#include "fs.h"
#include "string.h"
#include "disk.h"

#include <stdbool.h>

void fs_init(void)
{ 
    uint8_t buffer[FS_SECTOR_SIZE];
    disk_read_sector(FS_HEADER_SECTOR, buffer);

    FsHeader* fs = (FsHeader*) buffer;

    //if the magic number matches, the disk is set up
    if (fs->magic == FS_MAGIC) {
        return;
    }

    fs->magic = FS_MAGIC;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        fs->entries[i].used = 0;
        fs->entries[i].name[0] = '\0';
        fs->entries[i].start_sector = 0;
        fs->entries[i].sector_count = 0;
        fs->entries[i].size = 0;
    }

    disk_write_sector(FS_HEADER_SECTOR, buffer);

}

void fs_format(void)
{
    uint8_t buffer[FS_SECTOR_SIZE];
    
    //treat the raw sector as an FsHeader
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    fs->magic = FS_MAGIC;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        entries[i].used = 0;
        entries[i].name[0] = '\0';
        entries[i].start_sector = 0;
        entries[i].sector_count = 0;
        entries[i].size = 0;
    }

    disk_write_sector(FS_HEADER_SECTOR, buffer);
}

int fs_create(const char* name)
{
    uint8_t buffer[FS_SECTOR_SIZE];
    
    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                return 1;
            }
        }
    }

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 0) {
            strcpy(entries[i].name, name);
            //entry 0 -> sector 101, entry 1 -> sector 102 and so on
            entries[i].start_sector = (FS_DATA_START_SECTOR + (i * FS_SECTORS_PER_FILE));
            entries[i].size = 0;
            entries[i].sector_count = 1;
            entries[i].used = 1;

            disk_write_sector(FS_HEADER_SECTOR, buffer);
            return 0;
        }
    }

    return 2;
}

int fs_write(const char* name, const char* content)
{
    uint8_t buffer[FS_SECTOR_SIZE];
    
    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                uint32_t content_size = strlen(content);
                uint32_t needed_sectors = (content_size + FS_SECTOR_SIZE - 1) / FS_SECTOR_SIZE;

                for (uint32_t sector = 0; sector < needed_sectors; sector++) {
                    uint8_t content_buffer[FS_SECTOR_SIZE];

                    //clear buffer
                    for (int i = 0; i < FS_SECTOR_SIZE; i++) {
                        content_buffer[i] = 0;
                    }

                    for (uint32_t j = 0; j < FS_SECTOR_SIZE; j++) {
                        uint32_t content_index = sector * FS_SECTOR_SIZE + j;

                        if (content_index < content_size) {
                            content_buffer[j] = content[content_index];
                        } else {
                            content_buffer[j] = 0;
                        }
                    }

                    disk_write_sector(entries[i].start_sector + sector, content_buffer);
                }

                entries[i].size = content_size;
                entries[i].sector_count = needed_sectors;
                disk_write_sector(FS_HEADER_SECTOR, buffer);
                return 0;
            }
        } 
    }

    return 1;
}

int fs_delete(const char* name, bool force)
{
    uint8_t buffer[FS_SECTOR_SIZE];

    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                uint8_t content_buffer[FS_SECTOR_SIZE];
                for (int i = 0; i < FS_SECTOR_SIZE; i++) {
                    content_buffer[i] = 0;
                }

                disk_write_sector(entries[i].start_sector, content_buffer);
                entries[i].used = 0;
                entries[i].name[0] = '\0';
                entries[i].start_sector = 0;
                entries[i].size = 0;
                entries[i].sector_count = 0;
                disk_write_sector(FS_HEADER_SECTOR, buffer);
                return 0;
            } 
        } 
    }

    if (force == true) {
        return 2;
    } else {
        return 1;
    }
}

int fs_clear(const char* name)
{
    uint8_t buffer[FS_SECTOR_SIZE];

    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                uint8_t content_buffer[FS_SECTOR_SIZE];
                for (int i = 0; i < FS_SECTOR_SIZE; i++) {
                    content_buffer[i] = 0;
                }

                disk_write_sector(entries[i].start_sector, content_buffer);
                entries[i].size = 0;
                entries[i].sector_count = 1;
                disk_write_sector(FS_HEADER_SECTOR, buffer);
                return 0;
            }
        }
    }

    return 1;
}

int fs_read(const char* name, char* out_buffer, uint32_t* out_size)
{
    uint8_t buffer[FS_SECTOR_SIZE];

    disk_read_sector(FS_HEADER_SECTOR, buffer);
    //fs_read only returns data, commands.c decides how to print it
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                for (uint32_t sector = 0; sector < entries[i].sector_count; sector++) { 
                    disk_read_sector(
                        entries[i].start_sector + sector, 
                        (uint8_t*) out_buffer + (sector * FS_SECTOR_SIZE)
                    );
                }
                *out_size = entries[i].size;
                return 0;
            }
        }  
    }
    
    return 1;
}

int fs_get_info(const char* name, FileEntry* out_entry)
{
    uint8_t buffer[FS_SECTOR_SIZE];

    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (entries[i].used == 1) {
            if (strcmp(entries[i].name, name) == 0) {
                *out_entry = entries[i];
                return 0;
            }
        }
    }

    return 1;
}

int fs_get_entry(int index, FileEntry* out_entry)
{
    uint8_t buffer[FS_SECTOR_SIZE];

    //checks if index is out of bounds
    if (index < 0 || index >= FS_MAX_FILES) {
        return 1;
    }

    disk_read_sector(FS_HEADER_SECTOR, buffer);
    FsHeader* fs = (FsHeader*) buffer;
    FileEntry* entries = fs->entries;

    *out_entry = entries[index];
    return 0;
}
