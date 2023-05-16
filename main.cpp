/** This is free and unencumbered software released into the public domain. **/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "plugin.h"

/***
  Quote filenames in a shell compatible way. Works on Bash.
  Escaping other characters than LF or apostroph isn't required.

  example:
  f=$(./qtfiledialog -shell); eval "test -f $f && echo yes || echo no"

  this should return "yes" even on filenames with newline characters
***/
static void shell_quote(qt_file_list list, bool print0)
{
  for (size_t i = 0; i < list.size; i++) {
    char prev = 0;

    for (char *p = list.list[i]; *p != 0; p++) {
      switch (*p) {
        case '\'':
          if (prev != 0 && prev != '\'') {
            std::cout << '\'';
          }
          std::cout << "\\'";
          break;

        case '\n':
          switch (prev) {
            case '\n':
              std::cout << "\\n";
              break;
            case '\'':
            case 0:
              std::cout << "$'\\n";
              break;
            default:
              std::cout << "'$'\\n";
              break;
          }
          break;

        default:
          if (prev == '\n') {
            std::cout << "''";
          } else if (prev == 0 || prev == '\'') {
            std::cout << '\'';
          }
          std::cout << *p;
          break;
      }

      prev = *p;
    }

    if (prev != '\'') {
      std::cout << '\'';
    }

    if (print0) {
      std::cout << '\0' << std::flush;
    } else {
      std::cout << std::endl;
    }

    free(list.list[i]);
  }

  free(list.list);
}

int main(int argc, char **argv)
{
  const char *help =
    " -help | --help   show this help\n"
    " -directory       select a directory\n"
    " -filter=LIST     comma-separated list of file extensions to filter in;\n"
    "                  example 1: \"*.png,*.jpg *.jpeg,*.gif\"\n"
    "                  example 2: \"MPEG Layer 3 (*.mp3),PCM Wave (*.wav *.wave)\"\n"
    " -list            set \"list\" view mode\n"
    " -multi           allow selection of multiple files\n"
    " -no-native       force the original Qt file selector to be used\n"
    " -path=PATH       set the initial path\n"
    " -print0          use nul character as delimiter\n"
    " -save            use \"save\" mode\n"
    " -shell           quote filenames in a shell compatible way;\n"
    "                  may not work on all shells\n"
    " -title=TEXT      set window title\n";

  qt_file_cfg cfg;
  cfg.path = NULL;
  cfg.title = NULL;
  cfg.filter = NULL;
  cfg.bnative = true;
  cfg.bdir = false;
  cfg.bsave = false;
  cfg.blist = false;
  cfg.bmulti = false;

  bool bprint0 = false;
  bool bshell = false;

  if (argc > 1) {
    for (int i=1; i < argc; i++) {
      if (*argv[i] != '-') {
        std::cerr << "error: unknown argument: " << argv[i] << std::endl;
        return 1;
      }

      int n = 1;
      const char *arg = argv[i] + 1;

#define ARG(x) \
  (strcmp(arg, x) == 0)

#define ARGX(x) \
  (strncmp(arg, x "=", sizeof(x)) == 0 && \
   strlen(arg) > (n = sizeof(x)))

      if (ARG("help") || ARG("-help")) {
        std::cout << help << std::flush;
        return 0;
      } else if (ARG("no-native")) {
        cfg.bnative = false;
      } else if (ARG("directory")) {
        cfg.bdir = true;
      } else if (ARG("save")) {
        cfg.bsave = true;
      } else if (ARG("list")) {
        cfg.blist = true;
      } else if (ARG("print0")) {
        bprint0 = true;
      } else if (ARG("shell")) {
        bshell = true;
      } else if (ARG("multi")) {
        cfg.bmulti = true;
      } else if (ARGX("title")) {
        cfg.title = arg + n;
      } else if (ARGX("path")) {
        cfg.path = arg + n;
      } else if (ARGX("filter")) {
        cfg.filter = arg + n;
      } else {
        std::cerr << "error: unknown argument: " << argv[i] << std::endl;
        return 1;
      }
    }
  }

  if (!cfg.title) {
    if (cfg.bdir) {
      cfg.title = "Select directory";
    } else if (cfg.bmulti) {
      cfg.title = "Select file(s)";
    } else {
      cfg.title = "Select file";
    }
  }

  qt_file_list list = qt_file_select(&cfg, argc, argv);
  if (list.size == 0) return 1;

  if (bshell) {
    shell_quote(list, bprint0);  /* automatically frees content */
  } else {
    for (size_t i = 0; i < list.size; i++) {
      std::cout << list.list[i];

      if (bprint0) {
        std::cout << '\0' << std::flush;
      } else {
        std::cout << std::endl;
      }

      free(list.list[i]);
    }

    free(list.list);
  }

  return 0;
}
