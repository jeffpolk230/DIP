/* using libpng.
* Take in a .png file, and output two laplacian filtered images named "laplacian_withWeight_.png"
* AIT_UG Digital image processing, Assignment one. Author: Jeff Chen
*
* ### notice the mapping function is rough still, need some patching
*/
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>




png_structp png_ptr;
png_infop info_ptr;

/*Laplacian Mask, or more precisely high-pass filter*/
int mask[3][3] = 
{
  {0, -1, 0},
  {-1, 4, -1},
  {0, -1, 0}
};
/*mask2 with bigger weight*/
int mask2[3][3] = 
{
  {-1, -1, -1},
  {-1, 8, -1},
  {-1, -1, -1}
};
int main(int argc, char *argv[])
{
  unsigned char header[8];
  if ( argc != 2 )
   {
        printf( "usage: %s filename\n", argv[0] );
    }
  FILE *fp = fopen(argv[1],"rb");
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
  int x, y, k, i, j; /* x, y, i, j will serve as loop vars */
  k = (int)((log(bit_depth)/log(2)));
  printf("%d\n", k);
  for (y = 0; y < height; y++)
    row_pointers[y] = (png_bytep) malloc(width*k);

  png_read_image(png_ptr, row_pointers);

  printf("color_type:%d\nbit_depth: %d\n",color_type, bit_depth);
  
  //create an empty out with row_pointers2
  png_bytep *row_pointers2 = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y = 0; y < height; y++)
    row_pointers2[y] = (png_bytep) malloc(width*k);

  //very lamp way to create a copy of same dimension of pointer. will patch this up

  //processing image using mask 2 (higher weight)
  for (y = 1; y< height -1; y++)
  {
    for(x = 1; x < width -1; x ++)
    {
      png_byte *ptr2 = &((row_pointers2[y])[x*k]);
      int r = 0;
      int g = 0;
      int b = 0;
      int a = 0;
      for(j = -1; j < 2; j++)
      {
        for(i = -1; i < 2; i++)
        {
          png_byte *ptr = &((row_pointers[y + j])[(x + i)*k]);    
          //ptr[0] = 255 - ptr[0]; //shouldn't invert alpha :)
          a += mask2[j + 1][i + 1] * ptr[0];
          r += mask2[j + 1][i + 1] * ptr[1];
          g += mask2[j + 1][i + 1] * ptr[2];
          b += mask2[j + 1][i + 1] * ptr[3];
        }     
      }
      ptr2[0] = a +128;
      ptr2[1] = r +128;
      ptr2[2] = g +128;
      ptr2[3] = b +128;
    } 

  }
  //output
  FILE *outfp = fopen("laplacianMaskWeight_8.png", "wb");
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, outfp);
  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);  
  png_write_image(png_ptr, row_pointers2);
  png_write_end(png_ptr, NULL);

  //processing image using mask one
  for (y = 1; y< height -1; y++)
  {
    for(x = 1; x < width -1; x ++)
    {
      png_byte *ptr2 = &((row_pointers2[y])[x*k]);
      int r = 0;
      int g = 0;
      int b = 0;
      int a = 0;
      for(j = -1; j < 2; j++)
      {
        for(i = -1; i < 2; i++)
        {
          png_byte *ptr = &((row_pointers[y + j])[(x + i)*k]);    
          //ptr[0] = 255 - ptr[0]; //shouldn't invert alpha :)
          a += mask[j + 1][i + 1] * ptr[0];
          r += mask[j + 1][i + 1] * ptr[1];
          g += mask[j + 1][i + 1] * ptr[2];
          b += mask[j + 1][i + 1] * ptr[3];
        }     
      }
      ptr2[0] = a +128;
      ptr2[1] = r +128;
      ptr2[2] = g +128;
      ptr2[3] = b +128;
    } 

  }


  outfp = fopen("laplacianMaskWeight_4.png", "wb");
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, outfp);
  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr, info_ptr);  
  png_write_image(png_ptr, row_pointers2);
  png_write_end(png_ptr, NULL);

  free(row_pointers);

  fclose(fp);
  fclose(outfp);


}
