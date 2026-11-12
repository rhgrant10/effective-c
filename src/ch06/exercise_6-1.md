Original code:
```c
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int closedir(DIR *dirp) {
  free(dirp->d_buf);
  free(dirp);
  return close(dirp->d_fd);  // dirp has already been freed
}
```

Fixed code:
```c
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int closedir(DIR *dirp) {
  int retval = close(dirp->d_fd);  // close and save result first
  free(dirp->d_buf);
  free(dirp);  // then free last
  return retval;
}
```
