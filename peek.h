
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>


#ifndef __PEEK_H
#define __PEEK_H
#include <stdio.h>

void peek(char *home, char *prev_dir, int home_size, char *extract);
#endif