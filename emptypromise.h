#ifndef EMPTYPROMISE_H
#define EMPTYPROMISE_H

#include <QtCore>

class EmptyPromise : public QObject {
    Q_OBJECT

public:
    explicit EmptyPromise(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &EmptyPromise::resolve, this, &QObject::deleteLater);
        connect(this, &EmptyPromise::reject, this, &QObject::deleteLater);
    };

    template <typename Functor> EmptyPromise &then(Functor func) {
        connect(this, &EmptyPromise::resolve, this, func);
        return *this;
    }
    template <typename Functor> EmptyPromise &onFailed(Functor func) {
        connect(this, &EmptyPromise::reject, this, func);
        return *this;
    }
    template <typename Functor> EmptyPromise &finally(Functor func) {
        connect(this, &EmptyPromise::destroyed, this, func);
        return *this;
    }

signals:
    void resolve();
    void reject(const QString &message);
};

#endif // EMPTYPROMISE_H
