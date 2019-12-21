#include "signalr/hubs/hubconnection.hpp"
#include "hubproxy.hpp"

#include <QJsonDocument>

namespace signalr {
namespace hubs {

HubConnection::HubConnection(QString url)
    : Connection(std::move(url)) {
}

HubConnection::HubConnection(QString url, bool useDefaultUrl)
    : Connection(getUrl(url, useDefaultUrl)) {
}

HubConnection::HubConnection(QString url, QString queryString)
    : Connection(std::move(url), std::move(queryString)){
}

HubConnection::HubConnection(QString url, QString queryString, bool useDefaultUrl)
    : Connection(getUrl(url, useDefaultUrl), std::move(queryString)){
}

HubConnection::HubConnection(QString url, QList<QPair<QString, QString>> queryString)
    : Connection(std::move(url), std::move(queryString)){
}

std::shared_ptr<IHubProxy> HubConnection::createHubProxy(const QString& hubName) {
  if(m_Hubs.contains(hubName)) return m_Hubs[hubName];

  using hubConnectionShared = enable_shared_from_this<HubConnection>;
  std::shared_ptr<HubProxy> hubProxy = std::make_shared<HubProxy>(hubConnectionShared::shared_from_this(), hubName);
  m_Hubs.insert(hubName, hubProxy);
  return std::dynamic_pointer_cast<IHubProxy>(hubProxy);
}

QtPromise::QPromise<void> HubConnection::start(std::shared_ptr<http::IHttpClient> pHttpClient) {
  return Connection::start(pHttpClient);
}

QtPromise::QPromise<void> HubConnection::start(std::shared_ptr<transports::IClientTransport> pClientTransport) {
  return Connection::start(pClientTransport);
}

void HubConnection::stop() {
  Connection::stop();
}

void HubConnection::stop(const TimeDelta& timeout) {
  Connection::stop(timeout);
}

void HubConnection::removeCallback(const QString& callbackId) {
  std::lock_guard<std::mutex> lock(m_Mutex);
  m_callback.remove(callbackId);
}

//void HubConnection::setClientCertificate(QSslConfiguration configuration) {
//  Connection::setClientCertificate(std::move(configuration));
//}

QString HubConnection::onSending() {
  throw QException();
}

void HubConnection::onMessageReceived(const QJsonDocument &message) {
  if(!message["P"].isNull()) {
    //auto result = message.object()JsonSerializer
  }
  else if(!message["I"].isNull()) {

  }
  else {

  }
}

void HubConnection::onReceived(const QJsonDocument& data) {

}

void HubConnection::onError(const QException& error) {

}

void HubConnection::onReconnecting() {

}

void HubConnection::onReconnected() {

}

void HubConnection::onConnectionSlow() {

}

QString HubConnection::getUrl(QString& url, bool useDefaultUrl) {
  if(!url.endsWith("/")) url += "/";
  if(useDefaultUrl) return url + "signalr";
  return url;
}

}
}
