#include <unistd.h>

#include "util.h"
#include "html.h"

int main(int argc, char *argv[]) {
  util_resize_photos(argc, argv);
  html_init();
  for (int i = 1; i < argc; i++) {
    util_process_photo(argv[i], i);
  }
  html_write_to_file();
  return 0;
}
