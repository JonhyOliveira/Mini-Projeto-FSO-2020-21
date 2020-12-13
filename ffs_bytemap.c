#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef FFS_SUPER_H
#include "ffs_super.h"
extern struct super_operations super_ops;
#endif

#ifndef FFS_BYTEMAP_H
#include "ffs_bytemap.h"
#endif



/* bytemap operations */

static int bytemap_read(struct bytemap *bmap, unsigned int max_entries,\
			unsigned int absDskBlk) {
  int ercode;

  // using the absolute disk block value, read the bytemap block
  if (ercode < 0) return ercode;

  // use max_entries to set the bytemap size;  set index to 0

  return 0;
}

static int bytemap_getNextEntry(struct bytemap *bmap) {
  int entry;

  // return the next value in the map (using index)
  // if the size has been reached return -ENOSPC
  // update the index

  return entry;
}


static int bytemap_setIndex(struct bytemap *bmap, unsigned int value) {

  // may be this function is not needed at all...
  // but, if you need, this should set the index to some value

  return value;
}


void bytemap_printTable(struct bytemap *bmap) {

  int left= bmap->size, entry= 0;

  // prints 16 entries per line
  while (left) {
    if ( (entry+1)%16 ) printf("%u ", bmap->bmap[entry]);
    else printf("%u\n", bmap->bmap[entry]);
    left--; entry++;
  }
  if ( entry%16 ) printf("\n"); // last NL for general case

}


struct bytemap_operations bmap_ops= {
	.read= bytemap_read,
	.getNextEntry= bytemap_getNextEntry,
	.setIndex= bytemap_setIndex,
	.printTable= bytemap_printTable
};
