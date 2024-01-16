#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "image.h"

/* img_resize */
void img_resize(const char *srcname, const char *dstname, const char *percent) {
  int rc, pid, status;
  if (strchr(percent, '%') == NULL) { /* percent param must literally contain '%' */
    fprintf(stderr, "Forgotten \'%%\' sign\n");
    exit(1);
  }
  pid = fork();
  if (0 == pid) {
    rc = execlp("convert", "convert", "-resize", percent, srcname, dstname,
                NULL);
    // shouldn't reach here
    fprintf(stderr, "error %d with resize", rc);
    exit(1);
  }
  waitpid(pid, &status, 0);
}

/* img_rotate */
void img_rotate(const char *srcname, const char *dstname, int rotation) {
  int rc, pid, status;
  printf("rotating %s\n", srcname);
  pid = fork();
  if (0 == pid) {
    switch (rotation) {
    case CLOCKWISE:
      rc =
          execlp("convert", "convert", "-rotate", "90", srcname, dstname, NULL);
      fprintf(stderr, "error %d with rotation\n", rc);
      exit(1);
      break;
    case CCLOCKWISE:
      rc = execlp("convert", "convert", "-rotate", "-90", srcname, dstname,
                  NULL);
      fprintf(stderr, "error %d with rotation\n", rc);
      exit(1);
      break;
    default:
      fprintf(stderr, "error: Unsupported rotation\n");
      exit(1);
      break;
    }
  }
  waitpid(pid, &status, 0);
}

/* img_display */
int img_display(const char *srcname) {
  int rc;
  rc = fork();
  if (0 == rc) {
    rc = execlp("display", "display", srcname, NULL);
    // shouldn't reach here
    fprintf(stderr, "error %d with display\n", rc);
    exit(1);
  }
  return rc;
}
