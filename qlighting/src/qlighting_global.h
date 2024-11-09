#ifndef QLIGHTING_GLOBAL_H
#define QLIGHTING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLIGHTING_LIBRARY)
#define QLIGHTING_EXPORT Q_DECL_EXPORT
#else
#define QLIGHTING_EXPORT Q_DECL_IMPORT
#endif

#endif // QLIGHTING_GLOBAL_H
