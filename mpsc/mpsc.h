#ifndef MPSC_H
#define MPSC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Transpilation from C++11 to C11
#ifdef __cplusplus
#include <atomic>
#define ATOMIC(T) std::atomic<T>
#define memory_order_relaxed std::memory_order_relaxed
#define memory_order_seq_cst std::memory_order_seq_cst
#define memory_order_acquire std::memory_order_acquire
#define memory_order_release std::memory_order_release
#define atomic_thread_fence std::atomic_thread_fence
#define atomic_store_explicit std::atomic_store_explicit
#define atomic_load_explicit std::atomic_load_explicit
#define atomic_fetch_add_explicit std::atomic_fetch_add_explicit
#else
#include <stdatomic.h>
#define ATOMIC(T) _Atomic(T)
#endif

#ifdef __cplusplus
#define MPSCDEF extern "C"
#else
#define MPSCDEF extern
#endif

#define MPSC_CACHE_LINE 64
#define MPSC_ALIGN __attribute__((aligned(MPSC_CACHE_LINE)))

// Arbitrary data
typedef struct {
  int a;
  int b;
} Payload;

typedef struct {
  ATOMIC(size_t) seq;
  char payload[];
} Slot;

typedef struct {
  size_t capacity; // power of 2
  size_t mask; // capacity - 1 

  MPSC_ALIGN ATOMIC(size_t) head; // Producer
  MPSC_ALIGN size_t tail; // Consumer
  uint8_t slots[];
} Queue;

// These variables can be fine-tuned due to your traffic
// High traffic = high these constants
// Low traffic = lower values
#define MPSC_WAIT_NO_PAUSE_MAGIC 100
#define MPSC_WAIT_PAUSE_MAGIC 1000

// Return exact size of slot with padding, payload and seq
MPSCDEF size_t mpsc_slot_stride();

// Return the slot by idx from the given queue
MPSCDEF Slot* mpsc_slot_get(Queue* q, size_t idx);

// Extract payload from that slot
MPSCDEF Payload* mpsc_slot_payload(Slot* s);

// Create MPSC queue with given capacity (capacity must be power of 2)
MPSCDEF Queue* mpsc_queue_create(size_t capacity);

// Destroy the MPSC queue when logger destroy
MPSCDEF void mpsc_queue_destroy(Queue* q);

// Push something to the MPSC queue (Producer)
MPSCDEF bool mpsc_queue_push(Queue* q, const Payload* data);

// Pop something from the MPSC queue (Consumer)
MPSCDEF bool mpsc_queue_pop(Queue* q, Payload* out);

// Adaptive waiters, not only spins, it spins until reaches
// MPSC_NO_PAUSE_MAGIC and MPSC_PAUSE_MAGIC values
// Pause is pause instruction no pause -> just spins
// No pause means spin and pause
// If pause magic exceeded, it sleeps (Sleep or nanosleep)
MPSCDEF void mpsc_adaptive_wait(int* spins);
MPSCDEF void mpsc_adaptive_wait_reset(int* spins);

#ifdef MPSC_IMPLEMENTATION

// sleep for us microseconds
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#define MPSC_PAUSE_INS() _mm_pause()

// Sleep() has terrible resolution (milliseconds)
// But who uses winbloat for production-ready logger?
#define MPSC_SLEEP(us)                          \
  do {                                          \
    Sleep(1);                                   \
  } while (0)

#else // POSIX:
#include <time.h>
#define MPSC_SLEEP(us)                                              \
  do {                                                              \
    struct timespec ts = {(us) / 1000000, ((us) % 1000000) * 1000}; \
    nanosleep(&ts, NULL);                                           \
  } while (0)

#ifdef __x86_64__
  #define MPSC_PAUSE_INS() __asm__ volatile("pause" ::: "memory")
#elif defined(__aarch64__)
  #define MPSC_PAUSE_INS() __asm__ volatile("yield" ::: "memory")
#else
#error "No such supported platform for pause instruction"
#endif

#endif // _WIN32

size_t mpsc_slot_stride() {
  size_t total = sizeof(Slot) + sizeof(Payload);
  return (total + MPSC_CACHE_LINE - 1) & ~(size_t)(MPSC_CACHE_LINE - 1);
}

Slot* mpsc_slot_get(Queue* q, size_t idx) {
  size_t stride = mpsc_slot_stride();
  return (Slot*)(q->slots + (idx & q->mask) * stride);
}

