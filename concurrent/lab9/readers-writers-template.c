#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "err.h"

#define READERS 3
#define WRITERS 2
#define NAP 2
#define BSIZE 32

struct readwrite {
  pthread_mutex_t lock;         
  pthread_cond_t readers;      
  pthread_cond_t writers;    
  int rcount, wcount, rwait, wwait;
  int change;
};

struct readwrite library;
char book[BSIZE];
int working = 1;

/* Initialize a buffer */

void init(struct readwrite *rw) {
    int err;
    if((err = pthread_mutex_init(&rw->lock, 0)) != 0)
        syserr (err, "mutex init failed");
    if((err = pthread_cond_init(&rw->readers, 0)) != 0)
        syserr (err, "readers cond init failed");
    if((err = pthread_cond_init(&rw->writers, 0)) != 0)
        syserr (err, "writers cond init failed");
    rw->rcount = 0;
    rw->wcount = 0;
    rw->wwait = 0;
    rw->rwait = 0;
}

/* Destroy the buffer */

void destroy(struct readwrite *rw) {
    int err;
    if((err = pthread_mutex_destroy(&rw->lock)) != 0)
        syserr (err, "mutex destroy failed");
    if((err = pthread_cond_destroy(&rw->readers)) != 0)
        syserr (err, "readers cond destroy failed");
    if((err = pthread_cond_destroy(&rw->writers)) != 0)
        syserr (err, "writers cond destroy failed");
}

void wantToRead(struct readwrite *rw) {
    int err;
    if ((err = pthread_mutex_lock(&rw->lock)) != 0)
        syserr(err, "error in reader mutex lock");
    //printf("Want to read start\n");
    //int shouldWaitForMe = 0;
    if(rw->wwait > 0 || rw->wcount /*>*/< 0) {
        (rw->rwait)++;
        int waitForWCount = abs(rw->wcount) + (rw->wwait > 0 ?  1 : 0);
        while (waitForWCount > rw->wcount) {
            if ((err = pthread_cond_wait(&rw->readers, &rw->lock)) != 0)
                syserr(err, "error in reader wait");
        }
        (rw->rwait)--;
    }
    (rw->rcount)++;
    //printf("Want to read end\n");
    if ((err = pthread_mutex_unlock(&rw->lock)) != 0)
        syserr(err, "error in reader mutex unlock");
}

void endRead(struct readwrite *rw) {
    int err;
    if ((err = pthread_mutex_lock(&rw->lock)) != 0)
        syserr(err, "error in reader mutex lock");
    (rw->rcount)--;
    if (rw->rcount == 0) {
        if ((err = pthread_cond_signal(&rw->writers)) != 0)
            syserr(err, "error in writer signal");
    }
    //printf("End read\n");
    if ((err = pthread_mutex_unlock(&rw->lock)) != 0)
        syserr(err, "error in reader mutex unlock");
}

void wantToWrite(struct readwrite *rw) {
    int err;
    if ((err = pthread_mutex_lock(&rw->lock)) != 0)
        syserr(err, "error in writer mutex lock");
    //printf("Want to write start\n");
    int wasWaiting = 0;
    while (rw->rcount > 0 || rw->wcount /*>*/< 0) {
        if(wasWaiting == 0) (rw->wwait)++;
        wasWaiting = 1;
        if ((err = pthread_cond_wait(&rw->writers, &rw->lock)) != 0)
            syserr(err, "error in writer wait");
    }
    if(wasWaiting == 1) (rw->wwait)--;
    wasWaiting = 0;
    (rw->wcount)++;
    (rw->wcount) *= -1;
    //printf("Want to write end\n");
    if ((err = pthread_mutex_unlock(&rw->lock)) != 0)
        syserr(err, "error in reader mutex unlock");
}

void endWrite(struct readwrite *rw) {
    int err;
    if ((err = pthread_mutex_lock(&rw->lock)) != 0)
        syserr(err, "error in writer mutex lock");
    //(rw->wcount)--;
    (rw->wcount) *= -1;
    if (rw->rwait > 0) {
        //printf("Zwalniam %d\n", rw->rwait);
        if ((err = pthread_cond_signal(&rw->readers)) != 0)
            syserr(err, "error in reader signal");
    } else {
        if ((err = pthread_cond_signal(&rw->writers)) != 0)
            syserr(err, "error in writer signal");
    }
    //printf("End write\n %d %d\n", rw->rwait, rw->wwait);
    if ((err = pthread_mutex_unlock(&rw->lock)) != 0)
        syserr(err, "error in writer mutex unlock");
}

void *reader(void *data)
{
   while (working) {
       wantToRead(&library);
     printf("reader read: %s\n", book); /* reading */
     endRead(&library);
   }
   return 0;
}

void *writer(void *data)
{  
   int l;
   while (working) {
       wantToWrite(&library);
     l = rand()%10;
       //printf("Writer\n");
     snprintf(book, BSIZE, "6 times a number %d %d %d %d %d %d", l, l, l, l, l, l);
       endWrite(&library);
   }
   return 0;
}


int main() {
  pthread_t th[READERS+WRITERS];
  pthread_attr_t attr;
  int i, err;
  void *retval;

  srand((unsigned)time(0));
  
  init(&library);
  if ((err = pthread_attr_init (&attr)) != 0)
    syserr (err, "attr_init failed");
  if ((err = pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_JOINABLE)) != 0)
    syserr (err, "attr_setdetachstate failed");

  for (i = 0; i < READERS + WRITERS; i++) {
    if (i < READERS) {
      if ((err = pthread_create(&th[i], &attr, reader, 0)) != 0)
	syserr (err, "create failed");
    } else 
      if ((err = pthread_create(&th[i], &attr, writer, 0)) != 0)
	syserr (err, "create failed");
  }
  
  sleep(NAP);
  working = 0;

  for (i = 0; i < READERS + WRITERS; i++) {
    if ((err = pthread_join(th[i], &retval)) != 0)
    syserr (err, "join failed");
  }
  
  if ((err = pthread_attr_destroy (&attr)) != 0)
    syserr (err, "cond destroy failed");
  destroy(&library);
  return 0;
}
