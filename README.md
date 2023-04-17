Takes an array of pixels {uint8_t r, uint8_t g, uint8_t b} and outputs a .bmp image.
Useful for testing and very quick implementation. :^)

void print_rgb(void *data, uint32_t w, uint32_t h, char *file_name)

data - is an unsigned char array of RGB pixels
w - is the width
h - is the height
file_name - is the output name, add .bmp yourself or don't. :)
