INCLUDEPATH += $$PWD

QT *= core

HEADERS += \
    $$PWD/emptypromise.h \
    $$PWD/objectcache.h \
    $$PWD/promise.h \
    $$PWD/variantpromise.h

contains(QT, qml) {
    HEADERS += $$PWD/jspromise.h
}
