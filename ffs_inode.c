#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef FFS_INODE_H
#include "ffs_inode.h"
#endif


static void inode_print(unsigned int number, struct inode *in) {

  printf("%d:\n", number);
  printf("	%s\n", (in->isvalid)?"valid":"invalid");
  if (in->isvalid) {

    // print the i-node information for one i-node

  }
}

static int inode_printTable(unsigned int ninodeblocks, unsigned int ninodes,\
			unsigned int inodesStartBlock) {
  int ercode;
  union in_block in_b;
  // more variables...

  printf("i-nodes:\n");

  for ( // ninodeblocks is the number of blocks of the inode table) {
	// the table starts at inodesStartBlock
    // Get a full inode block; 
    ...
    if (ercode < 0) return ercode;

    // Print each inode in block
    for or while....
      inode_print( number of the inode, inode to print);
    }
  }
  
  return 0;
}


/* inode (global) number is decomposed into inode block number
   and offset within that block. The inode block number starts at 0 */

static void inode_location(unsigned int numinode,\
		 unsigned int *numblock, unsigned int *offset) {
  *numblock= numinode / INODES_PER_BLOCK;
  *offset= numinode % INODES_PER_BLOCK;
}

// read an i-node from disk
static int inode_read(unsigned int startInArea, unsigned int absinode, struct inode *in) {
  int ercode;
  unsigned int block, offset;
  union in_block in_b;

  inode_location(absinode, &block, &offset);
  // read the inode block
  if (ercode < 0) return ercode;
  
  // extract the inode information from the block into inode *in
  
  return 0;
}

/* For option A prints a buffer of data up to toPrint chars

void f_data_print(unsigned char *buf, int toPrint) {
int left= toPrint, entry= 0;

  // prints 16 entries per line
  while (left) {
    if ( (entry+1)%16 ) printf("%c ", buf[entry]);
    else printf("%c\n", buf[entry]);
    left--; entry++;
  }
  if ( entry%16 ) printf("\n"); // last NL for general case

}

static int inode_printFileData(unsigned int startInArea, unsigned int absinode,\
			   unsigned int startDtArea) {
  int ercode, size, toPrint;
  unsigned int block, offset;
  union in_block in_b;
  unsigned char buf[DISK_BLOCK_SIZE];

  inode_location(absinode, &block, &offset);
  // read the data block containing the inode number absinode
  if (ercode < 0) return ercode;

  if ( the inode referenced at offset is not a valid one) return 0;  

  printf("\nPrinting contents of file(inode) %d\n", absinode);

  size= get the inode value of the file size
  if (!size) {printf("** NO DATA **\n"); return 0;}

  // print the contents of the data blocks
  
  return 0;
}
 ** end of Option A code */

struct inode_operations inode_ops= {
	.read= inode_read,
	.printFileData= inode_printFileData,
	.printTable= inode_printTable
};
