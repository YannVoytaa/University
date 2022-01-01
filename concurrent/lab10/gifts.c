#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include "err.h"


//some consts
#define MAX_REINDEERS 7
#define MAX_ELFS 6
#define MAX_GIFTS 5

#define NO_REINDEERS 3
#define NO_ELFS 4
#define NO_GIFTS 5

#define BUFFSIZE 3
#define LOOPS 5

/**************************************************************************************************/
//storage compartment
struct storage{
    int presents[BUFFSIZE];
    sem_t empty;
    sem_t full;
    sem_t mutex;
    int begin;
    int size;
    int waitingForNonEmpty;
    int waitingForNonFull;

    int endedElfs;
};

struct storage* init() {
    void *mapped_mem_all;
    char *mapped_mem;
    int fd_memory = -1;
    int flags, prot;

    prot = PROT_READ | PROT_WRITE;
    flags = MAP_SHARED | MAP_ANONYMOUS;
    mapped_mem_all = mmap(NULL, sizeof(struct storage), prot, flags,
                          fd_memory, 0);

    if(mapped_mem_all == MAP_FAILED)
        syserr("mmap");

    struct storage *mem = (struct storage *) mapped_mem_all;
    if(sem_init(&mem->empty, 1, 0)) {
        syserr("sem_init");
    }
    if(sem_init( &mem->full, 1, 0)) {
        syserr("sem_init");
    }
    if(sem_init( &mem->mutex, 1, 1)) {
        syserr("sem_init");
    }
    return mem;
}
void destroy(struct storage *s) {
    sem_destroy(&s->mutex);
    sem_destroy(&s->full);
    sem_destroy(&s->empty);
    munmap(s,  sizeof(struct storage));
}
/**************************************************************************************************/
//popular names
char *elfs_names[MAX_ELFS] = {"Mirek","Zuzia","Gienia", "Macius", "Ela", "Stasia"};
char *reindeers_names[MAX_REINDEERS] = {"Janek", "Zosia", "Franek", "Jozek", "Asia", "Olek", "Ruda"};
char *gifts[MAX_GIFTS] = {"lalka", "klocki", "ciuchcia", "rozga", "rower"};

/**************************************************************************************************/
//toymaker
int produce(){
  sleep(rand() % 3);
  return rand()%NO_GIFTS;
}

//sent to santa
void deliver(int i){
  sleep(rand() % 3);
}

void nap(int i){
  sleep(i);
}
/**************************************************************************************************/
//life of an elf
void elf(int id, struct storage* s){

  int i,g;
  printf("Hej! Jestem elfem o imieniu %s, zaczynam!\n", elfs_names[id]);
  for(i = 0; i< LOOPS; ++i){

    g = produce();
    printf("Hej! Jestem elfem o imieniu %s, wyprodukowalem/am prezent: %s\n", elfs_names[id], gifts[g]);
    if(sem_wait(&s->mutex)) {
      syserr("mutex wait");
    }
    if (s->size == BUFFSIZE) {
      (s->waitingForNonFull)++;
      if(sem_post(&s->mutex))
          syserr("mutex post");
      if(sem_wait(&s->full))
          syserr("full wait");
      (s->waitingForNonFull)--;
    }
    (s->size)++;
    s->presents[(s->begin + s->size - 1) % BUFFSIZE] = g;
    //printf("%d %d\n", s->begin, s->size);
    printf("Hej! Jestem elfem o imieniu %s, wstawilem/am prezent: %s\n", elfs_names[id], gifts[g]);
    if (s->waitingForNonEmpty > 0) {
      if(sem_post(&s->empty))
          syserr("empty post");
    }
    else {
      if(sem_post(&s->mutex))
          syserr("mutex post");
    }
  }
    if(sem_wait(&s->mutex))
        syserr("mutex wait");
    (s->endedElfs)++;
    if(sem_post(&s->mutex))
        syserr("mutex post");
}

/**************************************************************************************************/
//life of a reindeer
void reindeer(int id, struct storage* s){
  
  int end = 0;
  int g;
  int makeBreak = 0;
  printf("Hej! Jestem reniferem o imieniu %s, zaczynam!\n", reindeers_names[id]);
  while(!end){
    if(sem_wait(&s->mutex)) {
        syserr("mutex wait");
    }
    if (s->size == 0 && s->endedElfs == NO_ELFS) {
      break;
    }
    if (s->size == 0) {
        (s->waitingForNonEmpty)++;
        if(sem_post(&s->mutex))
            syserr("mutex post");
        if(sem_wait(&s->empty))
            syserr("empty wait");
        (s->waitingForNonEmpty)--;
        if (s->size == 0 && s->endedElfs == NO_ELFS) {
            break;
        }
    }
    g = s->presents[s->begin];
    s->begin = ((s->begin) + 1) % BUFFSIZE;
    (s->size)--;
    printf("Hej! Jestem reniferem o imieniu %s, odebralem/am prezent: %s\n", reindeers_names[id], gifts[g]);
    if (s->waitingForNonFull > 0) {
        if(sem_post(&s->full))
            syserr("mutex post");
    }
    else {
        makeBreak = s->size == 0 && s->endedElfs == NO_ELFS;
        if(!makeBreak && sem_post(&s->mutex))
            syserr("mutex post");
    }
    deliver(g);
    printf("Hej! Jestem reniferem o imieniu %s dostarczylem/am prezent: %s\n", reindeers_names[id], gifts[g]);
    if (makeBreak) {
      break;
    }
  }
  if (s->waitingForNonEmpty > 0) {
      sem_post(&s->empty);
  }
  else {
      sem_post(&s->mutex);
  }
}
/**************************************************************************************************/
/**************************************************************************************************/
int main(){
    
  int i;
  pid_t pid;
  struct storage *mapped_mem = init();
  int seed = time(0);
  srand(seed);

  printf("Tworze pracownikow.\nElfy: %d; Renifery: %d\n", NO_ELFS, NO_REINDEERS);
  
  for(i = 0; i < NO_ELFS + NO_REINDEERS; i++){
    
    rand();//some randomness
    switch(pid = fork()){
    case -1:
      syserr("fork");
    case 0:
      srand(rand());
      if (i < NO_ELFS){
        printf("Elf %d!\n", i);
        elf(i, mapped_mem);
        
      }else{
        printf("Renifer %d!\n", i);
        reindeer(i-NO_ELFS,mapped_mem);
      }
      return 0;
    default:
      nap(1);
      printf("Kolejny pracownik!\n");
      break;
    }
  }

  for(i = 0; i< NO_ELFS+NO_REINDEERS; ++i) { printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%d\n", i);wait(0);}
  destroy(mapped_mem);
  return 0;
  }
