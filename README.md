# Qt Promises

Very simple Promise implementation for Qt. The main idea is to use QObject features (i.e. signal and slots, deleteLater) to implement promises.

I wrote these in Qt 5 times. I'm not sure whether Qt 6 has something comparable in terms of simplicity and lightness. Anyway, these work great in Qt 6 too and I'm using this in my apps at https://flavio.tordini.org

The main class is Promise. It is template-based. Use it like:

```c++
Promise<QString> &myAsyncFunction() {
  auto promise = new Promise<QString>(this);

  // any asynchronous work you might have to do:
  connect(myObj, &MyObject::finished, this, [promise](auto result) {
    if (myObj.hasError()) promise->reject(myObj.errorMessage());
    else promise->resolve(result);
  });
  myObj.doSomething();

  return *promise;
}

myAsyncFunction().then([](auto result) {
  qDebug() << "Success!" << result;
}).onFailed([](auto errorMessage)) {
  qWarning() << "Error!" << errorMessage;
}).finally([] {
  // optionally do something in both cases
});
```
