#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QObject>
#include <QSslConfiguration>

#include "signalr/iconnection.hpp"

namespace signalr {

class Connection : public IConnection {

public:
    Connection(QString url);

    virtual std::future<void> start() override;
    virtual std::future<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;

    virtual void stop() override;
    virtual void stop(int timeout) override;

    virtual void setClientCertificate(QSslConfiguration configuration) override;


    virtual ~Connection() = default;

private:
    QString m_Url;
    QSslConfiguration m_Configuration;
};

}
#endif //CONNECTION_HPP
