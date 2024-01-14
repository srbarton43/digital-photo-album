#ifndef __HTML_H
#define __HTML_H

/* library for creating index.html file */

void html_init(void);

void html_add_photo(const char *bn, const char *caption);

void html_write_to_file(void);

#endif
