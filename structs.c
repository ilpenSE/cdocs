#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int width;
  int height;
} Rectangle;

Rectangle set_rect(int w, int h) {
  Rectangle rect;
  rect.width = w;
  rect.height = h;
  return rect;
}

void print_area(Rectangle* rect) {
  // NON-NESTING
  if (NULL == rect) { // Yoda condition
    perror("Null pointer received");
    return;
  }
  int area = rect->width * rect->height;
  printf("Area: %d\n", area);

  // NESTING
  /*
  if (rect != NULL) {
    int area = rect->width * rect->height;
    printf("Area: %d\n", area);
  } else {
    printf("Invalid rectangle pointer\n");
  }*/
}

int main() {
  // dynamic allocation
  Rectangle* rects = (Rectangle*)malloc(3 * sizeof(Rectangle));
  Rectangle rectarr[3] = {0};

  if (rects == NULL) {
    perror("Failed to allocate memory");
    return 1;
  }

  printf("\nUsing pointer of structs:\n");
  *rects = set_rect(10, 20);
  *(rects + 1) = set_rect(30, 40);
  *(rects + 2) = set_rect(50, 60);

  print_area(rects);
  print_area(rects + 1);
  print_area(rects + 2);

  printf("\nUsing array of structs:\n");
  rectarr[0] = set_rect(15, 25);
  rectarr[1] = set_rect(35, 45);
  rectarr[2] = set_rect(55, 65);
  print_area(&rectarr[0]);
  print_area(&rectarr[1]);
  print_area(&rectarr[2]);

  free(rects);
  return 0;
}