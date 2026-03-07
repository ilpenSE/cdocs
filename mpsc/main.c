// This is basically lock-free async logger

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define MPSC_IMPLEMENTATION
#define MPSC_STRIP_PREFIXES
#define MPSC_DROP_THE_SLOT
#include "mpsc.h"

#define QUEUE_SIZE 1024

void print_payload(Payload* pyld) {
  printf("Payload{");
  printf("int a = %d, ", pyld->a);
  printf("int b = %d", pyld->b);
  printf("}\n");
}

typedef struct {
  ATOMIC(bool) isAlive;
  pthread_t consumer_thread;
  Queue* queue;
} Context;

// Basic processing, logger must accept context for configuration
void process_payload(Payload* payload) {
  printf("[CONSUMER] Recieved: ");
  print_payload(payload);
}

void* consumer_func(void* arg) {
  Context* ctx = (Context*)arg;
  Payload out = {0};
  int spins = 0;

  while (atomic_load_explicit(&ctx->isAlive, memory_order_acquire)) {
    if (queue_pop(ctx->queue, &out)) {
      spins = 0;
      process_payload(&out);
    } else {
      adaptive_wait(&spins);
    }
  }

  // isAlive = false, drain remaining entries
  while (queue_pop(ctx->queue, &out)) {
    printf("DRAINING ENTRIES!!\n");
    process_payload(&out);
  }

  return NULL;
}

bool producer_func(Context* ctx, Payload* payload) {
  if (!ctx || !payload) return false;
  if (!queue_push(ctx->queue, payload)) return false;
  printf("Pushed payload: ");
  print_payload(payload);
  return true;
}

bool destroyer_func(Context* ctx) {
  if (!ctx) return false;
  atomic_store_explicit(&ctx->isAlive, false, memory_order_release);
  if (pthread_join(ctx->consumer_thread, NULL) != 0) return false;
  queue_destroy(ctx->queue);
  // close logFile etc.
  return true;
}

bool init_func(Context* ctx) {
  if (!ctx) return false;
  ctx->queue = queue_create(QUEUE_SIZE);
  if (!ctx->queue) {
    fprintf(stderr, "Failed to create queue!\n");
    return false;
  }

  if (pthread_create(&ctx->consumer_thread, NULL, consumer_func, ctx) != 0) {
    fprintf(stderr, "Failed to create consumer thread!\n");
    return false;
  }

  atomic_store_explicit(&ctx->isAlive, true, memory_order_relaxed);
  return true;
}

int main() {
  Context ctx = {0};
  init_func(&ctx);

  Payload payload1 = {1, 2};
  Payload payload2 = {6, 7};
  Payload payload3 = {6, 9};

  producer_func(&ctx, &payload1);
  producer_func(&ctx, &payload2);
  producer_func(&ctx, &payload3);

  destroyer_func(&ctx);
  return 0;
}
