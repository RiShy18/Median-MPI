#include <stdlib.h>
#include <stdio.h>
#include <MagickWand/MagickWand.h>
//E: Two integers
//R: NO negative numbers
//O: Argument for Qsort
int cmpfunc(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

//E: requires a path to a image, the full name of the image, the destiny of the path where the filtered image
//R: all paths must exists
//O: the filtered image with the average filter

void average_filter(const char *image_path, const char *image_name, const char *destiny_path)
{
  //startup of library instances
  MagickWand *m_wand, *copy_wand;
  PixelIterator *iterator_up, *iterator_center;
  PixelIterator *iterator_down, *iterator_write;
  PixelWand **pixels_up, **pixels_down, **pixels_center, **pixels_write;
  MagickBooleanType status;

  //picture's indexes
  int width, height, x, y;

  unsigned long width_iterator;
  char buffer[256];


  //image path
  sprintf(buffer, "%s%s", image_path, image_name);
  MagickWandGenesis();
  m_wand = NewMagickWand();
  status = MagickReadImage(m_wand, buffer);

  //Volatile Variables
  double red_pixel = 0;
  double green_pixel = 0;
  double blue_pixel = 0;

  //image opening excpetion
  if (status == MagickFalse)
    printf("Can't open image.\n");


  // Get the image's width and height
  width = MagickGetImageWidth(m_wand);
  height = MagickGetImageHeight(m_wand);

  //Creates a clone image, to write the copy, but not the original
  copy_wand = CloneMagickWand(m_wand);

  //Pixel Movements
  iterator_up = NewPixelIterator(m_wand);
  iterator_center = NewPixelIterator(m_wand);
  iterator_down = NewPixelIterator(m_wand);
  iterator_write = NewPixelIterator(copy_wand);

  if ((iterator_up == (PixelIterator *)NULL) || (iterator_center == (PixelIterator *)NULL) || (iterator_down == (PixelIterator *)NULL) || (iterator_write == (PixelIterator *)NULL))
    printf("Iterator error.\n");

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {

      PixelSetIteratorRow(iterator_center, y);
      pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
      if (((PixelGetRed(pixels_center[x]) * 255) == 0) || ((PixelGetGreen(pixels_center[x]) * 255) == 0) || ((PixelGetBlue(pixels_center[x]) * 255) == 0))
      {
        //Upper Left Corner
        if (y == 0 && x == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) / 3);
        }
        //Upper Right Corner 
        else if (x == width - 1 && y == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255)) / 3);
        }
        //Down Left Corner
        else if (y == height - 1 && x == 0)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) / 3);
        }
        //Down Right Corner
        else if (y == height - 1 && x == width - 1)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255)) / 3);
        }
        //Border collision
        else if (y > 0 && x > 0 && x < width - 1 && y < height - 1)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_up[x - 1]) * 255) + (PixelGetRed(pixels_up[x]) * 255) + (PixelGetRed(pixels_up[x + 1]) * 255) +
                        (PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255) +
                        (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       8);

          green_pixel = (((PixelGetGreen(pixels_up[x - 1]) * 255) + (PixelGetGreen(pixels_up[x]) * 255) + (PixelGetGreen(pixels_up[x + 1]) * 255) +
                          (PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255) +
                          (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         8);

          blue_pixel = (((PixelGetBlue(pixels_up[x - 1]) * 255) + (PixelGetBlue(pixels_up[x]) * 255) + (PixelGetBlue(pixels_up[x + 1]) * 255) +
                         (PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255) +
                         (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        8);
        }
        //Collision Border
        else if (y == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255) +
                        (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255) +
                          (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255) +
                         (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (x == 0)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_up[x]) * 255) + (PixelGetRed(pixels_up[x + 1]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) +
                        (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_up[x]) * 255) + (PixelGetGreen(pixels_up[x + 1]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) +
                          (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_up[x]) * 255) + (PixelGetBlue(pixels_up[x + 1]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) +
                         (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (y == height - 1)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) +
                        (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) +
                          (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) +
                         (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (x == width - 1)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_up[x - 1]) * 255) + (PixelGetRed(pixels_up[x]) * 255) + (PixelGetRed(pixels_center[x - 1]) * 255) +
                        (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_up[x - 1]) * 255) + (PixelGetGreen(pixels_up[x]) * 255) + (PixelGetGreen(pixels_center[x - 1]) * 255) +
                          (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_up[x - 1]) * 255) + (PixelGetBlue(pixels_up[x]) * 255) + (PixelGetBlue(pixels_center[x - 1]) * 255) +
                         (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255)) /
                        5);
        }

        PixelSetIteratorRow(iterator_write, y);
        pixels_write = PixelGetCurrentIteratorRow(iterator_write, &width_iterator);

        //Overwriting the new pixel
        sprintf(buffer, "rgb(%f,%f,%f)", red_pixel, green_pixel, blue_pixel);
        PixelSetColor(pixels_write[x], buffer);
        (void)PixelSyncIterator(iterator_write);
        //printf("r:%d,g:%d,b:%d\n",red_pixel,green_pixel,blue_pixel);
      }

      else
      {
        continue;
      }
    }
  }

  //free resources
  iterator_up = DestroyPixelIterator(iterator_up);
  iterator_center = DestroyPixelIterator(iterator_center);
  iterator_down = DestroyPixelIterator(iterator_down);
  iterator_write = DestroyPixelIterator(iterator_write);

  m_wand = DestroyMagickWand(m_wand);

  // Write the new image

  sprintf(buffer, "%s%s", destiny_path, image_name);
  status = MagickWriteImages(copy_wand, buffer, MagickTrue);
  if (status == MagickFalse)
    printf("Image writing error .\n");

  //Destroy copy image
  copy_wand = DestroyMagickWand(copy_wand);

  MagickWandTerminus();
}

