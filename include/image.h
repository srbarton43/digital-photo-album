#ifndef __IMAGE_H
#define __IMAGE_H

/*
 * library for calling ImageMagick commands
 */

#define CLOCKWISE  0
#define CCLOCKWISE 1

int img_rotate(const char *srcname, const char *dstname, int rotation);

int img_resize(const char *srcname, const char *dstname, const char *percent);

/* 
 * img_display - display the image
 * 
 * returns:
 *  PID of the display process
 */
int img_display(const char *srcname);
#endif