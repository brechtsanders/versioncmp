#include "versioncmp.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

DLL_EXPORT_VERSIONCMP void versioncmp_get_version (int* pmajor, int* pminor, int* pmicro)
{
  if (pmajor)
    *pmajor = VERSIONCMP_VERSION_MAJOR;
  if (pminor)
    *pminor = VERSIONCMP_VERSION_MINOR;
  if (pmicro)
    *pmicro = VERSIONCMP_VERSION_MICRO;
}

DLL_EXPORT_VERSIONCMP const char* versioncmp_get_version_string ()
{
  return VERSIONCMP_VERSION_STRING;
}

static inline int versioncmp_base (const char* s1, const char* s2, int factor, int (*cmp_fn)(const char*, const char*), int (*cmpn_fn)(const char*, const char*, size_t))
{
  long val1;
  long val2;
  const char* p1;
  const char* p2;
  const char* q1;
  const char* q2;
  p1 = s1;
  p2 = s2;
  while (*p1 && *p2) {
    //skip hyphen so it wouldn't be treated as a minus sign
    if (*p1 == '-' && *p2 == '-') {
      p1++;
      p2++;
    }
    //dot is always larger than something else
    if (*p1 == '.' && *p2 != '.') {
      return factor;
    } else if (*p2 == '.' && *p1 != '.') {
      return -factor;
    }
    //detect numbers
    val1 = strtol(p1, (char**)&q1, 10);
    val2 = strtol(p2, (char**)&q2, 10);
    //first one is a number
    if (q1 != p1) {
      if (q2 != p2) {
        //both are numbers
        if (val1 != val2)
          return (val1 < val2 ? -factor : factor);
      } else {
        //the first one is a number, the second is not
        return factor;
      }
    } else {
      if (q2 == p2) {
        //both are not numbers
        //while (*q1 && *q2 && *q1 == *q2 && !isdigit(*q1)) {
        //while (*q1 && *q2 && tolower(*q1) == tolower(*q2) && !isdigit(*q1)) {
        while (*q1 && *q2 && (*cmpn_fn)(q1, q2, 1) == 0 && !isdigit(*q1)) {
          q1++;
          q2++;
        }
        if (*q1 != *q2 && !isdigit(*q1) && !isdigit(*q2)) {
          return factor * (*cmp_fn)(q1, q2);
          //return (*q1 < *q2 ? -factor : factor);
        }
      } else if (q2 != p2) {
        //the second one is a number, the first one is not
        return -factor;
      }
    }
    p1 = q1;
    p2 = q2;
  }
  return (*p1 == *p2 ? 0 : (*p2 ? -factor : factor));
}

#if 0
static inline int versioncasecmp_base (const char* s1, const char* s2, int factor, int (*cmp_fn)(const char*, const char*), int (*cmpn_fn)(const char*, const char*, size_t))
{
  long val1;
  long val2;
  const char* p1;
  const char* p2;
  const char* q1;
  const char* q2;
  p1 = s1;
  p2 = s2;
  while (*p1 && *p2) {
    //skip hyphen so it wouldn't be treated as a minus sign
    if (*p1 == '-' && *p2 == '-') {
      p1++;
      p2++;
    }
    //detect numbers
    val1 = strtol(p1, (char**)&q1, 10);
    val2 = strtol(p2, (char**)&q2, 10);
    if (q1 != p1 && q2 != p2) {
      //both are numbers
      if (val1 != val2)
        return (val1 < val2 ? -factor : factor);
    } else if (q1 == p1 && q2 == p2) {
      //both are not numbers
      //while (*q1 && *q2 && tolower(*q1) == tolower(*q2) && !isdigit(*q1)) {
      while (*q1 && *q2 && (*cmpn_fn)(q1, q2, 1) == 0 && !isdigit(*q1)) {
        q1++;
        q2++;
      }
      if (*q1 != *q2 && !isdigit(*q1) && !isdigit(*q2)) {
        return factor * (*cmp_fn)(q1, q2);
      }
    } else if (q1 != p1) {
      //the first one is a number
      return factor;
    } else if (q2 != p2) {
      //the second one is a number
      return -factor;
/*
    } else {
      //one is a number and the other is not
      return factor * (*cmp_fn)(q1, q2);
*/
    }
    p1 = q1;
    p2 = q2;
  }
  return (*p1 == *p2 ? 0 : (*p2 ? -factor : factor));
}
#endif

DLL_EXPORT_VERSIONCMP int versioncmp (const char* s1, const char* s2)
{
  return versioncmp_base(s1, s2, 1, strcmp, strncmp);
}

DLL_EXPORT_VERSIONCMP int versioncmpreverse (const char* s1, const char* s2)
{
  return versioncmp_base(s1, s2, -1, strcmp, strncmp);
}

DLL_EXPORT_VERSIONCMP int versioncasecmp (const char* s1, const char* s2)
{
  //return versioncasecmp_base(s1, s2, 1, strcasecmp);
  return versioncmp_base(s1, s2, 1, strcasecmp, strncasecmp);
}

DLL_EXPORT_VERSIONCMP int versioncasecmpreverse (const char* s1, const char* s2)
{
  //return versioncasecmp_base(s1, s2, -1, strcasecmp);
  return versioncmp_base(s1, s2, -1, strcasecmp, strncasecmp);
}
