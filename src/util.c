/*
 * util.c - util module source file
 *
 * SRB -- 24W
 *
 * see util.h for more details
 */
#include "util.h"

#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "html.h"
#include "image.h"

#define MAX_INPUT_LEN 32

/* static function prototypes */
static int get_input(const char *message, char *buffer, int len);
static void ask_rotation(const char *filename);
static int ask_caption(char *caption);
static void resize_photo(const char *filename);

/* util_resize_photos */
int util_resize_photos(const int argc, char **argv, int *sv1) {
  int rc, bytes;
  rc = fork(); /* do it all in a child process and return to main */
  if (0 == rc) {
    close(sv1[RPIPE]);          /* close the reading end of the pipe */
    int to_parent = sv1[WPIPE]; /* a more relavant name */
    for (int i = 1; i < argc; i++) {
      resize_photo(argv[i]);
      bytes = write(to_parent, &i, sizeof(int));
      if (bytes < sizeof(int)) {
        fprintf(stderr, "Error writing to pipe\n");
        exit(-1);
      }
    }
    close(to_parent); /* close pipe sending EOF to parent */
    exit(0);
  }
  return rc;
}

/* util_get_filenames */
void util_get_filenames(const char *filename, char *thumbnail_fn,
                        char *medium_fn) {
  char copy[strlen(filename) + 1]; /* copy the filename because the `basename`
                 spec mentions that it might alter its parameter */
  strcpy(copy, filename);
  char *bn = basename(copy); /* filename stripped of path */
  sprintf(thumbnail_fn, "tn_%s", bn);
  sprintf(medium_fn, "med_%s", bn);
}

/* util_process_file */
int util_process_photo(const char *filename) {
  int rc;
  printf("Photo Filename: %s\n", filename); /* print out filename */
  /* get html filenames */
  char thumbnail_fn[strlen(filename) + 4];
  char medium_fn[strlen(filename) + 5];
  util_get_filenames(filename, thumbnail_fn, medium_fn);

  /* display thumbnail and store the PID */
  int display_pid = img_display(thumbnail_fn);

  /* prompt user for rotation */
  ask_rotation(filename);

  /* prompt user for caption */
  char *caption = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = ask_caption(caption);
  printf("Caption: %s\n", caption);

  /* add photo + caption to html document buffer */
  html_add_photo(filename, caption);
  free(caption); /* free caption once it is in the html */

  /* kill display process */
  rc = kill(display_pid, SIGINT);

  return rc;
}

////////////////////////////////////////////////
///             STATIC FUNCTIONS             ///
////////////////////////////////////////////////

/*
 * resize_photo - creates medium and thumbnail sizes
 *
 * filename := path to photo for resizing
 */
static void resize_photo(const char *filename) {
  char thumbnail_fn[strlen(filename)];
  char medium_fn[strlen(filename)];
  util_get_filenames(filename, thumbnail_fn, medium_fn);

  /* resize images...no nead to wait */
  img_resize(filename, thumbnail_fn, "10%");
  img_resize(filename, medium_fn, "25%");
}

/*
 * ask_rotation - prompts user to rotate photo
 *
 * filename := path to image
 */
static void ask_rotation(const char *filename) {
  int rc;
  /* get thumbnail filename and medium filename */
  char thumbnail_fn[strlen(filename) + 4]; // accomodates for prefix
  char medium_fn[strlen(filename) + 5];    // accomodates for prefix
  util_get_filenames(filename, thumbnail_fn, medium_fn);
  char *buffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  while (1) {
    rc = get_input("rotate the image 90 degrees? (cw=clockwise, "
                   "ccw=counterclockwise, <empty>=stay-same)",
                   buffer, MAX_INPUT_LEN);
    if (-1 == rc) {
      // definitely bad input if its longer than MAX_INPUT_LEN
      fprintf(stderr, "invalid option for rotating the image...try again\n");
      continue;
    } else if (strlen(buffer) == 0) {
      /* no rotation */
      printf("Rotation: None\n");
    } else if (strcmp(buffer, "cw") == 0) {
      /* clockwize */
      img_rotate(medium_fn, medium_fn, CLOCKWISE);
      img_rotate(thumbnail_fn, thumbnail_fn, CLOCKWISE);
      printf("Rotation: Clockwise\n");
    } else if (strcmp(buffer, "ccw") == 0) {
      /* counterclockwise */
      img_rotate(medium_fn, medium_fn, CCLOCKWISE);
      img_rotate(thumbnail_fn, thumbnail_fn, CCLOCKWISE);
      printf("Rotation: Counter-Clockwise\n");
    } else {
      /* invalid option for rotation */
      fprintf(stderr, "invalid option for rotating the image...try again\n");
      continue; /* loops back to the top without breaking */
    }
    break;
  }
  free(buffer);
}

/*
 * ask_caption - asks user for caption
 *
 * caption  := alloc'd buffer of size MAX_INPUT_LEN + 1
 *
 * notes:
 *  stores caption in alloc'd caption
 *
 * returns:
 *  result of get_input, ie -1 if input overflows and zero otherwise
 */
static int ask_caption(char *caption) {
  int rc;
  char *buffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = get_input("Enter A Caption", buffer, MAX_INPUT_LEN);
  if (-1 == rc) {
    /* error in get_input */
    /* here I decided to allow the abridged caption to exist */
    strcpy(caption, buffer);
  } else if (strlen(buffer) == 0) {
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
static int get_input(const char *message, char *buffer, int len) {
  int rc = 0, fetched, lastchar;
  if (buffer == NULL)
    return -1;

  if (message)
    printf("%s: ", message);

  // get input string
  if (fgets(buffer, len, stdin) ==
      NULL) { /* new-line terminated string goes in buffer */
    fprintf(stderr, "EOF entered\n");
    return -1;
  }
  fetched = strlen(buffer); /* length of input string */

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
