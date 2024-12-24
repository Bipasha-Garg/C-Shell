
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h> 

#ifndef __WARP_H
#define __WARP_H

void warp_ok(char home_directory[500], char prev_dir[500], int home_size, char *extract);


#endif