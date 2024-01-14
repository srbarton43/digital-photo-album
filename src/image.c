#include <stdio.h>
#include <unistd.h>

#include "image.h"

int img_resize(const char *srcname, const char *dstname, const char *percent) {
  int rc;
  rc = fork();
  if (0 == rc) {
    rc = execlp("convert", "convert", "-resize", percent, srcname, dstname, NULL);
    // shouldn't reach here
    fprintf(stderr, "error %d with resize", rc);
  }
  return rc;
}

int img_rotate(const char *srcname, const char *dstname, int rotation) {
  int rc;
  printf("rotating %s\n", srcname);
  rc = fork();
  if (0 == rc) {
    switch (rotation)
    {
    case CLOCKWISE:
      rc = execlp("convert", "convert", "-rotate", "90", srcname, dstname, NULL);
      break;
    case CCLOCKWISE:
      rc = execlp("convert", "convert", "-rotate", "-90", srcname, dstname, NULL);
      break;
    default:
      fprintf(stderr, "error: Unsupported rotation\n");
      break;
    }
  }
  return rc;
}

int img_display(const char *srcname) {
  int rc;
  rc = fork();
  if (0 == rc) {
    rc = execlp("display", "display", srcname, NULL);
    // shouldn't reach here
    fprintf(stderr, "error %d with display\n", rc);
  }
  return rc;
}
