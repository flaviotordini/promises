#ifndef EMPTYPROMISE_H
#define EMPTYPROMISE_H

#include <QtCore>

class EmptyPromise : public QObject {
    Q_OBJECT

public:
    explicit EmptyPromise(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &EmptyPromise::success, this, &QObject::deleteLater);
        connect(this, &EmptyPromise::error, this, &QObject::deleteLater);
    };

    template <typename Functor> EmptyPromise &onSuccess(Functor func) {
        connect(this, &EmptyPromise::success, this, func);
        return *this;
    }
    template <typename Functor> EmptyPromise &onError(Functor func) {
        connect(this, &EmptyPromise::error, this, func);
        return *this;
    }
    template <typename Functor> EmptyPromise &finally(Functor func) {
        connect(this, &EmptyPromise::destroyed, this, func);
        return *this;
    }

signals:
    void success();
    void error(const QString &message);
};

#endif // EMPTYPROMISE_H
