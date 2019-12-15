#include "signalr/hubs/hubconnection.hpp"
#include "hubproxy.hpp"

namespace signalr {
namespace hubs {

HubConnection::HubConnection(QString url) : Connection(std::move(url)) {
}

std::shared_ptr<IHubProxy> HubConnection::createHubProxy(const QString& hubName) {
  if(m_Hubs.contains(hubName)) return m_Hubs[hubName];

  std::shared_ptr<IHubProxy> hubProxy = std::make_shared<HubProxy>();
  m_Hubs.insert(hubName, hubProxy);
  return hubProxy;
}

std::future<void> HubConnection::start() {
  return Connection::start();
}

std::future<void> HubConnection::start(std::shared_ptr<transports::IClientTransport> pClientTransport) {
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

}
}
