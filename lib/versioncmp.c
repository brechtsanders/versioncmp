#include "versioncmp.h"

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

