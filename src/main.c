/*
 * main.c - entry point for album executable
 *
 * SRB -- 24W
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "html.h"
#include "util.h"

void check_args(const int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: album photo1.jpg [photo2.jpg...]");
    exit(99);
  }
}
int main(int argc, char *argv[]) {
  /* check the passed arguments */
  check_args(argc, argv);

  /* create pipe */
  int sv1[2];
  int rc, photo_index;
  rc = pipe(sv1); /* initialize the pipe */

  /*  check if pipe is valid */
  if (rc < 0) {
    fprintf(stderr, "Error creating pipe\n");
    exit(-1);
  }

  /* initialize html module */
  html_init();

  /* resize photos in child process */
  util_resize_photos(argc, argv, sv1);

  /* close write end of parent pipe and rename read end for clarity */
  close(sv1[WPIPE]);
  int from_child = sv1[RPIPE];

  /* for each photo that has finished resizing, process it */
  while (0 < read(from_child, &photo_index, sizeof(int))) {
    util_process_photo(argv[photo_index]);
  }
  printf("\nAll done!\n");

  /* write html contents to disk */
  html_write_to_file();

  return 0;
}
