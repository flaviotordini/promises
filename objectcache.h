#ifndef OBJECTCACHE_H
#define OBJECTCACHE_H

/**
 * @brief A simple "map" that autoremoves destroyed objects. Can hold only pointers to QObject
 * subclasses.
 *
 * While not directly tied to promises, this class is useful in the common use case of sharing a
 * single promise instance across multiple function calls, in order to do the processing once and
 * then notify all clients with same result.
 *
 * static ObjectCache<QString, Promise<QJsonDocument>> promises;
 * auto promise = promises.value(myId);
 * if (promise) return promise;
 * else promise = promises.create(myId);
 *
 */
template <class Key, class T> class ObjectCache {
public:
    /**
     * @brief A shared instance for cases when having a dedicated cache is overkill
     */
    static ObjectCache &instance() {
        static ObjectCache i;
        return i;
    }

    T *value(Key key) {
        auto i = cache.constFind(key);
        if (i != cache.constEnd()) return i.value();
        return nullptr;
    }

    void remove(Key key) { cache.remove(key); }

    void insert(Key key, T *obj) {
        obj->connect(obj, &QObject::destroyed, [key, this] { remove(key); });
        cache.insert(key, obj);
    }

    T *create(Key key) {
        auto obj = new T();
        insert(key, obj);
        return obj;
    }

private:
    QMap<Key, T *> cache;
};

#endif // OBJECTCACHE_H
