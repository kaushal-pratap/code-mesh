#include "scanner/tree_scanner.h"
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

Node node_list[1000];
int node_count = 0;

void traverse(const char *path) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    fprintf(stderr, "Error opening directory: %s\n", path);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    char fullpath[PATH_MAX];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

    struct stat st;
    if (lstat(fullpath, &st) == -1) {
      perror(fullpath);
      continue;
    }

    if (entry->d_name[0] == '.') {
      continue;
    }
    if (strstr(entry->d_name, ".dSYM")) {
      continue;
    }

    if (node_count >= 1000) {
      fprintf(stderr, "Warning: Reached max node limit (1000)\n");
      break;
    }

    snprintf(node_list[node_count].name, sizeof(node_list[node_count].name), "%s", entry->d_name);
    snprintf(node_list[node_count].path, sizeof(node_list[node_count].path), "%s", fullpath);
    node_list[node_count].id = node_count;
    node_list[node_count].is_dir = S_ISDIR(st.st_mode);

    node_count++;

    if (S_ISDIR(st.st_mode)) {
      traverse(fullpath);
    }
  }
  closedir(dir);
}
