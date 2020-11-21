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

int versioncmp_base (const char* s1, const char* s2, int factor)
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
    val1 = strtol(p1, (char**)&q1, 10);
    val2 = strtol(p2, (char**)&q2, 10);
    if (q1 != p1 && q2 != p2) {
      //both are numbers
      if (val1 != val2)
        return (val1 < val2 ? -factor : factor);
    } else if (q1 == p1 && q2 == p2) {
      //both are not numbers
      while (*q1 && *q2 && *q1 == *q2 && !isdigit(*q1)) {
        q1++;
        q2++;
      }
      if (*q1 != *q2 && !isdigit(*q1) && !isdigit(*q2)) {
        return (*q1 < *q2 ? -factor : factor);
      }
    } else {
      //one is a number and the other is not
      return factor * strcmp(q1, q2);
    }
    p1 = q1;
    p2 = q2;
  }
  return (*p1 == *p2 ? 0 : (*p2 ? -factor : factor));
}

int versioncasecmp_base (const char* s1, const char* s2, int factor)
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
    val1 = strtol(p1, (char**)&q1, 10);
    val2 = strtol(p2, (char**)&q2, 10);
    if (q1 != p1 && q2 != p2) {
      //both are numbers
      if (val1 != val2)
        return (val1 < val2 ? -factor : factor);
    } else if (q1 == p1 && q2 == p2) {
      //both are not numbers
      while (*q1 && *q2 && tolower(*q1) == tolower(*q2) && !isdigit(*q1)) {
        q1++;
        q2++;
      }
      if (*q1 != *q2 && !isdigit(*q1) && !isdigit(*q2)) {
        //return (*q1 < *q2 ? -factor : factor);
        return factor * strcasecmp(q1, q2);
      }
    } else {
      //one is a number and the other is not
      return factor * strcasecmp(q1, q2);
    }
    p1 = q1;
    p2 = q2;
  }
  return (*p1 == *p2 ? 0 : (*p2 ? -factor : factor));
}

DLL_EXPORT_VERSIONCMP int versioncmp (const char* s1, const char* s2)
{
  return versioncmp_base(s1, s2, 1);
}

DLL_EXPORT_VERSIONCMP int versioncmpreverse (const char* s1, const char* s2)
{
  return versioncmp_base(s1, s2, -1);
}

DLL_EXPORT_VERSIONCMP int versioncasecmp (const char* s1, const char* s2)
{
  return versioncasecmp_base(s1, s2, 1);
}

DLL_EXPORT_VERSIONCMP int versioncasecmpreverse (const char* s1, const char* s2)
{
  return versioncasecmp_base(s1, s2, -1);
}
