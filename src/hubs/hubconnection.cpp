#include "signalr/hubs/hubconnection.hpp"
#include "hubproxy.hpp"

#include <QtConcurrent>
#include <QJsonDocument>

#include "hubregistrationdata.hpp"

namespace signalr {
namespace hubs {

HubConnection::HubConnection(QString url)
    : HubConnection(std::move(url), true) {
}

HubConnection::HubConnection(QString url, bool useDefaultUrl)
    : Connection(getUrl(url, useDefaultUrl)) {
}

HubConnection::HubConnection(QString url, QString queryString)
    : HubConnection(std::move(url), std::move(queryString), true){
}

HubConnection::HubConnection(QString url, QString queryString, bool useDefaultUrl)
    : Connection(getUrl(url, useDefaultUrl), std::move(queryString)){
}

HubConnection::HubConnection(QString url, QList<QPair<QString, QString>> queryString)
    : HubConnection(std::move(url), std::move(queryString), true){
}

HubConnection::HubConnection(QString url, QList<QPair<QString, QString>> queryString, bool useDefaultUrl)
    : Connection(getUrl(url, useDefaultUrl), std::move(queryString)){
}

void HubConnection::onReconnected() {
  clearInvocationCallbacks("Reconnecting ...");
  Connection::onReconnected();
}

void HubConnection::onMessageReceived(const QJsonValue &message) {
  if(!message["P"].isUndefined() && !message["P"].isNull()) {
    auto result = getJsonSerializer()->deserialize<HubResult>(message.toObject());
    std::function<void(HubResult)> callback;
    {
      std::lock_guard<std::mutex> lock(m_CallbackMutex);
      if(m_Callbacks.contains(result.getHubProgressUpdate().getId()))
          callback = m_Callbacks[result.getHubProgressUpdate().getId()];
      //Trace.
    }

    if(callback != nullptr)
      callback(result);
  }

  else if(!message["I"].isUndefined() && !message["I"].isNull()) {
     auto result = getJsonSerializer()->deserialize<HubResult>(message.toObject());
     std::function<void(HubResult)> callback;
     {
       std::lock_guard<std::mutex> lock(m_CallbackMutex);
       if(m_Callbacks.contains(result.getId())) {
         callback = m_Callbacks.take(result.getId());
       }
       //Trace.
     }

     if(callback != nullptr)
        callback(result);
  }

  else {
      try {
        HubInvocation invocation = getJsonSerializer()->deserialize<HubInvocation>(message.toObject());
        std::shared_ptr<HubProxy> hubProxy;
        if(m_Hubs.contains(invocation.getHub())) {
          hubProxy = m_Hubs[invocation.getHub()];

          QMap<QString, QJsonDocument>::iterator it;
          for(it = invocation.getState().begin(); it != invocation.getState().end(); ++it) {
            hubProxy->operator[](it.key()) = it.value();
          }

          hubProxy->invokeEvent(invocation.getMethod(), invocation.getArgs());
        }
      } catch (...) {
         qDebug() << "Error when invokeing..";
      }
  }

  Connection::onMessageReceived(message);
}

QString HubConnection::onSending() {
  QList<HubRegistrationData> data;
  QHash<QString, std::shared_ptr<HubProxy>>::iterator it;

  for(it = m_Hubs.begin(); it != m_Hubs.end() ; ++it) {
    auto hubRegistrationData = HubRegistrationData();
    hubRegistrationData.setName(it.key());
    data.append(hubRegistrationData);
  }

  auto json = jsonSerializeObject(shared_from_this(), QVariant::fromValue<QList<HubRegistrationData>>(data));
  return json;
}

void HubConnection::onClosed() {
  clearInvocationCallbacks("Closing ...");
  Connection::onClosed();
}

std::shared_ptr<IHubProxy> HubConnection::createHubProxy(const QString& hubName) {
  if(m_Hubs.contains(hubName)) return m_Hubs[hubName];

  std::shared_ptr<HubProxy> hubProxy = std::make_shared<HubProxy>(std::shared_ptr<HubConnection>(this), hubName);
  m_Hubs.insert(hubName, hubProxy);
  return std::dynamic_pointer_cast<IHubProxy>(hubProxy);
}

QString HubConnection::registerCallback(std::function<void(HubResult)> callback) {
  std::lock_guard<std::mutex> lock(m_CallbackMutex);
  QString id = QString::number(m_callbackId);
  m_Callbacks[id] = callback;
  return id;
}

void HubConnection::removeCallback(const QString& callbackId) {
  std::lock_guard<std::mutex> lock(m_Mutex);
  m_Callbacks.remove(callbackId);
}

QString HubConnection::getUrl(QString& url, bool useDefaultUrl) {
  if(!url.endsWith("/")) url += "/";
  if(useDefaultUrl) return url + "signalr";
  return url;
}

void HubConnection::clearInvocationCallbacks(const QString& error) {
  QList<std::function<void(HubResult)>> callbacks; {
     std::lock_guard<std::mutex> lock(m_CallbackMutex);
     callbacks = m_Callbacks.values();
  }

  for(auto& callback : callbacks){
    QtPromise::QPromise<void> task = QtPromise::resolve(QtConcurrent::run([&](){
        auto hubResult = HubResult();
        hubResult.setError(error);
        callback(hubResult);
      })
    );
  }

}

std::shared_ptr<IHubProxy> HubConnection::operator[] (const QString& hubName) {
  if(m_Hubs.contains(hubName)) return m_Hubs[hubName];
  throw QException();
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

bool HubConnection::changeState(ConnectionState oldState, ConnectionState newState) {
  return Connection::changeState(oldState, newState);
}

void HubConnection::stop(const QException& error) {
  Connection::stop(error);
}

void HubConnection::disconnect() {
  Connection::disconnect();
}

QtPromise::QPromise<void> HubConnection::send(const QString& data) {
  return Connection::send(data);
}

void HubConnection::setClientCertificate(QSslConfiguration configuration) {
  Connection::setClientCertificate(configuration);
}

void HubConnection::prepareRequest(std::shared_ptr<http::IRequest> pRequest) {
  Connection::prepareRequest(pRequest);
}

void HubConnection::markLastMessage() {
  Connection::markLastMessage();
}

void HubConnection::markArchive() {
  Connection::markArchive();
}

//virtual void trace

QVersionNumber HubConnection::getProtocol() const {
  return Connection::getProtocol();
}

void HubConnection::setProtocol(QVersionNumber protocol) {
  Connection::setProtocol(std::move(protocol));
}

TimeDelta HubConnection::getTrasnportConnectTimeout() const {
  return Connection::getTrasnportConnectTimeout();
}

void HubConnection::setTransportConnectTimeout(TimeDelta transportConnectTimeout) {
  Connection::setTransportConnectTimeout(std::move(transportConnectTimeout));
}

TimeDelta HubConnection::getTotalTransportConnectTimeout() const {
  return Connection::getTotalTransportConnectTimeout();
}

TimeDelta HubConnection::getReconnectWindow() const {
  return Connection::getReconnectWindow();
}

void HubConnection::setReconnectWindow(TimeDelta reconnectWindow) {
  Connection::setReconnectWindow(std::move(reconnectWindow));
}

KeepAliveData HubConnection::getKeepAliveData() const {
  return Connection::getKeepAliveData();
}

void HubConnection::setKeepAliveData(KeepAliveData keepAliveData) {
  Connection::setKeepAliveData(std::move(keepAliveData));
}

QString HubConnection::getMesssageId() const {
  return Connection::getMesssageId();
}

void HubConnection::setMessageId(QString messageId) {
  Connection::setMessageId(std::move(messageId));
}

QString HubConnection::getGroupsToken() const {
  return Connection::getGroupsToken();
}

void HubConnection::setGroupsToken(QString groupsToken) {
  Connection::setGroupsToken(std::move(groupsToken));
}

QHash<QString, QObject*> HubConnection::getItems() const {
  return Connection::getItems();
}

QString HubConnection::getConnectionId() const {
  return Connection::getConnectionId();
}

QString HubConnection::getConnectionToken() const {
  return Connection::getConnectionToken();
}

QString HubConnection::getUrl() const {
  return Connection::getUrl();
}

QString HubConnection::getQueryString() const {
  return Connection::getQueryString();
}

ConnectionState HubConnection::getConnectionState() const {
  return Connection::getConnectionState();
}

std::shared_ptr<transports::IClientTransport> HubConnection::getClientTransport() const  {
  return Connection::getClientTransport();
}

QDateTime HubConnection::getLastMessageAt() const {
  return Connection::getLastMessageAt();
}

QDateTime HubConnection::getLastActiveAt() const {
  return Connection::getLastActiveAt();
}

QNetworkProxy HubConnection::getProxy () const {
  return Connection::getProxy();
}

void HubConnection::setProxy(QNetworkProxy proxy) {
  Connection::setProxy(proxy);
}

QSslConfiguration HubConnection::getCertificate() const {
  return Connection::getCertificate();
}

QList<QPair<QString, QString>> HubConnection::getHeaders() const {
  return Connection::getHeaders();
}

QAuthenticator HubConnection::getCredentials() const {
  return Connection::getCredentials();
}

void HubConnection::setCredentials(QAuthenticator credential) {
  Connection::setCredentials(std::move(credential));
}

QList<QNetworkCookie> HubConnection::getCookieContainer() const {
  return Connection::getCookieContainer();
}

std::shared_ptr<QJsonSerializer> HubConnection::getJsonSerializer()  const {
  return Connection::getJsonSerializer();
}

void HubConnection::onReceived(const QJsonValue &data) {
  Connection::onReceived(data);
}

void HubConnection::onError(const QException& error) {
  Connection::onError(error);
}

void HubConnection::onReconnecting() {
  Connection::onReconnecting();
}

void HubConnection::onConnectionSlow() {
  Connection::onConnectionSlow();
}

HubConnection::~HubConnection() {
}

}
}
