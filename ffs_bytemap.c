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

  ercode = disk_ops.read(absDskBlk, bmap->bmap);
  if (ercode < 0) return ercode;

  // reset values
  bmap->size = max_entries;
  bmap->index = 0;

  return 0;
}

static int bytemap_getNextEntry(struct bytemap *bmap) {
  int entry;

  if (bmap->index >= bmap->size) return -ENOSPC;

  entry = bmap->bmap[bmap->index];
  bmap->index += 1;

  return entry;
}


static int bytemap_setIndex(struct bytemap *bmap, unsigned int value) {

  bmap->index = value;

  return value;
}


void bytemap_printTable(struct bytemap *bmap) {

  int left= bmap->size, entry= 0;

  #ifdef DEBUG
  printf("%d\n", bmap->size);
  #endif
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
