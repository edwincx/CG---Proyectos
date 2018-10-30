
//LodePNG Examples
/*

Para compilar: gcc -o main main.c lodepng.c

*/

#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>



/*
Example 1
Encode from raw pixels to disk with a single function call
The image argument has width * height RGBA pixels or width * height * 4 bytes
*/

int main(int argc, char *argv[])
{
  const char* filename =  "test.png";

  /*generate some image*/
  unsigned width = 512, height = 512;
  unsigned char* image = malloc(width * height * 4);
  unsigned x, y;
  for(y = 0; y < height; y++)
  for(x = 0; x < width; x++)
  {
    image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
    image[4 * width * y + 4 * x + 1] = x ^ y;
    image[4 * width * y + 4 * x + 2] = x | y;
    image[4 * width * y + 4 * x + 3] = 255;
  }


  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));


  free(image);
  return 0;
}
