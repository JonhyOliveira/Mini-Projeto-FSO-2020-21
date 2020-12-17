CFLAGS = -Wall

checkBFS_D: checkBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o
	gcc $(CFLAGS) -D DEBUG -o checkBFS checkBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o -lm
	make cleanUP_checkBFS

dumpBFS_D: dumpBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o
	gcc $(CFLAGS) -D DEBUG -o dumpBFS dumpBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o -lm
	make cleanUP_dumpBFS

checkBFS: checkBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o
	gcc $(CFLAGS) -o checkBFS checkBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o -lm
	make cleanUP_checkBFS

dumpBFS: dumpBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o
	gcc $(CFLAGS) -o dumpBFS dumpBFS.o disk_driver.o ffs_super.o ffs_bytemap.o ffs_inode.o -lm
	make cleanUP_dumpBFS

checkBFS.o: checkBFS.c
	gcc $(CFLAGS) -c checkBFS.c

dumpBFS.o: dumpBFS.c
	gcc  $(CFLAGS) -c dumpBFS.c

disk_driver.o: disk_driver.c disk_driver.h ffs_errno.h
	gcc $(CFLAGS) -c disk_driver.c

ffs_super.o: ffs_super.c ffs_super.h ffs_inode.h ffs_errno.h
	gcc $(CFLAGS) -c ffs_super.c

ffs_bytemap.o: ffs_bytemap.c ffs_bytemap.h ffs_errno.h
	gcc $(CFLAGS) -c ffs_bytemap.c

ffs_inode.o: ffs_inode.c ffs_inode.h ffs_errno.h
	gcc $(CFLAGS) -c ffs_inode.c

clean: 
	rm dumpBFS checkBFS 
	make cleanUP_checkBFS
	make cleanUP_dumpBFS

cleanUP_dumpBFS:
	rm dumpBFS.o disk_driver.o ffs_bytemap.o ffs_inode.o ffs_super.o

cleanUP_checkBFS:
	rm checkBFS.o disk_driver.o ffs_bytemap.o ffs_inode.o ffs_super.o