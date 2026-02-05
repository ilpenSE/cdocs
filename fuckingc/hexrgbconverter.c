#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int showerr(char* errorstr) {
  printf("Error: %s\n", errorstr);
  return -1;
}

typedef struct {
  int r,g,b;
} RGB;

int isValidHex(char hexstr[7]) {
  int i;
  if (strlen(hexstr) != 6) return 0;
  if (hexstr[0] == '#') return 0;

  for (i = 0; i < 6; i++) {
    if (!((hexstr[i] >= '0' && hexstr[i] <= '9') || (hexstr[i] >= 'a' && hexstr[i] <= 'f') ||
        (hexstr[i] >= 'A' && hexstr[i] <= 'F'))) {
      return 0;
    }
  }
  return 1;
}

int isValidRGB(RGB rgb) {
  if (rgb.r < 0 || rgb.r > 255) return 0;
  if (rgb.g < 0 || rgb.g > 255) return 0;
  if (rgb.b < 0 || rgb.b > 255) return 0;
  return 1;
}

char* rgbstr(RGB rgb) {
  char* str = (char*)malloc(9*sizeof(char));
  if (str == NULL) return NULL;
  sprintf(str, "%d,%d,%d", rgb.r, rgb.g, rgb.b);
  return str;
}

RGB strrgb(char str[9]) {
  RGB rgb;
  sscanf(str, "%d,%d,%d", &rgb.r, &rgb.g, &rgb.b);
  return rgb;
}

RGB hex_to_rgb(char hex[7]) {
  RGB rgb;
  sscanf(hex, "%02x%02x%02x", &rgb.r, &rgb.g, &rgb.b);
  return rgb;
}

char* rgb_to_hex(RGB rgb) {
  char* str = (char*)malloc(7*sizeof(char));
  sprintf(str, "%02x%02x%02x", rgb.r, rgb.g, rgb.b);
  return str;
}

int main(int argc, char** argv) {
  // R: HEX to RGB
  // H: RGB TO HEX
  char op = 'r';
  if (argc >= 2) {
    op = *(*(argv + 1));
  }
  switch (op) {
  case 'r':
    // HEX to RGB
    char hexstr[7];
    printf("Enter a hex string:\n");
    scanf("%6s", hexstr);
    if (strlen(hexstr) != 6) return showerr("Invalid hex string!");

    if (!isValidHex(hexstr)) return showerr("Invalid hex string!");

    RGB rgb = hex_to_rgb(hexstr);
    char* rgb_string = rgbstr(rgb);
    if (rgb_string == NULL) return showerr("RGB conversion failed!");
    printf("RGB: %s\n", rgb_string);
    free(rgb_string);
    break;
  case 'h':
    // RGB to HEX
    printf("Enter a RGB value:\n");
    char rgbstr[9];
    scanf("%8s", rgbstr);
    if (strlen(rgbstr) != 8) return showerr("Invalid rgb string! Format: R,G,B (no spaces and commas included)");

    RGB parsedrgb = strrgb(rgbstr);
    if (!isValidRGB(parsedrgb)) return showerr("Invalid rgb! Please enter a valid RGB value (0-255 for each component).");
    
    char* hex_string = rgb_to_hex(parsedrgb);
    if (hex_string == NULL) return showerr("hex string conversion failed. Please ensure the RGB values are in the correct format (e.g., 255,255,255).");
    
    printf("HEX: %s\n", hex_string);
    free(hex_string);
    break;
  default:
    printf("Invalid operation!");
    break;
  }  
  return 0;
}