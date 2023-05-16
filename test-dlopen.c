/** This is free and unencumbered software released into the public domain. **/

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "plugin.h"


int main(int argc, char **argv)
{
  /* typedef */
  typedef qt_file_list (*qt_file_select_t)(qt_file_cfg *cfg, int argc, char **argv);
  qt_file_select_t qt_file_select_ptr;

  /* load library */
  const char *lib1 = "libqtfiledialog.so";
  const char *lib2 = "qtfiledialog.so";
  int flags = RTLD_NOW | RTLD_LOCAL;

  void *handle = dlopen(lib1, flags);
  if (!handle) handle = dlopen(lib2, flags);

  if (!handle) {
    fprintf(stderr, "failed to load plugin `%s' or `%s'\n", lib1, lib2);
    return 1;
  }

  /* load symbol */
  qt_file_select_ptr = (qt_file_select_t)dlsym(handle, "qt_file_select");
  char *err = dlerror();

  if (err) {
    fprintf(stderr, "error: %s\n", err);
    dlclose(handle);
    return 1;
  }

  /* use function */
  qt_file_list lst = qt_file_select_ptr(NULL, argc, argv);

  if (lst.list) {
    for (size_t i = 0; i < lst.size; i++) {
      printf("%s\n", lst.list[i]);
      free(lst.list[i]);
    }
    free(lst.list);
  }

  dlclose(handle);

  return 0;
}
