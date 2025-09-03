#ifndef QLIGHTING_GLOBAL_H
#define QLIGHTING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLIGHTING_LIBRARY)
#define QLIGHTING_EXPORT Q_DECL_EXPORT
#else
#define QLIGHTING_EXPORT Q_DECL_IMPORT
#endif

#if 1

#include <QQmlEngine>

#define QLIGHTING_SINGLETON \
    QML_ELEMENT \
    QML_SINGLETON

#define QLIGHTING_ELEMENT QML_ELEMENT

#endif

#endif // QLIGHTING_GLOBAL_H
