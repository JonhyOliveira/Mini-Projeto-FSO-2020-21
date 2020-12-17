#include <stdio.h>

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

    // open disk
    #ifdef DEBUG
    //printf("DEBUG - Opening disk\n");
    #endif 

    ercode = disk_ops.open(argv[1], 0); 
    if (ercode < 0) {
        #ifdef DEBUG
        printf("\nError opening disk: %d", ercode);
        #endif
        return ercode;
    }
    
    printf("Beggining Disk %s Check..\n", argv[1]);

    struct super sb;
    ercode = super_ops.read(&sb);
    if (ercode < 0) {
        printf("\nCorrupted SuperBlock! Error reading super block: %d", ercode);
        return ercode;
    }

    printf("\nChecking SuperBlock Integrity..\n");
    ercode = super_ops.checkIntegrity(&sb);
    if (ercode < 0) {
        printf("\nMisformed Super Block: ");
        switch (ercode)
        {
            case -ESBIMAGIC:
                printf("invalid magic number (expected %ud got %ud)\n", FS_MAGIC, sb.fsmagic);
                break;
            
            case -ESBIV:
                printf("invalid values (check disk block order and superblock debug information)\n");
                break;

            case -ESBISIZE:
                printf("invalid size (expected %ud blocks got %ud", disk_ops.stat(), sb.nblocks);
                break;

            default:
                printf("Unexpected error (%d)", ercode);
                break;
        }
        printf("\n");
        return ercode;
    }
    else
        printf("No errors found.\n");

    
    struct bytemap bmap_dt;
    #ifdef DEBUG
    //printf("DEBUG - Reading data bytemap\n");
    #endif
    ercode = bmap_ops.read(&bmap_dt, sb.ndatablocks, sb.startDtBmap);
    if (ercode < 0) {
        printf("\nError reading data bytemap: %d", ercode);
        return ercode;
    }
    
    printf("\nChecking Data Bytemap Integrity..\n");
    ercode = bmap_ops.checkDataIntegrity(&bmap_dt, &sb);
    if (ercode < 0) {
        printf("\nInvalid Data ByteMap: ");
        switch (ercode)
        {
        case -EBMUV:
            printf("unexpected bytemap value (should be 1 or 0 got %d)", bmap_dt.bmap[bmap_dt.index]);
            break;
        
        case -EBMWV:
            printf("wrong value (expected data block to be empty/contain data\
 but data block contains data/is empty)");
        
        default:
            printf("Unexpected error (%d)", ercode);
            break;
        }
        printf("\n");
        return ercode;
    }
    else
        printf("No errors found.\n");

    struct bytemap bmap_in;
    #ifdef DEBUG
    //printf("DEBUG - Reading inode bytemap\n");
    #endif
    ercode = bmap_ops.read(&bmap_in, sb.ninodes, BMi_OFFSET);
    if (ercode < 0) {
        printf("\nError reading inode bytemap: %d", ercode);
        return ercode;
    }
    
    printf("\nChecking Inode Bytemap Integrity..\n");
    ercode = bmap_ops.checkInodeIntegrity(&bmap_in, &sb);
    if (ercode < 0) {
        printf("\nInvalid Inode ByteMap: ");
        switch (ercode)
        {
        case -EBMUV:
            printf("unexpected bytemap value (should be 1 or 0)");
            break;
        
        case -EBMWV:
            printf("wrong value (expected inode to be valid/invalid\
 but inode is invalid/valid)");
        
        default:
            printf("Unexpected error (%d)", ercode);
            break;
        }
        printf("\n");
        return ercode;
    }
    else 
        printf("No errors found.\n");

    printf("\nChecking Inodes Integrity..\n");
    ercode = inode_ops.checkIntegrity(&bmap_in, &bmap_dt, &sb);
    if (ercode < 0) {
        printf("\nInvalid Inode: ");
        switch (ercode)
        {
        case -EISB:
            printf("inode shares data block with other inode");
            break;
        
        case -EIEB:
            printf("inode points to unused data block");
        
        default:
            printf("Unexpected error (%d)", ercode);
            break;
        }
        printf("\n");
        return ercode;
    }
    else
        printf("No errors found.\n");

    printf("\nDisk check completed: No errors found.\n\n");
    return 0;
}