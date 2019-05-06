#ifndef GENERIC_SQL_EXPORT_H
#define GENERIC_SQL_EXPORT_H

#ifdef _WIN32
#if defined GENERIC_SQL_DLL
#define GENERIC_SQL_EXPORT __declspec(dllexport)
#else
#define GENERIC_SQL_EXPORT __declspec(dllimport)
#endif
#else
#define GENERIC_SQL_EXPORT
#endif

#endif
