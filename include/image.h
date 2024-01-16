#ifndef __IMAGE_H
#define __IMAGE_H

/*
 * image.h - module for calling ImageMagick commands
 * 
 * SRB -- 24W
 * 
 * Each function wraps syscalls and ImageMagick commands
 */

#define CLOCKWISE  0
#define CCLOCKWISE 1

/*
 * img_rotate - rotate image using ImageMagick
 *
 * srcname  := original image path
 * dstname  := rotates image path
 * rotation := either CLOCKWISE or CCLOCKWISE
 *
 * note: the function waits for the conversion to finish before returning
 */
void img_rotate(const char *srcname, const char *dstname, int rotation);

/*
 * img_resize - resizes image to some percentage
 *
 * srcname  := original image path
 * dstname  := rotates image path
 * percent  := percent to resize to (eg. "10%")
 * 
 * note: the function waits for the conversion to finish before returning 
 */
void img_resize(const char *srcname, const char *dstname, const char *percent);

/* 
 * img_display - display the image
 *
 * srcname := path of image to display
 * 
 * returns:
 *  PID of the display process
 */
int img_display(const char *srcname);
#endif

