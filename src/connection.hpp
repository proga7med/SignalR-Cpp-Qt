#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <mutex>
#include <memory>

#include <QObject>
#include <QJsonSerializer>
#include <QSslConfiguration>

#include <QtPromise>
#include <time_delta.h>

#include "signalr/heartbeatmonitor.hpp"
#include "signalr/iconnection.hpp"

namespace signalr {
namespace hubs {
class HubConnection;
}

class Connection : public virtual IConnection, public std::enable_shared_from_this<Connection> {
  Q_OBJECT
public:
  Connection(QString url);
  Connection(QString url, QList<QPair<QString, QString>> queryString);
  Connection(QString url, QString queryString);

public:
  virtual TimeDelta getTrasnportConnectTimeout() const override;
  virtual void setTransportConnectTimeout(TimeDelta transportConnectTimeout) override;

  virtual TimeDelta getTotalTransportConnectTimeout() const override;

  virtual QVersionNumber getProtocol() const override;
  virtual void setProtocol(QVersionNumber protocol) override;

  virtual TimeDelta getReconnectWindow() const override;
  virtual void setReconnectWindow(TimeDelta reconnectWindow) override;

  virtual KeepAliveData getKeepAliveData() const override;
  virtual void setKeepAliveData(KeepAliveData keepAliveData) override;

  virtual QDateTime getLastMessageAt() const override;
  virtual QDateTime getLastActiveAt() const override;

  virtual QSslConfiguration getCertificate() const override;
  virtual std::shared_ptr<QJsonSerializer> getJsonSerializer() const override;

  virtual QList<QNetworkCookie> getCookieContainer() const override;

  virtual QAuthenticator getCredentials() const override;
  virtual void setCredentials(QAuthenticator credential) override;

  virtual QList<QPair<QString, QString>> getHeaders() const override;

  virtual QNetworkProxy getProxy () const override;
  virtual void setProxy(QNetworkProxy proxy) override;

  virtual QString getUrl() const override;

  virtual QString getMesssageId() const override;
  virtual void setMessageId(QString messageId) override;

  virtual QString getConnectionId() const override;
  virtual QString getConnectionToken() const override;

  virtual QString getGroupsToken() const override;
  virtual void setGroupsToken(QString groupsToken) override;

  virtual QHash<QString, QObject*> getItems() const override;

  virtual QString getQueryString() const override;

  virtual std::shared_ptr<transports::IClientTransport> getClientTransport() const override;

  virtual ConnectionState getConnectionState() const override;

  virtual QtPromise::QPromise<void> start(std::shared_ptr<http::IHttpClient> pHttpClient) override;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;

  virtual void stop() override;
  virtual void stop(const QException& error) override;
  void stop(const QException& error, const TimeDelta& timeout);
  void stop(const TimeDelta& timeout);

  virtual void disconnect() override;
  virtual QtPromise::QPromise<void> send(const QString& data) override;
  QtPromise::QPromise<void> send(const QObject& data);
  virtual void setClientCertificate(QSslConfiguration configuration) override;
  virtual void prepareRequest(std::shared_ptr<http::IRequest> pRequest) override;
  virtual void markLastMessage() override;
  virtual void markArchive() override;

  QException getLastError() const;

  virtual ~Connection() = default;

public:
   static QString jsonSerializeObject(std::shared_ptr<IConnection> pConnection, const QVariant &value);

protected:
  virtual QString onSending();
  virtual void onClosed();
  virtual void onMessageReceived(const QJsonValue &message);

private:
  virtual void onReceived(const QJsonValue &data) override;
  virtual void onError(const QException& error) override;
  virtual void onReconnecting() override;
  virtual void onReconnected() override;
  virtual void onConnectionSlow() override;
  QtPromise::QPromise<void> negotiate(std::shared_ptr<transports::IClientTransport> pClientTransport);
  QtPromise::QPromise<void> startTransport();
  virtual bool changeState(ConnectionState oldState, ConnectionState newState) override;
  QVersionNumber verifyProtocolVersion(const QString& versionString);
  static QString createUserAgentString(const QString& client);
  static bool tryParseVersion(const QString& versionString, QVersionNumber& version);
  static QString createQueryString(const QList<QPair<QString, QString>>& queryString);
  //setTimeout
  void setConnectionState(ConnectionState state);
  void registerJsonMetaTypes();

private:
  static TimeDelta m_DefaultAbortTimeout;
  static QVersionNumber m_MinimumSupportedVersion;
  static QVersionNumber m_MaximumSupportedVersion;
  static QVersionNumber m_MinimumSupportedNegotiateRedirectVersion;
  static int m_MaxRedirects;
  static QVersionNumber m_AssemblyVersion;

private:
  friend class hubs::HubConnection;

private:
  std::shared_ptr<transports::IClientTransport> m_pClientTransport;
  TimeDelta m_DisconnectTimeout;
  TimeDelta m_TotalTransportConnectTimeout;
  ConnectionState m_State;
  KeepAliveData m_KeepAliveData;
  TimeDelta m_ReconnectWindow;
  QtPromise::QPromise<void> m_ConnectTask;
  //textwriter for trace.
  QString m_ConnectionData;
  //TaskQueue
  //TaskQueueMonitor.
  QtPromise::QPromise<void> m_LastQueuedReceiveTask;
  //DispatchingTaskCompletionSource.
  std::recursive_mutex m_StateLock;
  std::mutex m_StartLock;
  //tracemutex
  QDateTime m_LastMessageAt;
  QDateTime m_LastActiveAt;
  std::shared_ptr<QJsonSerializer> m_pJsonSerializer;
  QSslConfiguration m_CertCollection;
  QString m_UserUrl;
  QString m_ActualUrl;
  QString m_UserQueryString;
  QString m_ActualQueryString;
  std::shared_ptr<HeartBeatMonitor> m_pMonitor;
  TimeDelta m_TransportConnectTimeout;
  TimeDelta m_DeadlockErrorTimeout;
  QException m_LastError;
  QVersionNumber m_Protocol;
  QString m_MessageId;
  QString m_GroupsToken;
  QHash<QString, QObject*> m_Items;
  QString m_ConnectionId;
  QString m_ConnectionToken;
  QNetworkProxy m_Proxy;
  QList<QPair<QString, QString>> m_Headers;
  QAuthenticator m_Credentials;
  QList<QNetworkCookie> m_CookieContainer;
  static bool m_IsRegistered;
};

}
#endif //CONNECTION_HPP
