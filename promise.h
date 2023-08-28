#ifndef PROMISE_H
#define PROMISE_H

#include <QtCore>

/// private, don't use directly
class BasePromise : public QObject {
    Q_OBJECT

public:
    explicit BasePromise(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void resolve();
    void reject(const QString &message);
};

template <class T> class Promise : public BasePromise {
public:
    explicit Promise(QObject *parent = nullptr) : BasePromise(parent) {
        connect(this, &BasePromise::resolve, this, &QObject::deleteLater);
        connect(this, &BasePromise::reject, this, &QObject::deleteLater);
    }

    template <typename Function> Promise<T> &then(Function func) {
        connect(this, &BasePromise::resolve, this, [this, func] { func(data); });
        return *this;
    }

    template <typename Function> Promise<T> &onFailed(Function func) {
        connect(this, &BasePromise::reject, this, func);
        return *this;
    }

    template <typename Function> Promise<T> &finally(Function func) {
        connect(this, &BasePromise::destroyed, this, func);
        return *this;
    }

    void resolve(T value) {
        data = value;
        BasePromise::resolve();
    }

    T result() const { return data; }

private:
    T data;
};

#endif // PROMISE_H
