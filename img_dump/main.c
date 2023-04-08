#include <stdint.h>
#include <stdio.h>

#include "kachow.h"

#define SECTOR_SIZE 512

struct img {
  const uint8_t *img;
  /* Width and height after image rotation */
  uint32_t       width;
  uint32_t       height;
};

#define IMG_COUNT 1
struct img img_table[IMG_COUNT] = {
  {kachow, 96, 96}
};

int main(int argc, char *args[])
{
  FILE *file;
  file = fopen("images.img", "wb");

  for (uint32_t img = 0; img < IMG_COUNT; ++img) {
    printf("Sector start: %d\n", ftell(file) / SECTOR_SIZE);
    uint32_t bytes = img_table[img].width * img_table[img].height;
    const uint8_t *ptr = &img_table[img].img[0];
    while (bytes > SECTOR_SIZE) {
      fwrite(ptr, 1, SECTOR_SIZE, file);
      ptr += SECTOR_SIZE;
      bytes -= SECTOR_SIZE;
    }
    /* Write the remaining bytes */
    fwrite(ptr, 1, bytes, file);
    for (uint32_t i = 0; i < SECTOR_SIZE - bytes; ++i) {
      fputc(0, file);
    }
  }

  fclose(file);
  return 0;
}