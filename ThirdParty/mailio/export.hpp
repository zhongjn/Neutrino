
#ifndef MAILIO_EXPORT_H
#define MAILIO_EXPORT_H

#ifdef MAILIO_STATIC_DEFINE
#  define MAILIO_EXPORT
#  define MAILIO_NO_EXPORT
#else
#  ifndef MAILIO_EXPORT
#    ifdef mailio_EXPORTS
        /* We are building this library */
#      define MAILIO_EXPORT 
#    else
        /* We are using this library */
#      define MAILIO_EXPORT 
#    endif
#  endif

#  ifndef MAILIO_NO_EXPORT
#    define MAILIO_NO_EXPORT 
#  endif
#endif

#ifndef MAILIO_DEPRECATED
#  define MAILIO_DEPRECATED __declspec(deprecated)
#endif

#ifndef MAILIO_DEPRECATED_EXPORT
#  define MAILIO_DEPRECATED_EXPORT MAILIO_EXPORT MAILIO_DEPRECATED
#endif

#ifndef MAILIO_DEPRECATED_NO_EXPORT
#  define MAILIO_DEPRECATED_NO_EXPORT MAILIO_NO_EXPORT MAILIO_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MAILIO_NO_DEPRECATED
#    define MAILIO_NO_DEPRECATED
#  endif
#endif

#endif /* MAILIO_EXPORT_H */