Payload* mpsc_slot_payload(Slot* s) {
  return (Payload*)((uint8_t*)s + sizeof(Slot));
}

Queue* mpsc_queue_create(size_t capacity) {
  if ((capacity & (capacity - 1)) != 0) return NULL; // power of 2 check

  size_t stride = mpsc_slot_stride();
  size_t total  = sizeof(Queue) + capacity * stride;

  // void* aligned_alloc(size_t alignment, size_t size); (since C11, stdlib.h)
  Queue* q = (Queue*)aligned_alloc(MPSC_CACHE_LINE, total);
  if (!q) return NULL;

  // Set the fields
  q->capacity  = capacity;
  q->mask      = capacity - 1;
  q->tail = 0;
  atomic_store_explicit(&q->head, 0, memory_order_relaxed);

  // Initialize every slot's seq by index
  for (size_t i = 0; i < capacity; i++) {
    Slot* s = (Slot*)(q->slots + i * stride);
    atomic_store_explicit(&s->seq, i, memory_order_relaxed);
  }

  atomic_thread_fence(memory_order_seq_cst); // init barrier
  return q;
}

void mpsc_queue_destroy(Queue* q) {
  free(q);
}

/*
 * PRODUCER - thread safe, multiple threads can call
 */
bool mpsc_queue_push(Queue* q, const Payload* data) {
  size_t pos = atomic_load_explicit(&q->head, memory_order_relaxed);

  size_t seq;
  Slot* s;
  for (;;) {
    s = mpsc_slot_get(q, pos);
    seq = atomic_load_explicit(&s->seq, memory_order_acquire);
    intptr_t diff = (intptr_t)(seq - pos);

    if (diff == 0) {
      if (atomic_compare_exchange_weak_explicit(
            &q->head, &pos, pos + 1,
            memory_order_relaxed, memory_order_relaxed)) {
        break; // claim success
      }
      // another producer claimed, retry
    } else if (diff < 0) {
      // ring is full
#ifndef MPSC_DROP_THE_SLOT
      mpsc_adaptive_wait(&spins); // block the thread
#else
      return false;
#endif
    } else { // pos stale, re-read
      pos = atomic_load_explicit(&q->head, memory_order_relaxed);
    }
  }

  // Copy the data to payload
  memcpy(mpsc_slot_payload(s), data, sizeof(Payload));

  // Slot ready signal to consumer
  atomic_store_explicit(&s->seq, pos + 1, memory_order_release);

  return true;
}

/*
 * CONSUMER - being called by single thread
 * out: buffer to write payload into
 * (in logger, this can be stdout/logFile stream)
 */
bool mpsc_queue_pop(Queue* q, Payload* out) {
  size_t pos = q->tail;
  Slot*  s   = mpsc_slot_get(q, pos);

  size_t seq = atomic_load_explicit(&s->seq, memory_order_acquire);

  // seq == pos + 1 -> producer finished
  if (seq != pos + 1) {
    return false; // not ready yet
  }

  memcpy(out, mpsc_slot_payload(s), sizeof(Payload));

  /*
   * Free the slot: seq = pos + capacity
   * Therefore, slot can be reused
   */
  atomic_store_explicit(&s->seq, pos + q->capacity, memory_order_release);

  q->tail = pos + 1;
  return true;
}

void mpsc_adaptive_wait(int* spins) {
  if (!spins) return;
  if (*spins < MPSC_WAIT_NO_PAUSE_MAGIC) {
    *spins += 1;
    // no pause
  } else if (*spins < MPSC_WAIT_PAUSE_MAGIC) {
    *spins += 1;
    MPSC_PAUSE_INS();
  } else {
    struct timespec ts = {0, 1000};
    nanosleep(&ts, NULL); // real sleep
  }
}

void mpsc_adaptive_wait_reset(int* spins) {
  *spins = 0;
}

#endif // MPSC_IMPLEMENTATION

#ifdef MPSC_STRIP_PREFIXES
#define slot_stride mpsc_slot_stride
#define slot_get mpsc_slot_get
#define slot_payload mpsc_slot_payload
#define queue_create mpsc_queue_create
#define queue_destroy mpsc_queue_destroy
#define queue_push mpsc_queue_push
#define queue_pop mpsc_queue_pop
#define adaptive_wait mpsc_adaptive_wait
#define adaptive_wait_reset mpsc_adaptive_wait_reset
#endif // MPSC_STRIP_PREFIXES

#endif // MPSC_H
