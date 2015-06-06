#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

int sum = 0;

/**
 * Description of 1 pendind read.  
 * Each pending read has one of these stored on the prq queue.
 */
struct PendingRead {
  // TODO
  char* abuf;
  int anbytes;
  int ablockno;
  void (*handler) (char*, int, int);
} pendingRead;

/**
 * Queue of pending reads.
 */
queue_t prq;

void interruptServiceRoutine () {
  // called when disk fires an interrupt TODO
  struct PendingRead* finishedRead = queue_dequeue(&prq);
  finishedRead->handler(finishedRead->abuf, finishedRead->anbytes, finishedRead->ablockno);
}

void asyncRead (char* buf, int nbytes, int blockno, void (*handler) (char*, int, int)) {
  struct PendingRead* read = malloc(sizeof(struct PendingRead*));
  read->abuf = buf;
  read->anbytes = nbytes;
  read->ablockno = blockno;
  read->handler = handler;
  queue_enqueue (&prq, read);
  disk_scheduleRead(buf, nbytes, blockno);
}

/**
 * This is the read completion routine.  This is where you 
 * would place the code that processed the data you got back
 * from the disk.  In this case, we'll just verify that the
 * data is there.
 */
void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
  sum += *(((int*) buf) + 1);
}

/**
 * Read numBlocks blocks from disk sequentially starting at block 0.
 */
void run (int numBlocks) {
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    // call asyncRead to schedule read TODO
    char buf[8];
    asyncRead(buf, sizeof(buf), numBlocks, handleRead);
  }
  disk_waitForReads();
}

int main (int argc, char** argv) {
  static const char* usage = "usage: aRead numBlocks";
  int numBlocks = 0;
  
  if (argc == 2)
    numBlocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (numBlocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }
  
  uthread_init (1);
  disk_start   (interruptServiceRoutine);
  
  run (numBlocks);
  
  printf ("%d\n", sum);
}
