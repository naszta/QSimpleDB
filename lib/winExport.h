#ifndef QSimpleDBLibExportH
#define QSimpleDBLibExportH 1

#ifdef WIN32
# ifdef SimpleDBLib_EXPORTS
#  define sdblibEXPORT __declspec(dllexport)
# else
#  define sdblibEXPORT __declspec(dllimport)
# endif
#else
# define sdblibEXPORT
#endif

#endif
