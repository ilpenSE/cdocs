#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* make_dangling() {
    int x = 5;
    return &x; // returning address of local variable (undefined behavior)
    // x goes out of scope after function returns
    // SAFE USAGE:
    // int* p = malloc(sizeof(int)); -> allocate memory on heap (use free() later)
    // *p = 5;
    // return p;
}

int danglingptr() {
    int* p = make_dangling();
    printf("Value pointed to by p: %d\n", *p); // dereferencing dangling pointer (undefined behavior)
    return 0;
}

int uninitializedptr() {
    char* str; // uninitialized pointer
    // TRUE USAGE:
    // char* str = malloc(100 * sizeof(char)); -> allocate memory on heap (use free() later)
    // strcpy(str, "Hello");
    // OR:
    // char str[100]; -> allocate memory on stack
    size_t len = strlen(str); // using strlen on uninitialized pointer (undefined behavior)
    printf("Length of string: %zu\n", len);
    return 0;
}

int bufferoverflow() {
    char buffer[10];
    printf("Enter a string: ");
    scanf("%s", buffer); // no length check, can cause buffer overflow (undefined behavior)
    // SAFE USAGE:
    // fgets(buffer, sizeof(buffer), stdin); -> safer alternative
    printf("You entered: %s\n", buffer);
    return 0;
}

int useafterfree() {
    char* str = malloc(100 * sizeof(char));
    strcpy(str, "Hello, World!");
    free(str); // free allocated memory
    printf("String after free: %s\n", str); // using memory after free (undefined behavior)
    return 0;
}

int outofbounds() {
    int arr[5] = {1,2,3,4,5};
    printf("%d\n", arr[10]); // UB: access past array
    return 0;
}

int invalidfree() {
    int x = 10;
    free(&x); // UB: freeing stack memory
    return 0;
}

int misaligned() {
    char buffer[sizeof(int) * 2];
    int* p = (int*)(buffer + 1); // intentionally misalign
    *p = 42; // UB: may segfault on strict alignment CPUs
    printf("Value at misaligned address: %d\n", *p);
    return 0;
}

int main() {
    printf("Which segfault do you wanna see?\n");
    printf("[1] Using unallocated memory (char* and strlen)\n");
    printf("[2] Out-Of-Scope pointer usage which causes dangling pointers\n");
    printf("[3] Buffer Overflows or Unchecked user input\n");
    printf("[4] Use After Free\n");
    printf("[5] Out of Bounds Array Access\n");
    printf("[6] Invalid Free\n");
    printf("[7] Misaligned Access (may not segfault on all systems)\n");
    printf("Enter choice: ");
    int choice;
    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
    case 1:
        return uninitializedptr();
    case 2:
        return danglingptr();
    case 3:
        return bufferoverflow();
    case 4:
        return useafterfree();
    case 5:
        return outofbounds();
    case 6:
        return invalidfree();
    case 7:
        return misaligned();
    default:
        break;
    }

    return 1;
}
