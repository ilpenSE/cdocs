#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int *items;
	size_t count;
	size_t capacity;
} Numbers;

typedef struct {
	float *items;
	size_t count;
	size_t capacity;
} Prices;

typedef struct {
	char* *items;
	size_t count;
	size_t capacity;
} Names;

#define da_append(darr, item) do { \
		if (darr.count >= darr.capacity) { \
			if (darr.capacity == 0) darr.capacity = 32; \
			else darr.capacity *= 2; \
			darr.items = realloc(darr.items, darr.capacity * sizeof(*darr.items)); \
		} \
		darr.items[darr.count++] = item; \
	} \
	while (0)

int main() {
	Numbers nums = {0};
	for (int i = 0; i < 10; ++i) da_append(nums, i);
	for (int i = 0; i < nums.count; ++i) printf("%d\n", nums.items[i]);

	Prices prices = {0};
	for (float i = 0.0f; i < 50.0f; i += 1.0f) da_append(prices, i);
	for (int i = 0; i < prices.count; ++i) printf("%f\n", prices.items[i]);

	Names names = {0};
	da_append(names, "World");
	da_append(names, "Freunde!");
	da_append(names, "ilpeN");
	for (int i = 0; i < names.count; ++i) printf("%s\n", names.items[i]);

	return 0;
}
