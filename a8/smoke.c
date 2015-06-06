#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

struct Helper{
  struct Agent*  a;
  uthread_cond_t ms;
  uthread_cond_t ps;
  uthread_cond_t ts;
  int m_count;
  int t_count;
  int p_count;
};

struct Helper* createHelper(struct Agent* agent) {
  struct Helper* helper = malloc (sizeof (struct Helper));
  helper->a  = agent;
  helper->ms     = uthread_cond_create (agent->mutex);
  helper->ps     = uthread_cond_create (agent->mutex);
  helper->ts     = uthread_cond_create (agent->mutex);
  helper->m_count        = 0;
  helper->t_count        = 0;
  helper->p_count        = 0;
  return helper;
}

 

//
// TODO
// You will probably need to add some procedures and struct etc.
//

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

void* mcount(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while(1){
      uthread_cond_wait(h->a->match);
      h->m_count++;
      if (h->t_count > 0){ 
        h->m_count--;
        h->t_count--;
        uthread_cond_signal(h->ps);
      }
      if (h->p_count > 0){
        h->m_count--;
        h->p_count--;
        uthread_cond_signal(h->ts);
      }
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

void* msmoke(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while (1){
      uthread_cond_wait(h->ms);
      smoke_count[1]++;
      uthread_cond_signal(h->a->smoke);
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

void* tcount(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while(1){
      uthread_cond_wait(h->a->tobacco);
      h->t_count++;
      if (h->m_count > 0){ 
        h->m_count--;
        h->t_count--;
        uthread_cond_signal(h->ps);
      }
      if (h->p_count > 0){
        h->t_count--;
        h->p_count--;
        uthread_cond_signal(h->ms);
      }
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

void* tsmoke(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while (1){
      uthread_cond_wait(h->ts);
      smoke_count[4]++;
      uthread_cond_signal(h->a->smoke);
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

void* pcount(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while(1){
      uthread_cond_wait(h->a->paper);
      h->p_count++;
      if (h->t_count > 0){ 
        h->p_count--;
        h->t_count--;
        uthread_cond_signal(h->ms);
      }
      if (h->m_count > 0){
        h->m_count--;
        h->p_count--;
        uthread_cond_signal(h->ts);
      }
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

void* psmoke(void* hv){
  struct Helper* h = hv;
  uthread_mutex_lock(h->a->mutex);
    while (1){
      uthread_cond_wait(h->ps);
      smoke_count[2]++;
      uthread_cond_signal(h->a->smoke);
    }
  uthread_mutex_unlock(h->a->mutex);
  return NULL;
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();
  struct Helper* h = createHelper(a);
  // TODO
  uthread_t tcountthread = uthread_create(tcount, h);
  uthread_t pcountthread = uthread_create(pcount, h);
  uthread_t mcountthread = uthread_create(mcount, h);
  uthread_t tsmokethread = uthread_create(tsmoke, h);
  uthread_t psmokethread = uthread_create(psmoke, h);
  uthread_t msmokethread = uthread_create(msmoke, h);


  uthread_join (uthread_create (agent, a), 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}
