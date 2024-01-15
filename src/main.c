#include "util.h"
#include "html.h"

int main(int argc, char *argv[]) {
  int rc, pid, status;
  html_init();
  for (int i = 1; i < argc; i++) {
    process_file(argv[i]);
  }
  html_write_to_file();
  return 0;
}
