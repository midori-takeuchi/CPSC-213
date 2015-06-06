#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t waiters;

int sum = 0;

void interruptServiceRoutine () {
  // TODO
  uthread_unblock(queue_dequeue(&waiters));
}

void blockUntilComplete() {
  // TODO
  uthread_block();
}

void handleRead (char* buf, int nbytes, int blockno) {
  assert (*((int*)buf) == blockno);
  sum += *(((int*) buf) + 1);
}

/**
 * Struct provided as argument to readAndHandleBlock
 */
struct readAndHandleBlockArgs {
  char* buf;
  int   nbytes;
  int   blockno;
};

void* readAndHandleBlock (void* args_voidstar) {
  // TODO
  // Synchronously:
  //   (1) call disk_scheduleRead to request the block
  //   (2) wait read for it to complete
  //   (3) call handleRead
  struct readAndHandleBlockArgs* x = ((struct readAndHandleBlockArgs*) args_voidstar);
  queue_enqueue(&waiters, uthread_self);
  disk_scheduleRead(x->buf, x->nbytes, x->blockno);
  blockUntilComplete();
  handleRead(x->buf, x->nbytes, x->blockno);

  return NULL;  
}

void run (int numBlocks) {
  uthread_t thread [numBlocks];
  for (int blockno = 0; blockno < numBlocks; blockno++) {
    // TODO
    // call readAndHandleBlock in a way that allows this
    // operation to be synchronous without stalling the CPU

    struct readAndHandleBlockArgs* args = malloc (sizeof (struct readAndHandleBlockArgs));
    args->buf = malloc(8);
    args->nbytes = sizeof(args->buf);
    args->blockno = blockno;
    thread[blockno] = uthread_create(readAndHandleBlock, args);
  }
  for (int i=0; i<numBlocks; i++)
    uthread_join (thread [i], 0);
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
