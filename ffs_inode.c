#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef FFS_INODE_H
#include "ffs_inode.h"
#endif

#ifndef FFS_BYTEMAP_H
#include "ffs_bytemap.h"
#endif

#ifndef _BFS_ERRNO_H
#include "ffs_errno.h"
#endif

static void inode_print(unsigned int number, struct inode *in) {

  printf("%d:\n", number);
  printf("\t%s\n", (in->isvalid)?"valid":"invalid");
  if (in->isvalid) {
    printf("\t%d\n", in->size);
    for (int i = 0; i < POINTERS_PER_INODE; i++)
      if (i*DISK_BLOCK_SIZE >= in->size)
        printf("\t\tNULL\n");
      else
        printf("\t\t%d\n", in->direct[i]);
  }
}

static int inode_printTable(unsigned int ninodeblocks, unsigned int ninodes,\
			unsigned int inodesStartBlock) {
  int ercode;
  union in_block in_b;
  int inblk, in_nmbr;

  printf("i-nodes:\n");

  for (inblk = 0; inblk < ninodeblocks; inblk++) {
	  // the table starts at inodesStartBlock
    ercode = disk_ops.read(inodesStartBlock + inblk, in_b.data);
    if (ercode < 0) return ercode;

    // Print each inode in block
    for (in_nmbr = 0; in_nmbr < INODES_PER_BLOCK && (inblk*INODES_PER_BLOCK) + in_nmbr < ninodes; in_nmbr++) 
      inode_print((inblk*INODES_PER_BLOCK)+in_nmbr, &(in_b.ino[in_nmbr]));
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
  
  ercode = disk_ops.read(startInArea + block, in_b.data); // read the block

  if (ercode < 0) return ercode;
  
  *in = in_b.ino[offset]; // extract the inode information from the block into inode *in
  
  return 0;
}

int inode_check_integrity(struct bytemap *bmap_in, struct bytemap *bmap_dt, struct super *sb) {

  int ercode;

  int nInodes = bmap_in->size;

  struct inode in;
  int inNB; // number of used data blocks according to inode size
  int nDB; // data block index

  unsigned char *usedDB  = malloc(bmap_dt->size * sizeof(char)); // bytemap structure
  // to check if no two inodes share the same data block

  for (int i = 0; i < nInodes; i++) {
    ercode = inode_read(sb->startInArea, i, &in);
    if (ercode < 0) return ercode;

    #ifdef DEBUG
    printf("inode: %d data blocks:", i);
    #endif

    inNB = ceil((float) in.size / DISK_BLOCK_SIZE);
    int pointerN;
    for (pointerN = 0; pointerN < POINTERS_PER_INODE && !(pointerN >= inNB); pointerN++) {
      
      nDB = in.direct[pointerN];

      #ifdef DEBUG
      printf("\t%d", nDB);
      #endif

      // check if data block is used by other inode
      if (usedDB[nDB])
        return -EISB;
      else
        usedDB[nDB] = 1;

      // check if inote points to unused data block
      if (!(bmap_dt->bmap[nDB]))
        return -EIEB;

      

    }

    #ifdef DEBUG
    printf("\n");
    #endif

    /*  Not needed (?)
    // check if iterated through only the defined pointers
    if (pointerN != inNB) {
        #ifdef DEBUG
        printf("Inode size does not correspond to iterated pointers Inode: %d | Expected n Blocks: %d \
          | Iterated Blocks: %d", i, inNB, pointerN);
        #endif
        return -1;
    }*/
  }  

  return 0;
}

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
  int ercode, size;
  unsigned int block, offset;
  union in_block in_b;
  unsigned char buf[DISK_BLOCK_SIZE];
  struct inode in;

  inode_location(absinode, &block, &offset);
  
  // read inode block
  ercode = disk_ops.read(startInArea + block, in_b.data);
  if (ercode < 0) return ercode;

  // retrive desired inode
  in = in_b.ino[offset];
  
  // check if is valid
  if (!in.isvalid) return 0;  

  printf("\nPrinting contents of file(inode) %d\n", absinode);

  size= in.size;
  if (!size) {printf("** NO DATA **\n"); return 0;}

  // print inode pointer blocks
  int in_pointer = 0;
  int toPrint = in.size;
  while(in_pointer < ceil((float) in.size/DISK_BLOCK_SIZE) && toPrint > 0) { // number of inode pointers
    // read block
    ercode = disk_ops.read(startDtArea+in.direct[in_pointer], buf);
    if (ercode < 0) return ercode;

    // print block
    f_data_print(buf, fmin(DISK_BLOCK_SIZE, toPrint));

    in_pointer++;
    toPrint -= DISK_BLOCK_SIZE;
  }
  return 0;
}


struct inode_operations inode_ops= {
	.read= inode_read,
  .checkIntegrity= inode_check_integrity,
	.printFileData= inode_printFileData,
	.printTable= inode_printTable
};
