/*
 * html.c - html module implementation
 *
 * static variables:
 *  - HTML_MAX_SIZE := current size of buffer on heap
 *  - html_string   := heal-alloc'd buffer to store html contents
 *
 *  see html.h for more details
 *
 * SRB -- 24W
 */
#include "html.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static unsigned int HTML_MAX_SIZE; // current max size of buffer
static char *html_string;          // html string buffer - alloc'ed on heap

/* html_init */
void html_init(void) {
  HTML_MAX_SIZE = 4096; // 4kb to start (probably overkill)
  html_string = malloc((HTML_MAX_SIZE + 1) * sizeof(char));
  char *intro = /* the html preamble */
      "<html><title>Photo Album</title>\n"
      "<h1>Photo Album</h1>\n"
      "<body>\n";
  strncpy(html_string, intro, HTML_MAX_SIZE);
}

/* html_add_photo */
void html_add_photo(const char *filename, const char *caption) {
  /* get filenames */
  char thumbnail_fn[strlen(filename) + 4]; /* thumbnail filename */
  char medium_fn[strlen(filename) + 5];
  util_get_filenames(filename, thumbnail_fn, medium_fn);

  /* construct html parts */
  char header[strlen(caption) + 11]; /* size of caption plus html elems */
  char *header_restrict = "<h2>%s</h2>\n";
  sprintf(header, header_restrict, caption); /* add caption as <h2> element */
  char new_element[strlen(header) + strlen(thumbnail_fn) + strlen(medium_fn) +
                   48]; // plus size of restrict
  char *img_restrict = "%s"
                       "<a href=\"%s\">\n"
                       "<img src=\"%s\" border=\"1\">\n"
                       "</a>\n";
  sprintf(new_element, img_restrict, header,
          medium_fn, /* add caption + image hyperlink */
          thumbnail_fn);

  /* check size doesn't exceed buffer */
  if (strlen(html_string) + strlen(new_element) >= HTML_MAX_SIZE) {
    html_string =
        realloc(html_string,
                HTML_MAX_SIZE * 2 + 1); /* if it does, double the buffer size */
    HTML_MAX_SIZE *= 2;
  }

  /* add to main html */
  strncat(html_string, new_element, HTML_MAX_SIZE);
}

/* html_write_to_file */
void html_write_to_file(void) {
  char *ending = "</body>\n"
                 "</html>";

  /* check size doesn't exceed buffer */
  if (strlen(html_string) + strlen(ending) >=
      HTML_MAX_SIZE) { /* if it does, add enough space for html closures */
    html_string = realloc(html_string, HTML_MAX_SIZE + strlen(ending) + 1);
    HTML_MAX_SIZE += strlen(ending);
  }
  strncat(html_string, ending, HTML_MAX_SIZE);
  FILE *fp = fopen("index.html", "w+");
  if (fp) {
    fwrite(html_string, sizeof(char), strlen(html_string),
           fp); /* write buffer to file */
  } else {
    fprintf(stderr, "Error creating html file\n"); /* error creating file */
  }
  free(html_string); /* free the buffer */
}
