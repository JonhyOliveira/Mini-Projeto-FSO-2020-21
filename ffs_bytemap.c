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

#ifndef FFS_INODE_H
#include "ffs_inode.h"
extern struct inode_operations inode_ops;
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

int bytemap_check_data_integrity(struct bytemap *bmap, struct super *sb) {
  // TODO: error codes
  int ercode;

  unsigned char data[DISK_BLOCK_SIZE];
  unsigned char isDBOccupied;

  // iterate through data blocks
  for (int i = 0; i < bmap->size; i++) {

    #ifdef DEBUG
    printf("Bytemap index/val: %d/%d\t\tData block loc: %d", i, bmap->bmap[i], sb->startDtArea + i);
    #endif

    // check if bytemap value is valid
    if (!(bmap->bmap[i] == 1 || bmap->bmap[i] == 0)) {
      bmap->index = i;
      return -EBMUV;
    }

    // check if actual value corresponds to the expected value acording to bytemap
    isDBOccupied = 0;
    
    ercode = disk_ops.read(sb->startDtArea + i, data); // read data block
    if (ercode < 0) return ercode;

    // check if data block is occupied (has any data)
    for(int j = 0; j < DISK_BLOCK_SIZE; j++) {
      if (data[j]) {
        isDBOccupied = 1;
        break;
      }
    }

    #ifdef DEBUG
    printf("\tFound data: %s\n", isDBOccupied ? "yes":"no");
    #endif

    // check
    if (isDBOccupied != bmap->bmap[i]) {
      bmap->index = i;
      return -EBMWV;
    }
  }

  return 0;
}

int bytemap_check_inode_integrity(struct bytemap *bmap, struct super *sb) {
  // TODO: error codes
  int ercode;
  
  unsigned char isvalid;
  struct inode in;

  // check if there are no unexpected values
  for (int i = 0; i < bmap->size; i++) {
    isvalid = bmap->bmap[i];
    ercode = inode_ops.read(sb->startInArea, i, &in);
    #ifdef DEBUG
    printf("inode: %d\tbytemap is_valid:%d\tactual: %d\tmatch:%s\n", i, isvalid, in.isvalid, (isvalid == in.isvalid) ? "yes":"no");
    #endif
    if (ercode < 0) return ercode;

    // check if value is valid
    if (!(isvalid == 1 || isvalid == 0) && !(in.isvalid == 1 || in.isvalid == 0))
      return -EBMUV;

    // check if isvalid value is the same as the actual inode isvalid value
    if (in.isvalid != isvalid)
      return -EBMWV;
  }
  
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
  .checkInodeIntegrity = bytemap_check_inode_integrity,
  .checkDataIntegrity = bytemap_check_data_integrity,
	.getNextEntry= bytemap_getNextEntry,
	.setIndex= bytemap_setIndex,
	.printTable= bytemap_printTable
};
