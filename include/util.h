#ifndef __UTIL_H
#define __UTIL_H

/*
 * util_resize_photos - resizes all photos besides the first
 *
 * argc := argument count
 * argv := list of arguments
 *
 * notes:
 *  - uses 4 processes to complete all resizing commands
 *  - uses a sleep at the start of child process because
 *    creating 4 processes lags the parent process
 *
 * returns:
 *  - pid of child process
 */
int util_resize_photos(const int argc, char** argv);

/*
 * util_process_photo - prompts user for caption and rotation, displaying thumbnail
 *
 * filename := path to photo
 * order    := which photo (only the first one gets special treatment)
 * notes:
 *  - no resizing done, just rotations (except for first image)
 *
 * returns:
 *  - result of killing display process  
 */
int util_process_photo(const char *filename, int order);

/*
 * util_get_filenames - gets the thumbnail and medium filenames
 *
 * filename     := path to original image
 * thumbnail_fn := thumbnail fn, alloc'ed on stack with size strlen(filename)+4
 * medium_fn    := medium fn, alloc'ed on stack with size strlen(filename)+5
 */
void util_get_filenames(const char *filename, char *thumbnail_fn, char *medium_fn);
#endif

