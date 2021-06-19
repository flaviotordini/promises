#ifndef VARIANTPROMISE_H
#define VARIANTPROMISE_H

#include <QtCore>

class VariantPromise : public QObject {
    Q_OBJECT

public:
    explicit VariantPromise(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &VariantPromise::resolve, this, &QObject::deleteLater);
        connect(this, &VariantPromise::reject, this, &QObject::deleteLater);
    };

    template <typename Function> VariantPromise &then(Function func) {
        connect(this, &VariantPromise::resolve, this, func);
        return *this;
    }
    template <typename Function> VariantPromise &onFailed(Function func) {
        connect(this, &VariantPromise::reject, this, func);
        return *this;
    }
    template <typename Function> VariantPromise &finally(Function func) {
        connect(this, &VariantPromise::destroyed, this, func);
        return *this;
    }

signals:
    void resolve(QVariant result);
    void reject(const QString &message);
};

#endif // VARIANTPROMISE_H
