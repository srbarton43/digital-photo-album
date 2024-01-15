#include "html.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static unsigned int HTML_MAX_SIZE;  // current max size of buffer
static char *html_string;           // html string buffer - alloc'ed on heap

void html_init(void) {
  HTML_MAX_SIZE = 4096;  // 4kb to start
  html_string = malloc((HTML_MAX_SIZE + 1) * sizeof(char));
  char *intro =
      "<html><title>Photo Album</title>\n"
      "<h1>Photo Album</h1>\n"
      "<body>\n";
  strcpy(html_string, intro);
}

void html_add_photo(const char *filename, const char *caption) {
  /* get filenames */
  char thumbnail_fn[strlen(filename) + 4]; /* thumbnail filename */
  char medium_fn[strlen(filename) + 5];
  util_get_filenames(filename, thumbnail_fn, medium_fn);

  /* construct html parts */
  char header[strlen(caption) + 10];  // size of caption plus html elems
  char *header_restrict =
      "<h2>%s</h2>\n";
  sprintf(header, header_restrict, caption);
  char new_element[strlen(header) + strlen(thumbnail_fn) + strlen(medium_fn) + 48];  // plus size of restrict
  char *img_restrict =
      "%s"
      "<a href=\"%s\">\n"
      "<img src=\"%s\" border=\"1\">\n"
      "</a>\n";
  sprintf(new_element, img_restrict, header, medium_fn,
          thumbnail_fn);

  /* check size doesn't exceed buffer */
  if (strlen(html_string) + strlen(new_element) >= HTML_MAX_SIZE) {
    html_string = realloc(html_string, HTML_MAX_SIZE * 2 + 1);
    HTML_MAX_SIZE *= 2;
  }

  /* add to main html */
  strncat(html_string, new_element, HTML_MAX_SIZE);
}

void html_write_to_file(void) {
  char *ending =
      "</body>\n"
      "</html>";

  /* check size doesn't exceed buffer */
  if (strlen(html_string) + strlen(ending) >= HTML_MAX_SIZE) {
     html_string = realloc(html_string, HTML_MAX_SIZE + strlen(ending) + 1);
    HTML_MAX_SIZE += strlen(ending);
  }
  strncat(html_string, ending, HTML_MAX_SIZE);
  FILE *fp = fopen("index.html", "w+");
  if (fp) {
    fwrite(html_string, sizeof(char), strlen(html_string), fp);
  } else {
    fprintf(stderr, "Error creating html file\n");
  }
  free(html_string);
}
