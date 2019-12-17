#ifndef HUBCONNECTION_HPP
#define HUBCONNECTION_HPP

#include <mutex>
#include "connection.hpp"
#include "hubs/hubproxy.hpp"
#include "signalr/hubs/ihubconnection.hpp"

namespace signalr {
namespace hubs {

class HubConnection : public IHubConnection, public Connection, public std::enable_shared_from_this<HubConnection> {

public:
    HubConnection() = default;

    HubConnection(QString url);

    virtual std::shared_ptr<IHubProxy> createHubProxy(const QString &hubName) override;

    virtual std::promise<void> start() override;

    virtual std::promise<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;

    virtual void stop() override;

    virtual void stop(int timeout) override;

    virtual QString registerCallback(Action<HubResult> callback) override;

    virtual void removeCallback(const QString& callbackId) override;

    virtual void setClientCertificate(QSslConfiguration configuration) override;

    virtual ~HubConnection() = default;

private:

    QString m_Url;
    std::mutex m_Mutex;
    int m_callbackId;
    QHash<QString, std::shared_ptr<HubProxy>> m_Hubs;
    QHash<QString, Action<HubResult>> m_callback;
};

} //end hubs
} //end signalr

#endif //HUBCONNECTION_HPP
