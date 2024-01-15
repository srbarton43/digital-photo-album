#ifndef __IMAGE_H
#define __IMAGE_H

/*
 * library for calling ImageMagick commands
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
 * returns:
 *  - pid a command process
 */
int img_rotate(const char *srcname, const char *dstname, int rotation);

/*
 * img_resize - resizes image to some percentage
 *
 * srcname  := original image path
 * dstname  := rotates image path
 * percent  := percent to resize to (eg. "10%")
 *
 * returns:
 *  - pid of resize process
 */
int img_resize(const char *srcname, const char *dstname, const char *percent);

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

