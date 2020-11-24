#include "versioncmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//example data
char* versionlist[] = {
  "libNothing 10.1",
  "libnothing 10.0",
  "libsomething 1.0",
  "libsomething 0.1",
  "libsomething 20.1",
  "libsomething 2.0",
  "libsomething 10.2",
  "libnothing 2.0",
  "2.1.0",
  "1.0.0",
  "20.2.0",
  "10.0.0",
  "1.2beta",
  "1.2.src",
  "1.2.1",
  "1.1.src",
  "0.1.src",
  "2_6_2",
  "2_6_1",
  "0-6-1",
  "0-6-2"
};
#define versionlistsize (sizeof(versionlist) / sizeof(*versionlist))

//definition of compare function
typedef int (*cmp_fn)(const char*, const char*);

//list with compare functions
struct cmplist_struct {
  const char* name;
  cmp_fn comparefunction;
} cmplist[] = {
  {"strcmp", strcmp},
  {"strcasecmp", strcasecmp},
  {"versioncmp", versioncmp},
  {"versioncmpreverse", versioncmpreverse},
  {"versioncasecmp", versioncasecmp},
  {"versioncasecmpreverse", versioncasecmpreverse}
};
#define cmplistsize (sizeof(cmplist) / sizeof(*cmplist))

//global variable holding current compare function
cmp_fn comparefunction;

//compare function for qsort doing the required type case and dereferencing
static int cmpstringp (const void *p1, const void *p2)
{
  return (*comparefunction)(*(char* const*)p1, *(char* const*)p2);
}

int main()
{
  int i;
  int j;

  //show version
  printf("versioncmp v%s\n", versioncmp_get_version_string());

  //show list before sorting
  printf("Before sorting:\n");
  for (j = 0; j < versionlistsize; j++) {
    printf("  %s\n", versionlist[j]);
  }

  //perform various sorts
  for (i = 0; i < cmplistsize; i++) {
    printf("Sort with %s:\n", cmplist[i].name);

    //sort
    comparefunction = cmplist[i].comparefunction;
    qsort(versionlist, versionlistsize, sizeof(*versionlist), cmpstringp);

    //show list after sorting
    for (j = 0; j < versionlistsize; j++) {
      printf("  %s\n", versionlist[j]);
    }
  }

  return 0;
}
