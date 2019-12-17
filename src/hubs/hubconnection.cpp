#include "signalr/hubs/hubconnection.hpp"
#include "hubproxy.hpp"

namespace signalr {
namespace hubs {

HubConnection::HubConnection(QString url) : Connection(std::move(url)) {
}

std::shared_ptr<IHubProxy> HubConnection::createHubProxy(const QString& hubName) {
  if(m_Hubs.contains(hubName)) return m_Hubs[hubName];

  std::shared_ptr<HubProxy> hubProxy = std::make_shared<HubProxy>(shared_from_this(), hubName);
  m_Hubs.insert(hubName, hubProxy);
  return std::dynamic_pointer_cast<IHubProxy>(hubProxy);
}

std::promise<void> HubConnection::start() {
  return Connection::start();
}

std::promise<void> HubConnection::start(std::shared_ptr<transports::IClientTransport> pClientTransport) {
  return Connection::start(pClientTransport);
}

void HubConnection::stop() {
  Connection::stop();
}

void HubConnection::stop(int timeout) {
  Connection::stop(timeout);
}

void HubConnection::setClientCertificate(QSslConfiguration configuration) {
  Connection::setClientCertificate(std::move(configuration));
}

QString HubConnection::registerCallback(Action<HubResult> callback) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    {
        QString id = QString::number(m_callbackId);
        m_callback[id] = callback;
        m_callbackId++;
        return id;
    }
}

void HubConnection::removeCallback(const QString& callbackId) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    {
        m_callback.remove(callbackId);
    }
}

}
}
