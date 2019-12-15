#ifndef IHUBPROXY_HPP
#define IHUBPROXY_HPP

#include <QString>
#include <QException>

#include <future>

namespace signalr {

class IHubProxy {
public:

    template<typename ...Args>
    std::future<void> invoke(const QString& methodname, Args...){
        throw QException();
    }

    template<typename T, typename ...Args>
    std::future<T> invoke(const QString &methodname, Args...) {
        throw QException();
    }

    virtual ~IHubProxy() = default;

};

}

#endif //IHUBPROXY_HPP
