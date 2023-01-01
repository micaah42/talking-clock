QT += quick quickcontrols2 multimedia websockets
# CONFIG += qml_debug

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/alarm.h \
    src/alarmservice.h \
    src/application_defines.h \
    src/clockserver.h \
    src/constants.h \
    src/fontservice.h \
    src/jsonspecification.h \
    src/model.h \
    src/palette.h \
    src/pathservice.h \
    src/payload.h \
    src/loghandling.h \
    src/remoting.h \
    src/settingsservice.h \
    src/soundservice.h \
    src/taskservice.h

SOURCES += \
    src/clockserver.cpp \
    src/fontservice.cpp \
    src/main.cpp \
    src/alarm.cpp \
    src/alarmservice.cpp \
    src/jsonspecification.cpp \
    src/palette.cpp \
    src/model.cpp \
    src/pathservice.cpp \
    src/payload.cpp \
    src/loghandling.cpp \
    src/remoting.cpp \
    src/settingsservice.cpp \
    src/soundservice.cpp \
    src/taskservice.cpp

RESOURCES += \
    fonts/fonts.qrc \
    qml/qml.qrc \
    sounds/sounds.qrc

# message('"$${system(date --iso-8601=seconds)}"')
# DEFINES += BUILD_TIME='"$${system(date --iso-8601=seconds)}"'
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=


