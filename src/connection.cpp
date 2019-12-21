#include "connection.hpp"

#include <utility>

#include <QJsonDocument>

#include "signalr/transports/websockettransport.hpp"

namespace signalr {

auto defaultConstructor = [](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject){};
TimeDelta Connection::m_DefaultAbortTimeout = TimeDelta::FromSeconds(30);
QVersionNumber Connection::m_MinimumSupportedVersion = QVersionNumber(1, 4);
QVersionNumber Connection::m_MaximumSupportedVersion = QVersionNumber(2, 1);
QVersionNumber Connection::m_MinimumSupportedNegotiateRedirectVersion = QVersionNumber(2, 0);
int Connection::m_MaxRedirects = 100;
QVersionNumber Connection::m_AssemblyVersion;

Connection::Connection(QString url)
    : Connection(std::move(url), QString()) {
}

Connection::Connection(QString url, QList<QPair<QString, QString>> queryString)
    : Connection(std::move(url), createQueryString(queryString)) {
}

Connection::Connection(QString url, QString queryString)
    : m_ConnectTask(defaultConstructor), m_LastQueuedReceiveTask(defaultConstructor){
  if(url.isNull()) throw QException();
  if(url.contains("?")) throw  QException();
  if(!url.endsWith("/")) url += "/";

  m_UserUrl = url;
  m_ActualUrl = url;
  m_UserQueryString = queryString;
  m_ActualQueryString = queryString;
  m_LastMessageAt = QDateTime::currentDateTimeUtc();
  m_LastActiveAt = QDateTime::currentDateTimeUtc();
  m_ReconnectWindow = TimeDelta::FromSeconds(0);
  setConnectionState(ConnectionState::Disconnected);
  //Trace
  //Trace
  //m_Headers =
  m_TransportConnectTimeout = TimeDelta::FromSeconds(0);
  m_TotalTransportConnectTimeout = TimeDelta::FromSeconds(0);
  m_DeadlockErrorTimeout = TimeDelta::FromSeconds(10);
  m_Protocol = QVersionNumber(2, 1);
}

TimeDelta Connection::getTotalTransportConnectTimeout() const {
  return m_TotalTransportConnectTimeout;
}

QVersionNumber Connection::getProtocol() const {
  return m_Protocol;
}

void Connection::setProtocol(QVersionNumber protocol) {
  m_Protocol = std::move(protocol);
}

QException Connection::getLastError() const {
  return m_LastError;
}

TimeDelta Connection::getReconnectWindow() const {
  return m_ReconnectWindow;
}

void Connection::setReconnectWindow(TimeDelta reconnectWindow) {
  m_ReconnectWindow = std::move(reconnectWindow);
}

KeepAliveData Connection::getKeepAliveData() const {
  return m_KeepAliveData;
}

void Connection::setKeepAliveData(KeepAliveData keepAliveData) {
  m_KeepAliveData = std::move(keepAliveData);
}

QDateTime Connection::getLastMessageAt() const {
  return m_LastMessageAt;
}

QDateTime Connection::getLastActiveAt() const {
  return m_LastActiveAt;
}

QList<QSslConfiguration> Connection::getCertificates() const {
  return m_CertCollection;
}

std::shared_ptr<QNetworkCookieJar> Connection::getCookieContainer() const {
  return m_pCookieContainer;
}

QAuthenticator Connection::getCredentials() const {
  return m_Credentials;
}

void Connection::setCredentials(QAuthenticator credential) {
  m_Credentials = std::move(credential);
}

QList<QPair<QString, QString> > Connection::getHeaders() const {
  return m_Headers;
}

QNetworkProxy Connection::getProxy () const {
  return m_Proxy;
}

void Connection::setProxy(QNetworkProxy proxy) {
  m_Proxy = std::move(proxy);
}

QString Connection::getUrl() const {
  return metaObject()->className() == QString("IConnection")
          ? m_ActualUrl
          : m_UserUrl;
}

QString Connection::getMesssageId() const {
  return m_MessageId;
}

void Connection::setMessageId(QString messageId) {
  m_MessageId = std::move(messageId);
}

QString Connection::getConnectionId() const {
  return m_ConnectionId;
}

QString Connection::getConnectionToken() const {
  return m_ConnectionToken;
}

QString Connection::getGroupsToken() const {
  return m_GroupsToken;
}

void Connection::setGroupsToken(QString groupsToken) {
  m_GroupsToken = std::move(groupsToken);
}

QHash<QString, QObject*> Connection::getItems() const {
  return m_Items;
}

QString Connection::getQueryString() const {
  return metaObject()->className() == QString("IConnection")
          ? m_ActualQueryString
          : m_UserQueryString;
}

std::shared_ptr<transports::IClientTransport> Connection::getClientTransport() const {
  return m_pClientTransport;
}

ConnectionState Connection::getConnectionState() const {
  return m_State;
}

void Connection::setConnectionState(ConnectionState state) {
  std::lock_guard<std::mutex> lock(m_StateLock);
  if(m_State == state) return;
  m_State = state;
  emit stateChanged(m_State, state);
}

QtPromise::QPromise<void> Connection::start(std::shared_ptr<http::IHttpClient> pHttpClient) {
  return start(std::make_shared<transports::WebSocketTransport>(std::move(pHttpClient)));
}

QtPromise::QPromise<void> Connection::start(std::shared_ptr<transports::IClientTransport> pClientTransport) {
  if(pClientTransport == nullptr) throw QException();

  std::lock_guard<std::mutex> lock(m_StartLock);
  if(!changeState(ConnectionState::Disconnected, ConnectionState::Connecting))
      return m_ConnectTask;

  m_pClientTransport = std::move(pClientTransport);
  m_ConnectTask = negotiate(m_pClientTransport);
  return m_ConnectTask;
}

QString Connection::onSending() {
  return QString();
}

QtPromise::QPromise<void> Connection::negotiate(std::shared_ptr<transports::IClientTransport> pClientTransport) {
  auto negotiationAttempts = 0;  
  auto pIConnection = std::dynamic_pointer_cast<IConnection>(shared_from_this());

  m_ConnectionData = onSending();
  return startNegotiation(pIConnection, negotiationAttempts);
}

QtPromise::QPromise<void> Connection::completeNegotiation(std::shared_ptr<IConnection> pIConnection, int& negotiationAttempts, const NegotiationResponse& negotiationResponse) {
  m_ConnectionId = negotiationResponse.getConnectionId();
  m_ConnectionToken = negotiationResponse.getConnectionToken();
  m_DisconnectTimeout = TimeDelta::FromSeconds(5);
  m_TotalTransportConnectTimeout = m_TransportConnectTimeout + TimeDelta::FromSeconds(negotiationResponse.getTransportConnectTimeout());

  auto beatInterval = TimeDelta::FromSeconds(5);

  if(negotiationResponse.getKeepAliveTimeout().has_value()) {
    m_KeepAliveData = KeepAliveData(TimeDelta::FromSeconds(negotiationResponse.getKeepAliveTimeout().value()));
    m_ReconnectWindow = m_DisconnectTimeout + m_KeepAliveData.getTimeout();

    beatInterval = m_KeepAliveData.getCheckInterval();
  }
  else {
    m_ReconnectWindow = m_DisconnectTimeout;
  }
  // Monitor = new HeartbeatMonitor(this, _stateLock, beatInterval);
  return startTrasport();
}

QtPromise::QPromise<void> Connection::startNegotiation(std::shared_ptr<IConnection> pIConnection, int& negotiationAttempts) {
    return m_pClientTransport->negotiate(pIConnection, m_ConnectionData)
      .then([&](const NegotiationResponse& negotiationResponse) {
         QVersionNumber protocolVersion;
         if(protocolVersion >= m_MinimumSupportedNegotiateRedirectVersion) {
           if(!negotiationResponse.getError().isNull() || !negotiationResponse.getError().isEmpty()) throw QException();
           if(!negotiationResponse.getRedirectUrl().isNull() || !negotiationResponse.getRedirectUrl().isEmpty()) {
             auto splitUrlAndQuery = negotiationResponse.getRedirectUrl().split('?');
             m_ActualUrl = splitUrlAndQuery[0];
             splitUrlAndQuery.length() == 2 && !(splitUrlAndQuery[1].isNull() || splitUrlAndQuery[1].isEmpty())
                     ? m_ActualQueryString = splitUrlAndQuery[1] : m_ActualQueryString = m_UserQueryString ;

             if(!m_ActualUrl.endsWith("/")) m_ActualUrl += "/";
             if(!negotiationResponse.getAccessToken().isNull() || !negotiationResponse.getAccessToken().isEmpty()) {
                QPair<QString, QString> header;
                header.first = "Authorization";
                header.second = "Bearer "+ negotiationResponse.getAccessToken();
                m_Headers.append(header);
             }
             negotiationAttempts += 1;
             if(negotiationAttempts >= m_MaxRedirects) throw QException();
             return completeNegotiation(pIConnection, negotiationAttempts, negotiationResponse);
           }
         }

         return completeNegotiation(pIConnection, negotiationAttempts, negotiationResponse);
    }).fail([&]() {
      disconnect();
    });
}

QtPromise::QPromise<void> Connection::startTrasport() {
   return m_pClientTransport->start(shared_from_this(), m_ConnectionData)
           //RunSync
           .then([&]() { m_LastQueuedReceiveTask; });
}

bool Connection::changeState(ConnectionState oldState, ConnectionState newState) {
  std::lock_guard<std::mutex> lock (m_StateLock);
  if(m_State != oldState) return false;
  //trace.
  setConnectionState(newState);
  return true;
}

QVersionNumber Connection::verifyProtocolVersion(const QString& versionString) {
  QVersionNumber version;
  if((versionString.isNull() || versionString.isEmpty())
          || !tryParseVersion(versionString, version)
          || version < m_MinimumSupportedVersion
          || version > m_MaximumSupportedVersion) {
    throw QException();
  }
  return version;
}

void Connection::stop() {
  stop(m_DefaultAbortTimeout);
}

void Connection::stop(const QException& error) {
  stop(error, m_DefaultAbortTimeout);
}

void Connection::stop(const QException& error, const TimeDelta& timeout) {
  onError(error);
  stop(timeout);
}

void Connection::stop(const TimeDelta& timeout) {
  std::lock_guard<std::mutex> lock (m_StartLock);
  m_ConnectTask.timeout(1000)
          .fail([](const QtPromise::QPromiseTimeoutException& e) {
      //trace.
  });
  //taskQueue
  if(m_State == ConnectionState::Disconnected) return;
  //trace
  m_pClientTransport->abort(shared_from_this(), timeout, m_ConnectionData);
  disconnect();
}

void Connection::disconnect() {
  std::lock_guard<std::mutex> lock (m_StateLock);
  if(m_State == ConnectionState::Disconnected) return;
  setConnectionState(ConnectionState::Disconnected);
  //trace.
  //monitor.
  if(m_pClientTransport != nullptr) {
    //trace.
    m_pClientTransport.reset();
    m_pClientTransport = nullptr;
  }
  //trace.

  m_ConnectionId = nullptr;
  m_ConnectionId = nullptr;
  m_ConnectionToken = nullptr;
  m_GroupsToken = nullptr;
  m_MessageId = nullptr;
  m_ConnectionData = nullptr;
  m_ActualUrl = m_UserUrl;
  m_ActualQueryString = m_UserQueryString;

  onClosed();
}

void Connection::onClosed() {
  emit closed();
}

QtPromise::QPromise<void> Connection::send(const QString& data) {
  if(m_State == ConnectionState::Disconnected) throw QException();
  if(m_State == ConnectionState::Connecting) throw QException();
  return m_pClientTransport->send(shared_from_this(), data, m_ConnectionData);
}

QtPromise::QPromise<void> Connection::send(const QObject& data) {
  throw QException();
}

void Connection::setClientCertificate(QSslConfiguration configuration) {
  std::lock_guard<std::mutex> lock (m_StateLock);
  if(m_State != ConnectionState::Disconnected) throw QException();
  m_CertCollection.append(configuration);
}

//trace method.

void Connection::onReceived(const QJsonDocument &data) {
  //m_LastQueuedReceiveTask =
  onMessageReceived(data);
}

void Connection::onMessageReceived(const QJsonDocument &message) {
  emit received(message.toJson());
}

void IConnection::onError(const QException& error) {
  this->onError(error);
}

void IConnection::onReconnecting() {
  this->onReconnecting();
}

void Connection::onReconnecting() {
  emit reconnecting();
}

void Connection::onError(const QException& error) {

}

void Connection::onReconnected() {
  emit reconnected();
  //mointer
  ((IConnection*)this)->markLastMessage();
}

void Connection::onConnectionSlow() {
  //trace
  emit connectionSlow();
}

void Connection::markLastMessage() {
  m_LastMessageAt = QDateTime::currentDateTimeUtc();
}

void Connection::markArchive() {
  //TransportHelper
  m_LastActiveAt = QDateTime::currentDateTimeUtc();
}

void Connection::prepareRequest(std::shared_ptr<http::IRequest> pRequest) {
  pRequest->setUserAgent(createUserAgentString("SignalR.Client.Qt"));
  pRequest->setRequestHeaders(m_Headers);
}

QString Connection::createUserAgentString(const QString& client) {
  return QString(client + "/" + QT_VERSION_STR + QSysInfo::prettyProductName());
}

bool Connection::tryParseVersion(const QString& versionString, QVersionNumber& version) {
  version = version.fromString(versionString);
  return version.isNull();
}

QString Connection::createQueryString(const QList<QPair<QString, QString>>& queryString) {
  QStringList query;
  for(auto const& qs : queryString) {
    query << qs.first + "=" + qs.second;
  }
  return query.join("&");
}

}
