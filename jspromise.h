#ifndef JSPROMISE_H
#define JSPROMISE_H

#include <QJSEngine>

/***
 * A Promise object to be used with QML
 *
 */
class JSPromise : public QObject {
    Q_OBJECT

public:
    explicit JSPromise(QJSEngine *parent) : QObject(parent), engine(parent) {
        connect(this, &JSPromise::resolveJSValue, this, &QObject::deleteLater);
        connect(this, &JSPromise::reject, this, &QObject::deleteLater);
    }

    QJSValue toJSValue() { return engine->toScriptValue(this); }

    Q_INVOKABLE QJSValue then(QJSValue func) {
        connect(this, &JSPromise::resolveJSValue, this, [func](auto value) { func.call({value}); });
        return toJSValue();
    }

    Q_INVOKABLE QJSValue onFailed(QJSValue func) {
        connect(this, &JSPromise::reject, this, [func](auto msg) { func.call({msg}); });
        return toJSValue();
    }

    Q_INVOKABLE QJSValue finally(QJSValue func) {
        connect(this, &JSPromise::destroyed, this, [func]() { func.call(); });
        return toJSValue();
    }

    template <class T> void resolve(T value) { emit resolveJSValue(engine->toScriptValue(value)); }

signals:
    void resolveJSValue(QJSValue value);
    void reject(const QString &message);

private:
    QJSEngine *engine;
};
#endif // JSPROMISE_H
