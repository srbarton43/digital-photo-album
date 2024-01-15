#ifndef __HTML_H
#define __HTML_H

/* 
 * html.h - module for constructing html photo album
 * 
 * SRB -- 24W
 */

/*
 * html_init - initializes static variables for html doc
 */
void html_init(void);

/*
 * html_add_photo - adds photo, hyperlink, and caption
 *
 * filename := path to image
 * caption  := user's caption
 */
void html_add_photo(const char *filename, const char *caption);

/*
 * html_write_to_file - write html file to disk
 *
 * notes:
 *  - filename is 'index.html'
 */
void html_write_to_file(void);

#endif
