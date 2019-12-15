#ifndef ICONNECTION_HPP
#define ICONNECTION_HPP

#include <future>
#include <QSslConfiguration>

#include "hubs/ihubproxy.hpp"
#include "transports/iclienttransport.hpp"

namespace signalr {

class IConnection {
public:

    /**
     * @brief start
     * @return
     */
    virtual std::future<void> start() = 0;

    /**
     * @brief start
     * @param pClientTransport
     * @return
     */
    virtual std::future<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) = 0;

    /**
     * @brief stop
     */
    virtual void stop() = 0;

    /**
     * @brief stop
     * @param timeout
     */
    virtual void stop(int timeout) = 0;

    /**
     * @brief setClientCertificate
     * @param configuration
     */
    virtual void setClientCertificate(QSslConfiguration configuration) = 0;

    virtual ~IConnection() = default;
};

}

#endif //ICONNECTION_HPP
