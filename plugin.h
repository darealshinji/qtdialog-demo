/** This is free and unencumbered software released into the public domain. **/

/***
Usage:

qt_file_list lst = qt_file_select(NULL, argc, argv);

if (lst.list) {
  for (size_t i = 0; i < lst.size; i++) {
    printf("%s\n", lst.list[i]);
    free(lst.list[i]);
  }
  free(lst.list);
}
***/

#ifndef QT_FILE_SELECT_H
#define QT_FILE_SELECT_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#if defined QT_PLUGIN && !defined API_PUBLIC && defined __GNUC__
#define API_PUBLIC __attribute__ ((visibility ("default")))
#endif

#if !defined QT_PLUGIN && defined API_PUBLIC
#undef API_PUBLIC
#endif

#ifndef API_PUBLIC
#define API_PUBLIC /**/
#endif

typedef struct {
  const char *path;   /* initial path */
  const char *title;  /* window title */
  const char *filter; /* filename filter list */
  bool bnative;       /* native dialog */
  bool bdir;          /* directory selection */
  bool bsave;         /* save mode */
  bool blist;         /* list view */
  bool bmulti;        /* multiple files selection */
} qt_file_cfg;

typedef struct {
  size_t size;
  char **list;
} qt_file_list;

/* set cfg to NULL for default settings */
API_PUBLIC qt_file_list qt_file_select(qt_file_cfg *cfg, int argc, char **argv);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* QT_FILE_SELECT_H */
