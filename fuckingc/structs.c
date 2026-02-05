#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int age;
  char* name;
} Person;

void print_person(Person p) {
  printf("Name: %s, Age: %d\n", p.name, p.age);
}

int main() {
  const char* names[] = {"Bob", "Alice", "Mehmet", "Emir", "Dave"};

  printf("sizeof Person: %zu\n", sizeof(Person));

  Person* ppl = malloc(5*sizeof(*ppl));

  for (int i = 0; i < 5; ++i) {
    ppl[i].name = names[i];
    print_person(ppl[i]);
  }
  return 0;
}
