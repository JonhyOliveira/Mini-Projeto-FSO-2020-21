#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef DISK_DRIVER_H
#include "disk_driver.h"
extern struct disk_operations disk_ops;
#endif

#ifndef FFS_SUPER_H
#include "ffs_super.h"
#endif

/* #ifndef FFS_INODE_H
#include "ffs_inode.h"
#endif */
// (debug) void super_debug(const struct super *sb);

static int super_read(struct super *sb) {
  int ercode;
  union sb_block sb_u;

  ercode = disk_ops.read(SB_OFFSET, sb_u.data);

  if (ercode < 0) return ercode; // an advice: always test the return codes...

  *sb = sb_u.sb;

  return 0;
}


/* Helper functions */

void super_print(const struct super *sb) {
  printf("Superblock:\n");
  printf("%s\n", ((*sb).fsmagic==FS_MAGIC)?"valid":"invalid");
  printf("%d\n", (*sb).nblocks);
  printf("%d\n", (*sb).nbmapblocksinodes);
  printf("%d\n", (*sb).ninodeblocks);
  printf("%d\n", (*sb).ninodes);
  printf("%d\n", (*sb).nbmapblocksdata);
  printf("%d\n", (*sb).ndatablocks);
  printf("%d\n", (*sb).startInArea);
  printf("%d\n", (*sb).startDtBmap);
  printf("%d\n", (*sb).startDtArea);
  printf("%s\n", (*sb).mounted?"yes":"no");
}

void super_debug(const struct super *sb) {
  printf("Superblock (debug):\n");
  printf("fsmagic:\t\t\t%s\n", ((*sb).fsmagic==FS_MAGIC)?"valid":"invalid");
  printf("nblocks:\t\t\t%d\n", (*sb).nblocks);
  printf("nbmapblocksinodes:\t\t%d\n", (*sb).nbmapblocksinodes);
  printf("ninodeblocks:\t\t\t%d\n", (*sb).ninodeblocks);
  printf("ninodes:\t\t\t%d\n", (*sb).ninodes);
  printf("nbmapblocksdata:\t\t%d\n", (*sb).nbmapblocksdata);
  printf("ndatablocks:\t\t\t%d\n", (*sb).ndatablocks);
  printf("startInArea:\t\t\t%d\n", (*sb).startInArea);
  printf("startDtBmap:\t\t\t%d\n", (*sb).startDtBmap);
  printf("startDtArea:\t\t\t%d\n", (*sb).startDtArea);
  printf("mounted:\t\t\t%s\n", (*sb).mounted?"yes":"no");
}


struct super_operations super_ops= {
	.read= &super_read,
	.print= &super_print,
	.debug= &super_debug
};
