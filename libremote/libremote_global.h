#ifndef LIBREMOTE_GLOBAL_H
#define LIBREMOTE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBREMOTE_LIBRARY)
#  define LIBREMOTE_EXPORT Q_DECL_EXPORT
#else
#  define LIBREMOTE_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBREMOTE_GLOBAL_H
