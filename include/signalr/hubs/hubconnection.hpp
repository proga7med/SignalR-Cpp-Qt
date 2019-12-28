#ifndef HUBCONNECTION_HPP
#define HUBCONNECTION_HPP

#include <mutex>

#include <QHash>

#include "connection.hpp"
#include "hubs/hubproxy.hpp"
#include "signalr/hubs/ihubconnection.hpp"

namespace signalr {
namespace hubs {

class HubConnection : public virtual Connection, public virtual IHubConnection {
  Q_OBJECT
public:
  HubConnection() = default;
  HubConnection(QString url);
  HubConnection(QString url, bool useDefaultUrl);
  HubConnection(QString url, QString queryString);
  HubConnection(QString url, QString queryString, bool useDefaultUrl);
  HubConnection(QString url, QList<QPair<QString, QString> > queryString);
  HubConnection(QString url, QList<QPair<QString, QString> > queryString, bool useDefaultUrl);

  virtual std::shared_ptr<IHubProxy> createHubProxy(const QString &hubName) override;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<http::IHttpClient> pHttpClient) override;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;
  virtual void stop() override;
  virtual void stop(const TimeDelta& timeout);
  virtual void removeCallback(const QString& callbackId) override;
  virtual bool changeState(ConnectionState oldState, ConnectionState newState) override;
  virtual void stop(const QException& error) override;
  virtual void disconnect() override;
  virtual QtPromise::QPromise<void> send(const QString& data) override;
  virtual void setClientCertificate(QSslConfiguration configuration) override;

  virtual void prepareRequest(std::shared_ptr<http::IRequest> pRequest) override;
  virtual void markLastMessage() override;
  virtual void markArchive() override;

  //virtual void trace

  virtual QVersionNumber getProtocol() const override;
  virtual void setProtocol(QVersionNumber protocol) override;

  virtual TimeDelta getTrasnportConnectTimeout() const override;
  virtual void setTransportConnectTimeout(TimeDelta transportConnectTimeout) override;

  virtual TimeDelta getTotalTransportConnectTimeout() const override;

  virtual TimeDelta getReconnectWindow() const override;
  virtual void setReconnectWindow(TimeDelta reconnectWindow) override;

  virtual KeepAliveData getKeepAliveData() const override;
  virtual void setKeepAliveData(KeepAliveData keepAliveData) override;

  virtual QString getMesssageId() const override;
  virtual void setMessageId(QString messageId) override;

  virtual QString getGroupsToken() const override;
  virtual void setGroupsToken(QString groupsToken) override;

  virtual QHash<QString, QObject*> getItems() const override;
  virtual QString getConnectionId() const override;
  virtual QString getConnectionToken() const override;
  virtual QString getUrl() const override;
  virtual QString getQueryString() const override;
  virtual ConnectionState getConnectionState() const override;
  virtual std::shared_ptr<transports::IClientTransport> getClientTransport() const override;
  virtual QDateTime getLastMessageAt() const override;
  virtual QDateTime getLastActiveAt() const override;

  virtual QNetworkProxy getProxy () const override;
  virtual void setProxy(QNetworkProxy proxy) override;

  virtual QSslConfiguration getCertificate() const override;
  virtual QList<QPair<QString, QString>> getHeaders() const override;

  virtual QAuthenticator getCredentials() const override;
  virtual void setCredentials(QAuthenticator credential) override;

  virtual QList<QNetworkCookie> getCookieContainer() const override;
  virtual std::shared_ptr<QJsonSerializer> getJsonSerializer() const override;

  virtual std::shared_ptr<IHubProxy> operator[] (const QString& hubName) override;

  virtual ~HubConnection();

protected:
  virtual QString onSending() override;
  virtual void onClosed() override;
  virtual void onMessageReceived(const QJsonValue &message) override;
  virtual void onReceived(const QJsonValue& data) override;
  virtual void onError(const QException& error) override;
  virtual void onReconnecting() override;
  virtual void onReconnected() override;
  virtual void onConnectionSlow() override;

private:
  static QString getUrl(QString &url, bool useDefaultUrl);
  void clearInvocationCallbacks(const QString& error);
  virtual QString registerCallback(std::function<void(HubResult)> callback) override;

private:
  friend class Connection;

private:
  QString m_Url;
  std::mutex m_Mutex;
  std::mutex m_CallbackMutex;
  int m_callbackId = 1;
  QHash<QString, std::shared_ptr<HubProxy>> m_Hubs;
  QHash<QString, std::function<void(HubResult)>> m_Callbacks;
};

} //end hubs
} //end signalr

#endif //HUBCONNECTION_HPP
