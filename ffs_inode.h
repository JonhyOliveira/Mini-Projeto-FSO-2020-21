#define FFS_INODE_H

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

// need struct super
#ifndef FFS_SUPER_H 
#include "ffs_super.h"
#endif


// Note that inodes currently fit exactly in one block
#define INODES_PER_BLOCK	(DISK_BLOCK_SIZE / sizeof(struct inode))

#define POINTERS_PER_INODE	6

struct inode {
  unsigned int isvalid;
  unsigned int size;
  unsigned int direct[POINTERS_PER_INODE];
};

union in_block {
  struct inode ino[INODES_PER_BLOCK];
  unsigned char data[DISK_BLOCK_SIZE];
};



/* operations on inode structures

  read: fills in an inode with its disk image
    parameters:
     @in: (absolute) inode number
     @out: pointer to inode structure
    errors:
     those resulting from disk operations

*/

/* Helper function prototypes */


struct inode_operations {
  void (*clear)(struct inode *in);
  int (*read)(unsigned int absDskBlk, unsigned int absinode, struct inode *in);
  int (*printTable)(unsigned int ninodeblocks, unsigned int ninodes,\
                        unsigned int absDskBlk);
  int (*printFileData)(unsigned int startInArea, unsigned int absinode,\
                           unsigned int startDtArea);
};

