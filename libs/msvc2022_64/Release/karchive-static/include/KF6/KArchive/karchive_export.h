
#ifndef KARCHIVE_EXPORT_H
#define KARCHIVE_EXPORT_H

#ifdef KARCHIVE_STATIC_DEFINE
#  define KARCHIVE_EXPORT
#  define KARCHIVE_NO_EXPORT
#else
#  ifndef KARCHIVE_EXPORT
#    ifdef KF6Archive_EXPORTS
        /* We are building this library */
#      define KARCHIVE_EXPORT 
#    else
        /* We are using this library */
#      define KARCHIVE_EXPORT 
#    endif
#  endif

#  ifndef KARCHIVE_NO_EXPORT
#    define KARCHIVE_NO_EXPORT 
#  endif
#endif

#ifndef KARCHIVE_DECL_DEPRECATED
#  define KARCHIVE_DECL_DEPRECATED __declspec(deprecated)
#endif

#ifndef KARCHIVE_DECL_DEPRECATED_EXPORT
#  define KARCHIVE_DECL_DEPRECATED_EXPORT KARCHIVE_EXPORT KARCHIVE_DECL_DEPRECATED
#endif

#ifndef KARCHIVE_DECL_DEPRECATED_NO_EXPORT
#  define KARCHIVE_DECL_DEPRECATED_NO_EXPORT KARCHIVE_NO_EXPORT KARCHIVE_DECL_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef KARCHIVE_NO_DEPRECATED
#    define KARCHIVE_NO_DEPRECATED
#  endif
#endif
#include <karchive_version.h>

#define KARCHIVE_DECL_DEPRECATED_TEXT(text) __declspec(deprecated(text))

/* Take any defaults from group settings */
#if !defined(KARCHIVE_NO_DEPRECATED) && !defined(KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT)
#  ifdef KF_NO_DEPRECATED
#    define KARCHIVE_NO_DEPRECATED
#  elif defined(KF_DISABLE_DEPRECATED_BEFORE_AND_AT)
#    define KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT KF_DISABLE_DEPRECATED_BEFORE_AND_AT
#  endif
#endif
#if !defined(KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT) && defined(KF_DISABLE_DEPRECATED_BEFORE_AND_AT)
#  define KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT KF_DISABLE_DEPRECATED_BEFORE_AND_AT
#endif

#if !defined(KARCHIVE_NO_DEPRECATED_WARNINGS) && !defined(KARCHIVE_DEPRECATED_WARNINGS_SINCE)
#  ifdef KF_NO_DEPRECATED_WARNINGS
#    define KARCHIVE_NO_DEPRECATED_WARNINGS
#  elif defined(KF_DEPRECATED_WARNINGS_SINCE)
#    define KARCHIVE_DEPRECATED_WARNINGS_SINCE KF_DEPRECATED_WARNINGS_SINCE
#  endif
#endif
#if !defined(KARCHIVE_DEPRECATED_WARNINGS_SINCE) && defined(KF_DEPRECATED_WARNINGS_SINCE)
#  define KARCHIVE_DEPRECATED_WARNINGS_SINCE KF_DEPRECATED_WARNINGS_SINCE
#endif

#if defined(KARCHIVE_NO_DEPRECATED)
#  undef KARCHIVE_DEPRECATED
#  define KARCHIVE_DEPRECATED_EXPORT KARCHIVE_EXPORT
#  define KARCHIVE_DEPRECATED_NO_EXPORT KARCHIVE_NO_EXPORT
#elif defined(KARCHIVE_NO_DEPRECATED_WARNINGS)
#  define KARCHIVE_DEPRECATED
#  define KARCHIVE_DEPRECATED_EXPORT KARCHIVE_EXPORT
#  define KARCHIVE_DEPRECATED_NO_EXPORT KARCHIVE_NO_EXPORT
#else
#  define KARCHIVE_DEPRECATED KARCHIVE_DECL_DEPRECATED
#  define KARCHIVE_DEPRECATED_EXPORT KARCHIVE_DECL_DEPRECATED_EXPORT
#  define KARCHIVE_DEPRECATED_NO_EXPORT KARCHIVE_DECL_DEPRECATED_NO_EXPORT
#endif

/* No deprecated API had been removed from build */
#define KARCHIVE_EXCLUDE_DEPRECATED_BEFORE_AND_AT 0

#define KARCHIVE_BUILD_DEPRECATED_SINCE(major, minor) 1

#ifdef KARCHIVE_NO_DEPRECATED
#  define KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT KARCHIVE_VERSION
#endif
#ifdef KARCHIVE_NO_DEPRECATED_WARNINGS
#  define KARCHIVE_DEPRECATED_WARNINGS_SINCE 0
#endif

#ifndef KARCHIVE_DEPRECATED_WARNINGS_SINCE
#  ifdef KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT
#    define KARCHIVE_DEPRECATED_WARNINGS_SINCE KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT
#  else
#    define KARCHIVE_DEPRECATED_WARNINGS_SINCE KARCHIVE_VERSION
#  endif
#endif

#ifndef KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT
#  define KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT 0
#endif

#ifdef KARCHIVE_DEPRECATED
#  define KARCHIVE_ENABLE_DEPRECATED_SINCE(major, minor) (((major<<16)|(minor<<8)) > KARCHIVE_DISABLE_DEPRECATED_BEFORE_AND_AT)
#else
#  define KARCHIVE_ENABLE_DEPRECATED_SINCE(major, minor) 0
#endif

#if KARCHIVE_DEPRECATED_WARNINGS_SINCE >= 0x60d00
#  define KARCHIVE_DEPRECATED_VERSION_6_13(text) KARCHIVE_DECL_DEPRECATED_TEXT(text)
#else
#  define KARCHIVE_DEPRECATED_VERSION_6_13(text)
#endif
#define KARCHIVE_DEPRECATED_VERSION_6(minor, text)      KARCHIVE_DEPRECATED_VERSION_6_##minor(text)
#define KARCHIVE_DEPRECATED_VERSION(major, minor, text) KARCHIVE_DEPRECATED_VERSION_##major(minor, "Since "#major"."#minor". " text)
#define KARCHIVE_DEPRECATED_VERSION_BELATED(major, minor, textmajor, textminor, text) KARCHIVE_DEPRECATED_VERSION_##major(minor, "Since "#textmajor"."#textminor". " text)
// Not yet implemented for MSVC
#define KARCHIVE_ENUMERATOR_DEPRECATED_VERSION(major, minor, text)
#define KARCHIVE_ENUMERATOR_DEPRECATED_VERSION_BELATED(major, minor, textmajor, textminor, text)

#endif /* KARCHIVE_EXPORT_H */
