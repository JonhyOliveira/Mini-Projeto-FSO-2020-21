#include <stdio.h>
#include <string.h>

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
extern struct bytemap_operations bmap_ops;
#endif


#ifndef FFS_INODE_H
#include "ffs_inode.h"
extern struct inode_operations inode_ops;
#endif

int main( int argc, char *argv[]) {

  int ercode;
  struct super sb;
  struct bytemap bmapINODES, bmapDATA;

  ercode= disk_ops.open(argv[1], 0); 
  if (ercode < 0) return ercode;

  ercode= super_ops.read(&sb); 
  if (ercode < 0) return ercode;

  if (sb.fsmagic != FS_MAGIC)
    { printf("** INVALID superblock, terminating...\n"); return 0; }
  #ifndef DEBUG
  super_ops.print(&sb);
  #else
  super_ops.debug(&sb);
  #endif

  // read the block of the inode bytemap
  ercode = bmap_ops.read(&bmapINODES, sb.ninodes, BMi_OFFSET);
  if (ercode < 0) return ercode;
  printf("Bmap for i-nodes:\n");
  bmap_ops.printTable(&bmapINODES);

  // read the block of the data bytemap
  ercode = bmap_ops.read(&bmapDATA, sb.ndatablocks, sb.startDtBmap);
  if (ercode < 0) return ercode;
  printf("Bmap for data blocks:\n");
  bmap_ops.printTable(&bmapDATA);

  ercode = inode_ops.printTable(sb.ninodeblocks, sb.ninodes, sb.startInArea);
  if (ercode < 0) return ercode;
 
  for (int i= 0; i < sb.ninodes; i++) {
    ercode= inode_ops.printFileData(sb.startInArea, i, sb.startDtArea);
    if (ercode < 0) return ercode;
  }

  return 0;
}
