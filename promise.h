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

    static Promise<QList<T>> &all(QList<Promise<T> *> promises) {
        QPointer<Promise<QList<T>>> promise = new Promise<QList<T>>;
        auto count = new int(promises.size());
        auto results = new QList<T>;
        results->reserve(promises.size());
        for (auto p : promises) {
            p.then([count, results, promise, p](auto result) {
                 if (!promise) return;
                 results << result;
                 if (results->size() == count) {
                     promise->resolve(results);
                     delete count, delete results;
                 }
             }).onFailed([count, results, promise](auto msg) {
                if (!promise) return;
                promise->reject(msg);
                delete count, delete results;
            });
        }
        return *promise;
    }

    static Promise<QList<T>> &allSettled(QList<Promise<T> *> promises) {
        QPointer<Promise<QList<T>>> promise = new Promise<QList<T>>;
        auto count = new int(promises.size());
        auto outcomes = new QList<T>;
        outcomes->reserve(promises.size());
        for (auto p : promises) {
            p.then([count, outcomes, promise, p](auto result) {
                 if (!promise) return;
                 outcomes << result;
                 if (outcomes->size() == count) {
                     promise->resolve(outcomes);
                     delete count, delete outcomes;
                 }
             }).onFailed([count, outcomes, promise](auto msg) {
                if (!promise) return;
                promise->reject(msg);
                delete count, delete outcomes;
            });
        }
        return *promise;
    }

private:
    T data;
};

#endif // PROMISE_H
