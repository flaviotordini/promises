#ifndef PROMISE_H
#define PROMISE_H

#include <QtCore>

/// private, don't use directly
class BasePromise : public QObject {
    Q_OBJECT

public:
    explicit BasePromise(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &BasePromise::resolve, this, &QObject::deleteLater);
        connect(this, &BasePromise::reject, this, &QObject::deleteLater);
    };

    template <typename Function> BasePromise &onFailed(Function func) {
        connect(this, &BasePromise::reject, this, func);
        return *this;
    }
    template <typename Function> BasePromise &finally(Function func) {
        connect(this, &BasePromise::destroyed, this, func);
        return *this;
    }

signals:
    void resolve();
    void reject(const QString &message);
};

template <class T> class Promise : public BasePromise {
public:
    explicit Promise(QObject *parent = nullptr) : BasePromise(parent) {}

    template <typename Function> BasePromise &then(Function func) {
        connect(this, &BasePromise::resolve, this, [this, func] { func(data); });
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
