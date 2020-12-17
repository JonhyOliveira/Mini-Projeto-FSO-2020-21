#define FFS_BYTEMAP_H

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif


// Declare as many variables of this structure as bytemaps you want.
struct bytemap {
  unsigned char bmap[DISK_BLOCK_SIZE];
  unsigned int size;
  unsigned int index;
};


/* operations on bytemaps

  read: read a bytemap, set the index to zero
    parameters:
      @in: the array to store the bytemap
	   the number of valid entries, i.e., bytemap size
	   the physical disk block where the bytemap is stored
    errors:
     those resulting from disk operations

  checkInodeIntegrity: checks the integrity of a inode bytemap 
  note: should be called after data integrity has been confirmed, but no necessary
    parameters:
     @in: pointer to an inode bytemap structure
     @out: pointer to a valid inode bytemap structure, if no errors were reported

     errors:
     realted to inode read
     -1: unexpected bytemap value (should be 1 or 0)
     -1: inode bytemap expected inode be valid/invalid, inode is invalid/valid

  checkDataIntegrity: check the integrity of a data bytemap
    parameters:
     @in: pointer to a data bytemap structure
     @out: pointer to a valid data bytemap structure, if no errors were reported

    errors:
     related to disk driver read
     -1: data bytemap expected data block be empty/contain data, data block contains data/is empty

  getNextEntry: get the next entry on a bytemap array
		uses the current index in the array, then increments it
    parameters:
      @in: the array we are accessing
     @out: function return: entry value
    errors:
     -ENOSPC there are no more entries


  setIndex: set the index
    parameters:
      @in: the array we are accessing, the value for the index
    errors:
     -EINVAL invalid index


  bytemap_print_table: prints the full table, up to 16 entries per line
    parameter:
      @in: the array that holds the bytemap

*/



struct bytemap_operations {
  int (*read)(struct bytemap *bmap, unsigned int size,\
		unsigned int blockNbr);
  int (*checkInodeIntegrity)(struct bytemap *bmap, struct super *sb);
  int (*checkDataIntegrity)(struct bytemap *bmap, struct super *sb);
  int (*getNextEntry)(struct bytemap *bmap);
  int (*setIndex)(struct bytemap *bmap, unsigned int index);
  void (*printTable)(struct bytemap *bmap);
};

