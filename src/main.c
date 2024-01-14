#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "image.h"
#include "html.h"

#define MAX_INPUT_LEN 32

int process_file(const char *filename);
int get_input(const char *message, char *buffer, int len);
int ask_rotation(const char *filename, const char *bn);
int ask_caption(const char *filename, char *caption);

int main(int argc, char *argv[]) {
  html_init();
  for (int i = 1; i < argc; i++) {
    process_file(argv[i]);
  }
  html_write_to_file();
}

int process_file(const char *filename) {
  int pid, rc, status; /* setup */
  printf("%s\n", filename);
  char copy[strlen(filename) + 1];
  strcpy(copy, filename);
  char *bn = basename(copy); /* filename stripped of path */
  printf("basename: %s\n", bn);
  char thumbnail_fn[strlen(bn) + 12]; /* thumbnail filename */
  sprintf(thumbnail_fn, "tn_%s", bn);
  pid = img_resize(filename, thumbnail_fn, "10%");
  waitpid(pid, &status, 0); /* must wait for child to create thumbnail */
  int display_pid = img_display(thumbnail_fn);
  /* prompt user for rotation */
  rc = ask_rotation(filename, bn);
  /* prompt user for caption */
  char *caption = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = ask_caption(filename, caption);
  if (rc != -1) printf("Caption: %s\n", caption);

  html_add_photo(bn, caption);

//  printf("wait 3 seconds and kill display\n");
//  sleep(3);
  rc = kill(display_pid, SIGINT);
//  printf("kill ret: %d\n", rc);

  return 0;
}

int ask_rotation(const char *filename, const char *bn) {
  int rc, pid, status;
  /* get thumbnail filename and medium filename */
  char thumbnail_fn[strlen(bn)+12];
  sprintf(thumbnail_fn, "tn_%s", bn);
  char medium_fn[strlen(bn)+12];
  sprintf(medium_fn, "med_%s", bn);
  char *buffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = get_input("rotate the image 90 degrees? (cw=clockwise, ccw=counterclockwise, <empty>=no rotation)", buffer, MAX_INPUT_LEN);
  if (-1 == rc) {
  } else if (strlen(buffer) == 0) {
    // no need to rotate thumbnail
    img_resize(filename, medium_fn, "25%");
  } else if (strcmp(buffer, "cw") == 0) {
    printf("rotate cw\n");
    pid = img_rotate(filename, medium_fn, CLOCKWISE);
    waitpid(pid, &status, 0);
    img_resize(medium_fn, medium_fn, "25%");
    img_rotate(thumbnail_fn, thumbnail_fn, CLOCKWISE);
  } else if (strcmp(buffer, "ccw") == 0) {
    printf("rotate ccw\n");
    pid = img_rotate(filename, medium_fn, CCLOCKWISE);
    waitpid(pid, &status, 0);
    img_resize(medium_fn, medium_fn, "25%");
    img_rotate(thumbnail_fn, thumbnail_fn, CCLOCKWISE);
  } else {
    fprintf(stderr, "invalid option for rotating the image\n");
    rc = -1;
  }
  free(buffer);
  return rc;
}

int ask_caption(const char *filename, char *caption) {
  int rc;
  char *buffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = get_input("Enter A Caption", buffer, MAX_INPUT_LEN);
  if (-1 == rc) {
    // error in get_input
  } else if (strlen(buffer) == 0) {
    fprintf(stderr, "Empty Caption!\n");
    strcpy(caption, "Untitled");
  } else {
    strcpy(caption, buffer);
  }
  free(buffer);
  return rc;
}

/*
 * get_input - gets input from user
 *
 * credit: adam salem & s. smith -- demo.c
 *
 * - message  := message to print out
 * - buffer   := allocated string with size len
 * - len      := maximum string length
 *
 * returns:
 *  -  0 if no errors
 *  - -1 if input string longer than len
 *
 */
int get_input(const char *message, char *buffer, int len) {
  int rc = 0, fetched, lastchar;
  if (buffer == NULL) return -1;

  if (message)
    printf("%s: ", message);

  // get input string
  fgets(buffer, len, stdin); /* new-line terminated string goes in buffer */
  fetched = strlen(buffer);  /* length of input string */

  if (fetched + 1 >= len) {
    fprintf(stderr, "input string exceeded allowed length\n");
    rc = -1;
  }

  // consume trailing \n
  if (fetched) {
    lastchar = fetched - 1;
    if (buffer[lastchar] == '\n')
      buffer[lastchar] = '\0';
  }

  return rc;
}
