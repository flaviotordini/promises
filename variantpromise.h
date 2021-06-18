#ifndef VARIANTPROMISE_H
#define VARIANTPROMISE_H

#include <QtCore>

class VariantPromise : public QObject {
    Q_OBJECT

public:
    explicit VariantPromise(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &VariantPromise::success, this, &QObject::deleteLater);
        connect(this, &VariantPromise::error, this, &QObject::deleteLater);
    };

    template <typename Functor> VariantPromise &onSuccess(Functor func) {
        connect(this, &VariantPromise::success, this, func);
        return *this;
    }
    template <typename Functor> VariantPromise &onError(Functor func) {
        connect(this, &VariantPromise::error, this, func);
        return *this;
    }
    template <typename Functor> VariantPromise &finally(Functor func) {
        connect(this, &VariantPromise::success, this, func);
        connect(this, &VariantPromise::error, this, func);
        return *this;
    }

signals:
    void success(QVariant result);
    void error(const QString &message);
};

#endif // VARIANTPROMISE_H
