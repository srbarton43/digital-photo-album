#ifndef __UTIL_H
#define __UTIL_H
/*
 * utils.h - utilities module
 *
 * SRB -- 24W
 *
 * This module provides utility functions for the main program and for other
 * modules
 */

/* macros for pipes */
#define RPIPE 0
#define WPIPE 1

/*
 * util_resize_photos - resizes all photos
 *
 * argc := argument count
 * argv := list of arguments
 * sv1  := pipe for sending termination
 *
 * notes:
 *    each time the child finishes resizing images, it sends the image_index to
 * the pipe
 *
 * returns:
 *  - pid of child process
 */
int util_resize_photos(const int argc, char **argv, int *sv1);

/*
 * util_process_photo - prompts user for caption and rotation, displaying
 * thumbnail
 *
 * filename := path to photo
 * order    := which photo (only the first one gets special treatment)
 * notes:
 *  - no resizing done, just rotations (except for first image)
 *
 * returns:
 *  - result of killing display process
 */
int util_process_photo(const char *filename);

/*
 * util_get_filenames - gets the thumbnail and medium filenames
 *
 * filename     := path to original image
 * thumbnail_fn := thumbnail fn, alloc'ed on stack with size strlen(filename)+4
 * medium_fn    := medium fn, alloc'ed on stack with size strlen(filename)+5
 */
void util_get_filenames(const char *filename, char *thumbnail_fn,
                        char *medium_fn);
#endif
