#ifndef HUBCONNECTION_HPP
#define HUBCONNECTION_HPP

#include "connection.hpp"
#include "signalr/hubs/ihubproxy.hpp"
#include "signalr/hubs/ihubconnection.hpp"

namespace signalr {
namespace hubs {

class HubConnection : public IHubConnection, public Connection {

public:
    HubConnection(QString url);

    virtual std::shared_ptr<IHubProxy> createHubProxy(const QString& hubName) override;

    virtual std::future<void> start() override;

    virtual std::future<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;

    virtual void stop() override;

    virtual void stop(int timeout) override;

    virtual void setClientCertificate(QSslConfiguration configuration) override;

    ~HubConnection() {}

private:
    QString m_Url;
    QHash<QString, std::shared_ptr<IHubProxy>> m_Hubs;
};

} //end hubs
} //end signalr

#endif //HUBCONNECTION_HPP
