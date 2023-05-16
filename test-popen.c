/** This is free and unencumbered software released into the public domain. **/

#include <fcntl.h>  /* open */
#include <stdio.h>  /* printf, ... */
#include <stdlib.h> /* free */
#include <unistd.h> /* close */


int main()
{
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  /* open pipe */

  /* -multi: multiple files can be selected
   * -print0: filenames are separated by \0 character and not by newline
   */
  FILE *fp = popen("./qtfiledialog -multi -print0", "re");

  if (!fp) {
    perror("popen()");
    return 1;
  }

  /* read from pipe */

  while ((nread = getdelim(&line, &len, '\0', fp)) != -1) {
    /* do something with the filename, i.e. open() and close() */
    int fd = open(line, O_RDONLY|O_CLOEXEC);
    if (fd == -1) {
      printf("failed: %s\n", line);
      perror("open()");
    } else {
      printf("success: %s\n", line);
      close(fd);
    }
  }

  /* free resources and close pipe */

  if (line) free(line);
  int rv = pclose(fp);

  if (rv == -1) {
    perror("pclose()");
    return 1;
  }

  /* return exit status of command */
  return rv;
}
