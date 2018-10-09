///////////////////////////////////////////////////
//                                              //
//      Instalacion                             //
//                                              //
//////////////////////////////////////////////////
/*
 1. Actualizar primero
  sudo apt-get update

 2. Biblioteca necesaria 
  sudo apt-get install libltdl-dev

 2. Bajar version 6 de Image-Magick
 wget  http://www.imagemagick.org/download/ImageMagick-6.9.6-1.tar.gz

 3.
 tar xvzf ImageMagick.tar.gz

 4.
 cd ImageMagick-7.0.3

 5.
 ./configure  --with-modules


 6.
 make

 7.
 sudo make install

 8. Configurar, si esto no sirve
 sudo ldconfig /usr/local/lib

Referencias:
http://www.imagemagick.org/script/install-source.php#unix
http://www.imagemagick.org/download/


 
 Para compilar:

 gcc -o main main.c `pkg-config --cflags --libs MagickCore`


*/
///////////////////////////////////////////////



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <magick/MagickCore.h>




///////////////////////////////////////////////////
//                                              //
//      Api                                     //
//                                              //
//////////////////////////////////////////////////
// Generar imagen
//Recive: nombre de archivo con pixeles, nombre de imagen que se va a generar
int generate_image( char* input_file_name,  char* output_image_name);

// Generar imagen y resize
//Recive: nombre de archivo con pixeles, nombre de imagen que se va a generar, size x, sieze y
int generate_resize_image(char* input_file_name, char* output_image_name, int sieze_x, int sieze_y);




///////////////////////////////////////////////////
//                                              //
//      Funciones                               //
//                                              //
//////////////////////////////////////////////////

int generate_image( char* input_file_name,  char* output_image_name)
{
  return generate_resize_image(input_file_name, output_image_name, 0, 0);
}



int generate_resize_image(char* input_file_name, char* output_image_name, int sieze_x, int sieze_y)
{
  char* file_name_input = input_file_name;
  char* file_name_output = output_image_name;

  int resize_x = sieze_x;
  int resize_y = sieze_y;

  ExceptionInfo *exception;
  Image *image, *images, *resize_image, *image_output;
  ImageInfo *image_info;


  //if (argc != 3)
  // {
  //     (void) fprintf(stdout,"Usage: %s image thumbnail\n",argv[0]);
  //     exit(0);
  // }
 
  //Initialize the image info structure and read an image.
  MagickCoreGenesis(NULL,MagickTrue);
  exception=AcquireExceptionInfo();


  image_info=CloneImageInfo((ImageInfo *) NULL);

  (void) strcpy(image_info->filename,file_name_input);

  images=ReadImage(image_info,exception);
  if (exception->severity != UndefinedException)
    CatchException(exception);
  if (images == (Image *) NULL)
    exit(1);


  /*
    Convert the image to a thumbnail.
  */
  image_output = NewImageList();
  while ((image=RemoveFirstImageFromList(&images)) != (Image *) NULL)
  {
    if(resize_x>0 && resize_y>0 )
    {
      resize_image=ResizeImage(image, resize_x, resize_y,LanczosFilter,1.0,exception);
      if (resize_image == (Image *) NULL)
        MagickError(exception->severity,exception->reason,exception->description);
        image = resize_image;
    }
    (void) AppendImageToList(&image_output,image);
    //DestroyImage(image);
  }
  /*
    Write the image thumbnail.
  */

  (void) strcpy(image_output->filename,file_name_output);
  WriteImage(image_info,image_output);
  /*
    Destroy the image thumbnail and exit.
  */
  image_output=DestroyImageList(image_output);
  image_info=DestroyImageInfo(image_info);
  exception=DestroyExceptionInfo(exception);
  MagickCoreTerminus();
  return(0);
}