void median_filter(const char *image_path, const char *image_name, const char *destiny_path)
{
  //library elements initializing
  MagickWand *m_wand, *copy_wand;
  PixelIterator *iterator_up, *iterator_center;
  PixelIterator *iterator_down, *iterator_write;
  PixelWand **pixels_up, **pixels_down, **pixels_center, **pixels_write;
  MagickBooleanType status;

  //indexes
  int width, height, x, y;

  unsigned long width_iterator;
  char buffer[256];

  //volatile variables
  double red_pixel = 0;
  double green_pixel = 0;
  double blue_pixel = 0;

  //image path
  sprintf(buffer, "%s%s", image_path, image_name);
  MagickWandGenesis();
  m_wand = NewMagickWand();
  status = MagickReadImage(m_wand, buffer);

  //image opening exception
  if (status == MagickFalse)
    printf("Can't open image.\n");

  //MagickReadImage(m_wand,"ruido.jpeg");

  // Get the image's width and height
  width = MagickGetImageWidth(m_wand);
  height = MagickGetImageHeight(m_wand);

  //Copy the image to not write original
  copy_wand = CloneMagickWand(m_wand);

  //Creating iterators
  iterator_up = NewPixelIterator(m_wand);
  iterator_center = NewPixelIterator(m_wand);
  iterator_down = NewPixelIterator(m_wand);
  iterator_write = NewPixelIterator(copy_wand);

  if ((iterator_up == (PixelIterator *)NULL) || (iterator_center == (PixelIterator *)NULL) || (iterator_down == (PixelIterator *)NULL) || (iterator_write == (PixelIterator *)NULL))
    printf("Iterator error.\n");

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {

      PixelSetIteratorRow(iterator_center, y);
      pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
      if (((PixelGetRed(pixels_center[x]) * 255) == 0) || ((PixelGetGreen(pixels_center[x]) * 255) == 0) || ((PixelGetBlue(pixels_center[x]) * 255) == 0))
      {
        if (y == 0 && x == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) / 3);
        }
        //Upper Right Corner
        else if (x == width - 1 && y == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255)) / 3);
        }
        //Down Left Corner
        else if (y == height - 1 && x == 0)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) / 3);
        }
        //Upper Right Corner
        else if (y == height - 1 && x == width - 1)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255)) / 3);
          green_pixel = (((PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255)) / 3);
          blue_pixel = (((PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255)) / 3);
        }
        //Upper Left Corner
        //Obtain the median factor
        else if (y > 0 && x > 0 && x < width - 1 && y < height - 1)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          int *sort_array = calloc(9, sizeof(int));
          sort_array[0] = (PixelGetRed(pixels_up[x - 1]) * 255);
          sort_array[1] = (PixelGetRed(pixels_up[x]) * 255);
          sort_array[2] = (PixelGetRed(pixels_up[x + 1]) * 255);
          sort_array[3] = (PixelGetRed(pixels_center[x - 1]) * 255);
          sort_array[4] = (PixelGetRed(pixels_center[x]) * 255);
          sort_array[5] = (PixelGetRed(pixels_center[x + 1]) * 255);
          sort_array[6] = (PixelGetRed(pixels_down[x - 1]) * 255);
          sort_array[7] = (PixelGetRed(pixels_down[x]) * 255);
          sort_array[8] = (PixelGetRed(pixels_down[x + 1]) * 255);

          qsort(sort_array, 9, sizeof(int), cmpfunc);
          red_pixel = sort_array[4];
          free(sort_array);

          int *sort_array2 = calloc(9, sizeof(int));
          sort_array2[0] = (PixelGetGreen(pixels_up[x - 1]) * 255);
          sort_array2[1] = (PixelGetGreen(pixels_up[x]) * 255);
          sort_array2[2] = (PixelGetGreen(pixels_up[x + 1]) * 255);
          sort_array2[3] = (PixelGetGreen(pixels_center[x - 1]) * 255);
          sort_array2[4] = (PixelGetGreen(pixels_center[x]) * 255);
          sort_array2[5] = (PixelGetGreen(pixels_center[x + 1]) * 255);
          sort_array2[6] = (PixelGetGreen(pixels_down[x - 1]) * 255);
          sort_array2[7] = (PixelGetGreen(pixels_down[x]) * 255);
          sort_array2[8] = (PixelGetGreen(pixels_down[x + 1]) * 255);

          qsort(sort_array2, 9, sizeof(int), cmpfunc);
          green_pixel = sort_array2[4];
          free(sort_array2);

          int *sort_array3 = calloc(9, sizeof(int));
          sort_array3[0] = (PixelGetBlue(pixels_up[x - 1]) * 255);
          sort_array3[1] = (PixelGetBlue(pixels_up[x]) * 255);
          sort_array3[2] = (PixelGetBlue(pixels_up[x + 1]) * 255);
          sort_array3[3] = (PixelGetBlue(pixels_center[x - 1]) * 255);
          sort_array3[4] = (PixelGetBlue(pixels_center[x]) * 255);
          sort_array3[5] = (PixelGetBlue(pixels_center[x + 1]) * 255);
          sort_array3[6] = (PixelGetBlue(pixels_down[x - 1]) * 255);
          sort_array3[7] = (PixelGetBlue(pixels_down[x]) * 255);
          sort_array3[8] = (PixelGetBlue(pixels_down[x + 1]) * 255);

          qsort(sort_array3, 9, sizeof(int), cmpfunc);
          blue_pixel = sort_array3[4];
          free(sort_array3);
        }
        //Collision Border
        else if (y == 0)
        {
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) + (PixelGetRed(pixels_down[x - 1]) * 255) +
                        (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) + (PixelGetGreen(pixels_down[x - 1]) * 255) +
                          (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) + (PixelGetBlue(pixels_down[x - 1]) * 255) +
                         (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (x == 0)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_up[x]) * 255) + (PixelGetRed(pixels_up[x + 1]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) +
                        (PixelGetRed(pixels_down[x]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_up[x]) * 255) + (PixelGetGreen(pixels_up[x + 1]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) +
                          (PixelGetGreen(pixels_down[x]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_up[x]) * 255) + (PixelGetBlue(pixels_up[x + 1]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) +
                         (PixelGetBlue(pixels_down[x]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (y == height - 1)
        {
          PixelSetIteratorRow(iterator_center, y - 1);
          PixelSetIteratorRow(iterator_down, y);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_center[x - 1]) * 255) + (PixelGetRed(pixels_center[x]) * 255) + (PixelGetRed(pixels_center[x + 1]) * 255) +
                        (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x + 1]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_center[x - 1]) * 255) + (PixelGetGreen(pixels_center[x]) * 255) + (PixelGetGreen(pixels_center[x + 1]) * 255) +
                          (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x + 1]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_center[x - 1]) * 255) + (PixelGetBlue(pixels_center[x]) * 255) + (PixelGetBlue(pixels_center[x + 1]) * 255) +
                         (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x + 1]) * 255)) /
                        5);
        }
        //Collision Border
        else if (x == width - 1)
        {
          PixelSetIteratorRow(iterator_up, y - 1);
          PixelSetIteratorRow(iterator_center, y);
          PixelSetIteratorRow(iterator_down, y + 1);

          pixels_up = PixelGetCurrentIteratorRow(iterator_up, &width_iterator);
          pixels_center = PixelGetCurrentIteratorRow(iterator_center, &width_iterator);
          pixels_down = PixelGetCurrentIteratorRow(iterator_down, &width_iterator);

          red_pixel = (((PixelGetRed(pixels_up[x - 1]) * 255) + (PixelGetRed(pixels_up[x]) * 255) + (PixelGetRed(pixels_center[x - 1]) * 255) +
                        (PixelGetRed(pixels_down[x - 1]) * 255) + (PixelGetRed(pixels_down[x]) * 255)) /
                       5);

          green_pixel = (((PixelGetGreen(pixels_up[x - 1]) * 255) + (PixelGetGreen(pixels_up[x]) * 255) + (PixelGetGreen(pixels_center[x - 1]) * 255) +
                          (PixelGetGreen(pixels_down[x - 1]) * 255) + (PixelGetGreen(pixels_down[x]) * 255)) /
                         5);

          blue_pixel = (((PixelGetBlue(pixels_up[x - 1]) * 255) + (PixelGetBlue(pixels_up[x]) * 255) + (PixelGetBlue(pixels_center[x - 1]) * 255) +
                         (PixelGetBlue(pixels_down[x - 1]) * 255) + (PixelGetBlue(pixels_down[x]) * 255)) /
                        5);
        }

        PixelSetIteratorRow(iterator_write, y);
        pixels_write = PixelGetCurrentIteratorRow(iterator_write, &width_iterator);

        //Writing the new pixel
        sprintf(buffer, "rgb(%f,%f,%f)", red_pixel, green_pixel, blue_pixel);
        PixelSetColor(pixels_write[x], buffer);
        (void)PixelSyncIterator(iterator_write);
      }

      else
      {
        continue;
      }
    }
  }
  //Cleaning tools
  iterator_up = DestroyPixelIterator(iterator_up);
  iterator_center = DestroyPixelIterator(iterator_center);
  iterator_down = DestroyPixelIterator(iterator_down);
  iterator_write = DestroyPixelIterator(iterator_write);

  m_wand = DestroyMagickWand(m_wand);

  // Write the new image

  sprintf(buffer, "%s%s", destiny_path, image_name);
  status = MagickWriteImages(copy_wand, buffer, MagickTrue);
  if (status == MagickFalse)
    printf("Image writing error .\n");
  //Destroy the copy
  copy_wand = DestroyMagickWand(copy_wand);

  MagickWandTerminus();
}
//E: Image path, image name and a path of destiny
void classify(const char *image_path, const char *image_name, const char *destiny_path)
{
  //buffer for strings
  char buffer[120];
  //images of indexes
  long y;
  register long x;
  unsigned long width;

  //library elements initializing
  MagickBooleanType status;
  MagickWand *copy_wand, *m_wand;
  PixelIterator *iterator;
  PixelWand **pixels;

  //accumulators for average
  float r_pixels = 0;
  float g_pixels = 0;
  float b_pixels = 0;

  //Initialize library
  MagickWandGenesis();
  m_wand = NewMagickWand();

  //Image path
  sprintf(buffer, "%s%s", image_path, image_name);
  status = MagickReadImage(m_wand, buffer);
  printf("%s\n", buffer);

  //Validation for opening
  if (status == MagickFalse)
    printf("Can't open image.\n");
  copy_wand = CloneMagickWand(m_wand);

  //Total of pixels
  int total = (int)MagickGetImageHeight(m_wand) * MagickGetImageWidth(m_wand);

  //Accumulate the amount of images
  iterator = NewPixelIterator(m_wand);
  if ((iterator == (PixelIterator *)NULL))
    printf("Iterator error.\n");
  for (y = 0; y < (long)MagickGetImageHeight(m_wand); y++)
  {
    pixels = PixelGetNextIteratorRow(iterator, &width);
    if ((pixels == (PixelWand **)NULL))
      break;
    for (x = 0; x < (long)width; x++)
    {
      r_pixels += PixelGetRed(pixels[x]) * 255;
      g_pixels += PixelGetGreen(pixels[x]) * 255;
      b_pixels += PixelGetBlue(pixels[x]) * 255;
    }
  }
  //Divide by total
  r_pixels /= total;
  g_pixels /= total;
  b_pixels /= total;
  printf("R: %f, G: %f, B: %f\n", r_pixels, g_pixels, b_pixels);

  //Choose folder according to result 
  //WARNING: Foler names in Spanish
  if (r_pixels >= g_pixels && r_pixels >= b_pixels)
    sprintf(buffer, "%srojas/%s", destiny_path, image_name);
  else if (g_pixels >= r_pixels && g_pixels >= b_pixels)
    sprintf(buffer, "%sverdes/%s", destiny_path, image_name);
  else if (b_pixels >= g_pixels && b_pixels >= r_pixels)
    sprintf(buffer, "%sazules/%s", destiny_path, image_name);

  //Free resources
  iterator = DestroyPixelIterator(iterator);
  m_wand = DestroyMagickWand(m_wand);
  status = MagickWriteImages(copy_wand, buffer, MagickTrue);
  if (status == MagickFalse)
    printf("Can't write image.\n");
  //Destroy image
  copy_wand = DestroyMagickWand(copy_wand);
  MagickWandTerminus();
}