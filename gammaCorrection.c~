/* using libpng.
* Take in a .png file, and output a gamma corrected image name  "gamma.png"
* AIT_UG Digital image processing, Assignment one. Author: Jeff Chen
*/
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>
#include <string.h>



png_structp png_ptr;
png_infop info_ptr;
int main(int argc, char *argv[])
{
  unsigned char header[8];
  if ( argc != 3 )
   {
        printf( "usage: %s filename gamma_value", argv[0] );
    }
  FILE *fp = fopen(argv[1],"rb");
  float gamma ;
  sscanf(argv[2], "%f", &gamma);
  if(!fp)
    printf("not a file");
  fread(header, 1, 8, fp);
  int is_png = !png_sig_cmp(header, 0, 8); //Quite weird. cmp return 0 if bytes match PNG signature
  if(!is_png)
    printf("not a png");
  else
    printf("input file confirmed as png\n");

  //Creating png_structp
  png_structp png_ptr = png_create_read_struct
	(PNG_LIBPNG_VER_STRING, 
	 NULL, //(png_voidp)user_error_ptr, 
	 NULL, //user_error_fn, 
	 NULL); //user_warning_fn);
  
  if(!png_ptr)
    printf("png_ptr construct error\n");

  //Creating png_infop
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
  {
    png_destroy_read_struct(&png_ptr,(png_infopp) NULL,(png_infopp) NULL);
    printf("something wrong with pngInfo\n");
  }
  
  //setjmp for error condition from libpng

  if(setjmp(png_jmpbuf(png_ptr)))
    printf("something wrong with jmp setting\n");

  //eventually, we are reading the file :-)
  png_init_io(png_ptr, fp);
  //since we've sampled 8 byes for testing if it is png, we need to let libpng know 
  png_set_sig_bytes(png_ptr, 8);

  //There're bunch of error handling from the manual... we skip as usual
/*
  //High-level of reading interface, we'll read into memory directly
  png_read_png(png_ptr, info_ptr, 0, NULL); //third para as png_transforms bitwise OR mask
  png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
*/

  /* low level of reading, we need it to manipulate more detailed stuff*/
  png_read_info(png_ptr, info_ptr);
  
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  int x, y,k;
  k = (int)((log(bit_depth)/log(2)));
  printf("%d\n", k);
  for (y = 0; y < height; y++)
    row_pointers[y] = (png_bytep) malloc(width*k);

  png_read_image(png_ptr, row_pointers);

  printf("color_type:%d\nbit_depth: %d\n",color_type, bit_depth);
  

  //processing image
  for (y = 0; y< height; y++)
  {
    png_byte* row = row_pointers[y];
    for(x = 0; x < width; x ++)
    {
      png_byte* ptr = &(row[x*k]);    
      //ptr[0] = 255 - ptr[0]; //shouldn't invert alpha :)
      //ptr[0] = (int)(pow(ptr[0], 0.9)) ;
      ptr[1] = (int)(255 * pow((ptr[1]/255.0), gamma)) ;
      ptr[2] = (int)(255 * pow((ptr[2]/255.0), gamma)) ;
      ptr[3] = (int)(255 * pow((ptr[3]/255.0), gamma)) ;
    } 

  }
  //output
  FILE *outfp = fopen("gammaout.png", "wb");
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, outfp);
  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);  
  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, NULL);

  free(row_pointers);

  fclose(fp);
  


}
