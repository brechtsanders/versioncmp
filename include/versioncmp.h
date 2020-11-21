/**
 * @file PORTABLECONSOLE.h
 * @brief PORTABLECONSOLE library header file with main functions
 * @author Brecht Sanders
 *
 * This header file defines the functions needed for the versioncmp library
 */

#ifndef INCLUDED_VERSIONCMP_H
#define INCLUDED_VERSIONCMP_H

#include <stdio.h>

/*! \cond PRIVATE */
#if !defined(DLL_EXPORT_VERSIONCMP)
# if defined(_WIN32) && (defined(BUILD_VERSIONCMP_DLL) || defined(VERSIONCMP_EXPORTS))
#  define DLL_EXPORT_VERSIONCMP __declspec(dllexport)
# elif defined(__MINGW32__) && !defined(STATIC) && !defined(BUILD_VERSIONCMP_STATIC) && !defined(BUILD_VERSIONCMP)
#  define DLL_EXPORT_VERSIONCMP __declspec(dllimport)
# else
#  define DLL_EXPORT_VERSIONCMP
# endif
#endif
/*! \endcond */

/*! \brief version number constants
 * \sa     versioncmp_get_version()
 * \sa     versioncmp_get_version_string()
 * \name   VERSIONCMP_VERSION_*
 * \{
 */
/*! \brief major version number */
#define VERSIONCMP_VERSION_MAJOR 1
/*! \brief minor version number */
#define VERSIONCMP_VERSION_MINOR 0
/*! \brief micro version number */
#define VERSIONCMP_VERSION_MICRO 0
/*! @} */

/*! \brief packed version number */
#define VERSIONCMP_VERSION (VERSIONCMP_VERSION_MAJOR * 0x01000000 + VERSIONCMP_VERSION_MINOR * 0x00010000 + VERSIONCMP_VERSION_MICRO * 0x00000100)

/*! \cond PRIVATE */
#define VERSIONCMP_VERSION_STRINGIZE_(major, minor, micro) #major"."#minor"."#micro
#define VERSIONCMP_VERSION_STRINGIZE(major, minor, micro) VERSIONCMP_VERSION_STRINGIZE_(major, minor, micro)
/*! \endcond */

/*! \brief string with dotted version number \hideinitializer */
#define VERSIONCMP_VERSION_STRING VERSIONCMP_VERSION_STRINGIZE(VERSIONCMP_VERSION_MAJOR, VERSIONCMP_VERSION_MINOR, VERSIONCMP_VERSION_MICRO)



#ifdef __cplusplus
extern "C" {
#endif

/*! \brief get versioncmp library version string
 * \param  pmajor        pointer to integer that will receive major version number
 * \param  pminor        pointer to integer that will receive minor version number
 * \param  pmicro        pointer to integer that will receive micro version number
 * \sa     versioncmp_get_version_string()
 */
DLL_EXPORT_VERSIONCMP void versioncmp_get_version (int* pmajor, int* pminor, int* pmicro);

/*! \brief get versioncmp library version string
 * \return version string
 * \sa     versioncmp_get_version()
 */
DLL_EXPORT_VERSIONCMP const char* versioncmp_get_version_string ();

/*! \brief compare version strings
 * \param  s1            first string to compare
 * \param  s2            second string to compare
 * \return negative if s1 < s2, zero s1 = s2, positive if s1 > s2
 */
DLL_EXPORT_VERSIONCMP int versioncmp (const char* s1, const char* s2);

#ifdef __cplusplus
}
#endif

#endif
