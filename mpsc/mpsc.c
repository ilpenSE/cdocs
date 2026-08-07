#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <pthread.h>
#include <assert.h>

typedef enum {
  DROP_POLICY = 0,
  BLOCK_POLICY,
  _Policy_count,
} Policy;

#define QUEUE_SIZE 1024

sem_t sem_empty_slots;
sem_t sem_full_slots;
bool is_alive;
Policy log_policy;

typedef struct {
  char msg[256];
} Payload;

typedef struct {
  Payload payload;
  _Atomic size_t seq;
} Slot;

typedef struct {
  Slot items[QUEUE_SIZE];
  _Atomic size_t head;
  size_t tail;
} Queue;

size_t processed;

void *consumer(void *ctx) {
  Queue *q = (Queue *)ctx;
  while (1) {
    sem_wait(&sem_full_slots); // wait for signals
    // Exit condition
    size_t head = atomic_load_explicit(&q->head, memory_order_relaxed);
    if (head == q->tail) {
      if (!atomic_load_explicit(&is_alive, memory_order_relaxed)) break;
      continue;
    }

    // get sequence number and tail position
    // if seq == pos + 1, the slot is ready to be processed
    // if not, slot isn't ready and wait (spin a little bit then sched_yield)
    size_t pos = q->tail;
    Slot *slot = &q->items[pos % QUEUE_SIZE];
    size_t spins = 0;
    while (atomic_load_explicit(&slot->seq, memory_order_acquire) != pos + 1) {
      if (spins++ > 64) sched_yield();
    }

    // copy shared memory into stack
    Payload payload = slot->payload;

    // release locked slot:
    atomic_store_explicit(&slot->seq, pos + QUEUE_SIZE, memory_order_release);
    q->tail++;
    sem_post(&sem_empty_slots);

    // you can now process payload safely:
    printf("payload = {msg=%s}\n", payload.msg);
    processed += 1;
  }
  return NULL;
}

bool producer(Queue *q, const char *msg) {
  Payload payload = {0};
  memcpy(&payload.msg, msg, strlen(msg));

  Slot *slot;
  size_t pos;
  for (;;) {
    pos = atomic_load_explicit(&q->head, memory_order_relaxed);
    slot = &q->items[pos % QUEUE_SIZE];
    size_t seq = atomic_load_explicit(&slot->seq, memory_order_acquire);

    if (seq == pos) {
      if (atomic_compare_exchange_weak_explicit(&q->head, &pos, pos + 1, memory_order_relaxed, memory_order_relaxed))
        break; // this producer claimed this slot
      continue; // this producer couldn't claim this slot, try again
    } else if (seq < pos) {
      // queue full
      switch (log_policy) {
      case DROP_POLICY: return false;
      case BLOCK_POLICY: {
        sem_wait(&sem_empty_slots);
      } break;
      default: return false;
      }
    }
  }

  slot->payload = payload;
  atomic_store_explicit(&slot->seq, pos + 1, memory_order_release);
  sem_post(&sem_full_slots);
  return true;
}

void *producer_thread(void *ctx) {
  Queue *q = (Queue *)ctx;
  for (size_t i = 0; i < 1000; i++) {
    producer(q, "Hello, World");
  }
  return NULL;
}

int main(void) {
  Queue q = {0};
  pthread_t consumer_thread;
  is_alive = true;
  log_policy = DROP_POLICY;
  for (size_t i = 0; i < QUEUE_SIZE; ++i)
    atomic_init(&q.items[i].seq, i);
  sem_init(&sem_empty_slots, 0, QUEUE_SIZE);
  sem_init(&sem_full_slots, 0, 0);
  pthread_create(&consumer_thread, NULL, consumer, &q);

// MULTI PRODUCER
  pthread_t threads[10];
  for (size_t i = 0; i < sizeof(threads)/sizeof(*threads); i++) {
    pthread_create(&threads[i], NULL, producer_thread, &q);
  }

  for (size_t i = 0; i < sizeof(threads)/sizeof(*threads); i++) {
    pthread_join(threads[i], NULL);
  }
// MULTI PRODUCER

  atomic_store_explicit(&is_alive, false, memory_order_relaxed);
  sem_post(&sem_full_slots);
  pthread_join(consumer_thread, NULL);
  printf("processed = %zu\n", processed);
}
