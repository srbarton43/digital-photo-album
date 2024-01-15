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
#define MAX_PROCESSES 4

// static function prototypes
static int get_input(const char *message, char *buffer, int len);
static void ask_rotation(const char *filename);
static int ask_caption(char *caption);
static void resize_photo(const char *filename);

/* util_resize_photos */
int util_resize_photos(const int argc, char **argv) {
  int rc;
  rc = fork();  // do it all in a child process and return to main
  if (0 == rc) {
    sleep(1); /* removes input lag for the first image */
    /* create four processes */
    int p1, p2, p3, p4;
    p1 = fork();
    p2 = fork();
    p3 = fork();
    p4 = fork();

    if (p1 == 0) {
      for (int i = 2; i < argc; i += MAX_PROCESSES) {
        resize_photo(argv[i]);
      }
      exit(0);
    }
    if (p2 == 0) {
      for (int i = 3; i < argc; i += MAX_PROCESSES) {
        resize_photo(argv[i]);
      }
      exit(0);
    }
    if (p3 == 0) {
      for (int i = 4; i < argc; i += MAX_PROCESSES) {
        resize_photo(argv[i]);
      }
      exit(0);
    }
    if (p4 == 0) {
      for (int i = 5; i < argc; i += MAX_PROCESSES) {
        resize_photo(argv[i]);
      }
      exit(0);
    }
    exit(0);
  }
  return rc;
}

/* util_get_filenames */
void util_get_filenames(const char *filename, char *thumbnail_fn, char *medium_fn) {
  char copy[strlen(filename) + 1];
  strcpy(copy, filename);
  char *bn = basename(copy); /* filename stripped of path */
  sprintf(thumbnail_fn, "tn_%s", bn);
  sprintf(medium_fn, "med_%s", bn);
}

/* util_process_file */
int util_process_photo(const char *filename, int order) {
  int pid, rc, status; /* setup */
  printf("%s\n", filename);
  char thumbnail_fn[strlen(filename)+4];
  char medium_fn[strlen(filename)+5];
  util_get_filenames(filename, thumbnail_fn, medium_fn);
  /* if it's the first image, must generate thumbnail and medium img */
  if (order == 1) {
    img_resize(filename, medium_fn, "25%");
    pid = img_resize(filename, thumbnail_fn, "10%");
    waitpid(pid, &status, 0); /* must wait for child to create thumbnail */
  }
  int display_pid = img_display(thumbnail_fn);

  /* prompt user for rotation */
  ask_rotation(filename);

  /* prompt user for caption */
  char *caption = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  rc = ask_caption(caption);
  printf("Caption: %s\n", caption);

  /* add photo + caption to html document buffer */
  html_add_photo(filename, caption);

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
  char thumbnail_fn[strlen(filename)+4];  // accomodates for prefix
  char medium_fn[strlen(filename)+5];     // accomodates for prefix
  util_get_filenames(filename, thumbnail_fn, medium_fn);
  char *buffer = malloc((MAX_INPUT_LEN + 1) * sizeof(char));
  while (1) {
    rc = get_input("rotate the image 90 degrees? (cw=clockwise, ccw=counterclockwise, <empty>=stay-same)", buffer, MAX_INPUT_LEN);
    if (-1 == rc) {
      // definitely bad input
      fprintf(stderr, "invalid option for rotating the image...try again\n");
      continue;
    } else if (strlen(buffer) == 0) {
      // no need to rotate thumbnail
      printf("Rotation: None\n");
    } else if (strcmp(buffer, "cw") == 0) {
      img_rotate(medium_fn, medium_fn, CLOCKWISE);
      img_rotate(thumbnail_fn, thumbnail_fn, CLOCKWISE);
      printf("Rotation: Clockwise\n");
    } else if (strcmp(buffer, "cw") == 0) {
    } else if (strcmp(buffer, "ccw") == 0) {
      img_rotate(medium_fn, medium_fn, CCLOCKWISE);
      img_rotate(thumbnail_fn, thumbnail_fn, CCLOCKWISE);
      printf("Rotation: Counter-clockwise\n");
    } else if (strcmp(buffer, "cw") == 0) {
    } else {
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
    // error in get_input
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
