#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"

#define HTML_MAX_SIZE 16384 // 16kb
static char *html_string;

void html_init(void) {
  html_string = malloc((HTML_MAX_SIZE + 1) * sizeof(char)); // allocate 16kb
  char *intro = "<html><title>Photo Album</title>\n"
                "<h1>Photo Album</h1>\n"
                "<body>\n";
  strcpy(html_string, intro);
}

void html_add_photo(const char *bn, const char *caption) {
  /* get filenames */
  char thumbnail_fn[strlen(bn) + 4]; /* thumbnail filename */
  sprintf(thumbnail_fn, "tn_%s", bn);
  char medium_fn[strlen(bn) + 5];
  sprintf(medium_fn, "med_%s", bn);

  /* construct html parts */
  char header[strlen(caption) + 10]; // size of caption plus html elems
  sprintf(header, "<h2>%s</h2>\n", caption);
  char image[strlen(header) + (2*strlen(bn) + 7) + 48]; // plus size of restrict
  char *img_restrict = 
    "%s"
    "<a href=\"%s\">\n"
    "<img src=\"%s\" border=\"1\">\n"
    "</a>\n";
  sprintf(image, img_restrict, header, medium_fn,
          thumbnail_fn);

  /* add to main html */
  //strncat(html_string, header, HTML_MAX_SIZE);
  //strncat(html_string, image, HTML_MAX_SIZE);
  strncat(html_string, image, HTML_MAX_SIZE);
}

void html_write_to_file(void) {
  char *ending = "</body>\n"
                 "</html>";
  strncat(html_string, ending, HTML_MAX_SIZE);
  FILE *fp = fopen("index.html", "w+");
  if (fp) {
    fwrite(html_string, sizeof(char), strlen(html_string), fp);
  } else {
    fprintf(stderr, "Error creating html file\n");
  }
  free(html_string);
}
