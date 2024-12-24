#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "prompt.h"
#include "input.h"
#include "warp.h"
#include "peek.h"
#include "history.h"
#include "seek.h"
#include "bg.h"

#define MAX_HISTORY_SIZE 15
#define HISTORY_FILE "history.txt"
// #define HISTORY_FILE "/history.txt"

#endif
