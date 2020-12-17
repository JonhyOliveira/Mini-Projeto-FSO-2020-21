#define FFS_SUPER_H

/* Disk layout
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+
   | SuB | BMi | Ino | Dir | BMd | Dt1 | Dt2 | ... | END |
   +-----+-----+-----+-----+-----+-----+-----+-----+-----+
*/

#define SB_OFFSET	0
#define BMi_OFFSET	1

#define FS_MAGIC	0xf0f03410 // 4042273808

struct super {
  unsigned int fsmagic;
  unsigned int nblocks;
  unsigned int nbmapblocksinodes;	// Always 1 in BFS
  unsigned int ninodeblocks;
  unsigned int ninodes;
  unsigned int nbmapblocksdata;		// Always 1 in BFS
  unsigned int ndatablocks;
  unsigned int startInArea;
  unsigned int startDtBmap;
  unsigned int startDtArea;
  unsigned int mounted;
};

union sb_block {
  struct super sb;
  unsigned char data[DISK_BLOCK_SIZE];
};


/* operations on superblock structures

  read: reads in a superblock from disk
    parameters:
     @out: pointer to superblock structure

    errors:
     resulting from disk read

  check: checks the integrity of a superblock
    parameters:
     @in: pointer to a superblock structure
     @out: pointer to valid superblock structure, if no errors were reported
    
    errors:
     -1: invalid magic number
     -1: superblock size does not respond to actual disk size



  print: prints a superblock with minimum information
    parameters:
     @in: pointer to superblock structure

  debug: prints a superblock with extra information
    parameters:
     @in: pointer to superblock structure

*/


struct super_operations {
  int (*read)(struct super *sb);
  int (*checkIntegrity)(struct super *sb);
  void (*print)(const struct super *sb);
  void (*debug)(const struct super *sb);
};
