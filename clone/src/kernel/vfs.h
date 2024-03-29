#ifndef VFS_H_
#define VFS_H_

// TODO separate fat32 implementation from this class
// when pure virtual vtable shenanigans are fixed

#include "stdint.h"
#include "ata_block_device.h"
#include "list.h"

#define LFN_BUFFER_LENGTH 257

// 16 bytes
// https://en.wikipedia.org/wiki/Master_boot_record#PTE
struct PartitionEntry {
  uint8_t status; // 0x80 = active/bootable, 0x00 = inactive, else invalid
  uint8_t head_first; // CHS, ignore
  uint8_t sec_high_first; // CHS, ignore
  uint8_t sec_low_first; // CHS, ignore
  uint8_t partition_type;
  uint8_t head_last; // CHS, ignore
  uint8_t sec_high_last; // CHS, ignore
  uint8_t sec_low_last; // CHS, ignore
  uint32_t lba_first_sector;
  uint32_t num_sectors;
} __attribute__((packed));

// http://wiki.osdev.org/FAT#BPB_.28BIOS_Parameter_Block.29
// multi-byte fields are little endian, so uint16_t is ok to use
// 36 bytes
struct BPB {
  uint8_t jmp[3];
  uint8_t oem_id[8];
  uint16_t bytes_per_sector; // assert this is 512 bytes TODO
  uint8_t sectors_per_cluster; // FAT table points to clusters, not sectors!
  uint16_t num_reserved_sectors; // reserved section after BPB, in sectors, for GRUB
  uint8_t num_fats; // number of tables, usually 2
  uint16_t num_directory_entries;
  uint16_t total_sectors; // if 0 then more than 65535 sectors, count total_sectors_large instead
  uint8_t media_descriptor_type;
  uint16_t sectors_per_fat; // fat12/fat16 only. if zero (fat32), then use BootRecord.sectors_per_fat
  uint16_t sectors_per_track;
  uint16_t num_heads;
  uint32_t num_hidden_sectors; // LBA of the beginning of the partition
  uint32_t total_sectors_large; // used if total_sectors == 0
} __attribute__((packed));

// FAT32 Extended Boot Record (EBPB)
struct BootRecord {
  struct BPB bpb;
  uint32_t sectors_per_fat;
  uint8_t flags[2];
  uint8_t major_version;
  uint8_t minor_version;
  uint32_t root_cluster; // cluster number of root directory, usually 2
  uint16_t sector_fsinfo;
  uint16_t sector_backup_boot;
  uint8_t reserved[12];
  uint8_t drive_number;
  uint8_t nt_flags;
  uint8_t signature; // must be 0x28 or 0x29
  uint32_t volumeid_serial_number;
  char volume_label_string[11];
  char system_id_string[8]; // always "FAT32   "
} __attribute__((packed));

// entire MBR block, 512 bytes
struct MBR {
  struct BootRecord boot_record;
  uint8_t boot_code[420 - (16 * 4)];
  struct PartitionEntry partition_entries[4];
  uint16_t boot_partition_signature; // 0x55AA in little endian
} __attribute__((packed));

class Inode;
class Superblock;

class File {
 public:
  File(Inode* inode);

  int Read(uint8_t* dest, uint64_t length);
  int Write(uint8_t* src, uint64_t length);
  int Seek(uint64_t offset);
  int Close();
  // ? static int Close(File** file); 
  

  // todo eh
  uint64_t GetSize();

 private:
  Inode* inode;
  uint64_t offset;
};

class Inode {
 public:
  Inode(uint64_t cluster, char* name, bool is_directory, Superblock* superblock);

  static void Destroy(Inode* inode);

  File* Open();
  List<Inode> ReadDir();
  //int ReadDir(ReadDirCallback callback, void* arg);
  bool IsDirectory();
  char* GetName();

  uint64_t GetSize();
  uint64_t GetCluster();
  Superblock* GetSuperblock();

  // TODO
  //int Unlink(const char* name); // delete files in dir?
  
  // TODO make this private and move to constructor
  // FAT32 specific information
  uint64_t cluster; // points to first cluster of this file
  char filename[LFN_BUFFER_LENGTH];
  uint64_t size;

 private:
  Superblock* superblock;
  bool is_directory;
  // TODO
  /*struct timeval mod_time, acc_time, create_time;
  mode_t st_mode;
  uid_t uid;
  gid_t gid;
  uint64_t ino_num;*/
};

class Superblock {
 public:
  static Superblock* Create(ATABlockDevice* ata_device);
  static void Destroy(Superblock* superblock);

  Inode* GetRootInode();

  // TODO
  /*int SyncFS(); // sync files to disk?
  void PutSuper(); // write superblock to disk*/

  // FAT32 specific information
  uint8_t* ReadCluster(uint64_t cluster); // TODO this should be given a buffer
  uint64_t GetNextCluster(uint64_t cluster);

 private:
  Inode* root_inode;
  // TODO
  /*char* name; // untitled
  char* type; // fat32*/

  // FAT32 specific information
  
  uint64_t GetFatsStartSector();
  uint64_t GetDataRegionStartSector();
  uint64_t ClusterToSector(uint64_t cluster_number);

  ATABlockDevice* ata_device;
  MBR mbr;
  MBR partition;
  uint32_t* fat_table;
};

#endif  // VFS_H_
