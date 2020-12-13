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


static int super_read(struct super *sb) {
  int ercode;
  union sb_block sb_u;

  // read the disk block that contains the superblock
  // ....
  if (ercode < 0) return ercode; // an advice: always test the return codes...

  // copy the contents (without the extra garbage :-) to sb

  return 0;
}


/* Helper functions */

void super_print(const struct super *sb) {
  printf("Superblock:\n");
  // print the way it should look for Mooshak delivery
}

void super_debug(const struct super *sb) {
  printf("Superblock:\n");
  // May be printing the names when developing the application?
}


struct super_operations super_ops= {
	.read= &super_read,
	.print= &super_print,
	.debug= &super_debug
};
