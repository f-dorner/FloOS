#include <disk.h>
#include <io.h>

/* 
ATA primary channel ports, not used in this order
as some ports with a higher port number have to be used first
*/

#define ATA_DATA        0x1F0 // DATA of a sector
#define ATA_SECTOR_CNT  0x1F2 // how many sectors should be read or written
#define ATA_LBA_LOW     0x1F3 // lowest 8 bits of the sector number
#define ATA_LBA_MID     0x1F4 // middle 8 bits of the sector number
#define ATA_LBA_HIGH    0x1F5 // higher 8 bits of the sector number
#define ATA_DRIVE       0x1F6 // drive selection, lba/chs and highest 4 bits of the sector number
#define ATA_STATUS      0x1F7 // current disk status when read
#define ATA_COMMAND     0x1F7 // command for the disk when written

#define ATA_CMD_READ    0x20
#define ATA_CMD_WRITE   0x30
#define ATA_CMD_FLUSH   0xE7

#define ATA_STATUS_BSY  0x80
#define ATA_STATUS_DRQ  0x08

static void disk_wait_bsy(void)
{
    while (inb(ATA_STATUS) & ATA_STATUS_BSY)
    {
        
    }
}

static void disk_wait_drq(void)
{
    while (!(inb(ATA_STATUS) & ATA_STATUS_DRQ))
    {
        
    }    
}

void disk_write_sector(uint32_t lba, const uint8_t* buffer)
{
    //wait until the disk isnt busy
    disk_wait_bsy();

    /*
    this line activates lba mode, selects to use the master disk 
    and inserts the 4 highest bits of lba into the drive register
    */ 
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));

    //count of sectors to read
    outb(ATA_SECTOR_CNT, 1);

    //lowest byte of lba inserted into register
    outb(ATA_LBA_LOW, (uint8_t) lba);

    //mid byte of lba inserted into register
    outb(ATA_LBA_MID, (uint8_t) (lba >> 8));

    //higher byte of lba inserted into register
    outb(ATA_LBA_HIGH, (uint8_t) (lba >> 16));

    //ATA command; 0x30 = WRITE SECORS
    outb(ATA_COMMAND, ATA_CMD_WRITE);

    //waiting until disk accepts 
    disk_wait_bsy();

    //waiting until disk awaits data
    disk_wait_drq();

    //ATA writes data in 16-bit words
    const uint16_t* ptr = (const uint16_t*) buffer;

    //512 bytes per sector
    //512 / 2 = 256 words
    //outw() writes 2 bytes/1 word
    for (int i = 0; i < 256; i++)
    {
        outw(ATA_DATA, ptr[i]);
    }

    //actually writing the data on the disk
    outb(ATA_COMMAND, ATA_CMD_FLUSH);
    disk_wait_bsy();
}

void disk_read_sector(uint32_t lba, uint8_t* buffer)
{
    //wait until the disk isnt busy
    disk_wait_bsy();

    /*
    this line activates lba mode, selects to use the master disk 
    and inserts the 4 highest bits of lba into the drive register
    */ 
    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));

    //count of sectors to read
    outb(ATA_SECTOR_CNT, 1);

    //lowest byte of lba inserted into register
    outb(ATA_LBA_LOW, (uint8_t) lba);

    //mid byte of lba inserted into register
    outb(ATA_LBA_MID, (uint8_t) (lba >> 8));

    //higher byte of lba inserted into register
    outb(ATA_LBA_HIGH, (uint8_t) (lba >> 16));

    //ATA command; 0x20 = READ SECTORS
    outb(ATA_COMMAND, ATA_CMD_READ);

    //waiting until disk accepts 
    disk_wait_bsy();

    //waiting until data is ready
    disk_wait_drq();

    //ATA delivers data in 16-bit words
    uint16_t* ptr = (uint16_t*) buffer;

    //512 bytes per sector
    //512 / 2 = 256 words
    // inw() reads 2 bytes/1 word
    for(int i = 0; i < 256; i++)
    {
        //reading one 16-bit word from the data port
        ptr[i] = inw(ATA_DATA);
    }
}
