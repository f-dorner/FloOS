#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stdbool.h>

#define FS_MAGIC 0x46465331 // FFS1 -> FloOS File System version 1
#define FS_HEADER_SECTOR 100
#define FS_DATA_START_SECTOR 101
#define FS_MAX_FILES 9
#define FS_SECTOR_SIZE 512

typedef struct file_entry
{
    char name[32];
    uint32_t start_sector;
    uint32_t size;
    uint32_t sector_count;
    uint8_t used;
} FileEntry;

typedef struct fs_header
{
    uint32_t magic;
    FileEntry entries[FS_MAX_FILES];
} FsHeader;


void fs_init(void);
void fs_format(void);
int fs_create(const char* name);
int fs_write(const char* name, const char* content);
int fs_append(const char* name, const char* content);
int fs_delete(const char* name, bool force);
int fs_clear(const char* name);
int fs_read(const char* name, char* out_buffer);
int fs_get_info(const char* name, FileEntry* out_entry);
int fs_get_entry(int index, FileEntry* out_entry);

#endif
