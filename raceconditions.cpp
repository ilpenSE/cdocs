#include <iostream>
#include <thread>

int counter = 0; // shared variable

void increment() {
  for (int i = 0; i < 1000; ++i) {
    counter++; // race condition
  }
}

/**
  EXPLAINING:

  This is a classic race condition situation. Race conditions are about
  >1 different concurrent* thread trying to make their changes
  For example in this code, we have counter and increment function.
  Increment function is incrementing counter for 1000 times and so,
  if counter = 0 then increment() makes counter = 1000. BUT: if we have 2 different threads
  and we tryna run them concurrently, we expect counter to be 2000 but it is NOT.
  This is because our counter++; is actually 3 different instruction: LOAD, INC, STORE
  Kernel can interrupt this thread (in order to run concurrent tasks) at any instruction.
  What does means: if we've been interrupted at STORE, this means we've "incremented" counter but dont store,
  in other cycle we load same value as before (not incremented value) and we tryna increment that. Result is not 2, its 1.

  Example Flow: (INSTRUCTIONX, x stands for which thread is)
| Time  | Thread A  | Thread B  | Counter |
| ----- | --------- | --------- | ------- |
| t0    | LOAD 0    |           | 0       |
| t1    | (int.ed)  |           | 0       |
| t2    |           | LOAD 0    | 0       |
| t3    |           | INC → 1   | 0       |
| t4    |           | STORE → 1 | 1       |
| t5    | INC → 1   |           | 1       |
| t6    | STORE → 1 |           | 1       |

counter is NOT 2

*: its not concurrent acutally, you can see it in the example flow

Sometimes it can show expected result but its not stable, and our example is so simple
that modern system can handle this but if you run this on and on, you sometimes get different results than 2000
*/
int main() {
  std::thread t1(increment);
  std::thread t2(increment);

  t1.join();
  t2.join();

  std::cout << "Counter: " << counter << std::endl; // expected: 2000
  return 0;
}

