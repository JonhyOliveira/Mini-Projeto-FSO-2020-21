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
  int (*getNextEntry)(struct bytemap *bmap);
  int (*setIndex)(struct bytemap *bmap, unsigned int index);
  void (*printTable)(struct bytemap *bmap);
};

