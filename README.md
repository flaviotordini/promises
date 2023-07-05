# Qt Promises

A very simple Javascript-style Promise implementation for Qt. The main idea is to use QObject features (i.e. signal and slots, deleteLater) to implement promises.

I wrote this Promise implementation in Qt 5 times. I'm not sure whether Qt 6 has something comparable in terms of simplicity and lightness. Anyway, it works great in Qt 6 too and I'm using it in my apps at https://flavio.tordini.org

In the Qt world a signal is emitted by an object instance. Sometimes that's exactly what's needed, like in the Observer design pattern (aka Publisher/Subscriber). In other cases the asynchronous response should be bound to a specific function call and having an instance-wide signal can be a cause of concurrency problems. Using promises to return async data from a function is a better solution: in Qt terms, it basically creates a temporary object with its own signal connection which is not shared with other users of a function.

## Example usage

The main class is Promise. It is template-based. Use it like this:

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
