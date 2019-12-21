#ifndef ICONNECTION_HPP
#define ICONNECTION_HPP

#include <future>

#include <QDateTime>
#include <QNetworkProxy>
#include <QVersionNumber>
#include <QNetworkCookieJar>
#include <QSslConfiguration>
#include <QAuthenticator>

#include "http/irequest.hpp"
#include "keepalivedata.hpp"
#include "hubs/ihubproxy.hpp"
#include "connectionstate.hpp"
#include "transports/iclienttransport.hpp"

namespace signalr {

namespace http {
class IHttpClient;
}

class IConnection : public QObject {
  Q_OBJECT
public:

  virtual bool changeState(ConnectionState oldState, ConnectionState newState) = 0;

  /**
   * @brief start
   * @return
   */
  virtual QtPromise::QPromise<void> start(std::shared_ptr<http::IHttpClient> pHttpClient) = 0;

  /**
   * @brief start
   * @param pClientTransport
   * @return
   */
  virtual QtPromise::QPromise<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) = 0;

  /**
   * @brief stop
   */
  virtual void stop() = 0;

  /**
   * @brief stop
   * @param timeout
   */
  virtual void stop(const QException& error) = 0;

  virtual void disconnect() = 0;

  virtual QtPromise::QPromise<void> send(const QString& data) = 0;

  virtual void onReceived(const QJsonDocument &data) = 0;
  virtual void onError(const QException& error) = 0;
  virtual void onReconnecting() = 0;
  virtual void onReconnected() = 0;
  virtual void onConnectionSlow() = 0;

  virtual void prepareRequest(std::shared_ptr<http::IRequest> pRequest) = 0;
  virtual void markLastMessage() = 0;
  virtual void markArchive() = 0;

  //virtual void trace

public:
  virtual QVersionNumber getProtocol() const = 0;
  virtual void setProtocol(QVersionNumber protocol) = 0;

  virtual TimeDelta getTrasnportConnectTimeout() const = 0;
  virtual void setTransportConnectTimeout(TimeDelta transportConnectTimeout) = 0;

  virtual TimeDelta getTotalTransportConnectTimeout() const = 0;

  virtual TimeDelta getReconnectWindow() const = 0;
  virtual void setReconnectWindow(TimeDelta reconnectWindow) = 0;

  virtual KeepAliveData getKeepAliveData() const = 0;
  virtual void setKeepAliveData(KeepAliveData keepAliveData) = 0;

  virtual QString getMesssageId() const = 0;
  virtual void setMessageId(QString messageId) = 0;

  virtual QString getGroupsToken() const = 0;
  virtual void setGroupsToken(QString groupsToken) = 0;

  virtual QHash<QString, QObject*> getItems() const = 0;
  virtual QString getConnectionId() const = 0;
  virtual QString getConnectionToken() const = 0;
  virtual QString getUrl() const = 0;
  virtual QString getQueryString() const = 0;
  virtual ConnectionState getConnectionState() const = 0;
  virtual std::shared_ptr<transports::IClientTransport> getClientTransport() const = 0;
  virtual QDateTime getLastMessageAt() const = 0;
  virtual QDateTime getLastActiveAt() const = 0;

  virtual QNetworkProxy getProxy () const = 0;
  virtual void setProxy(QNetworkProxy proxy) = 0;

  virtual QList<QSslConfiguration> getCertificates() const = 0;
  virtual QList<QPair<QString, QString>> getHeaders() const = 0;

  virtual QAuthenticator getCredentials() const = 0;
  virtual void setCredentials(QAuthenticator credential) = 0;

  virtual std::shared_ptr<QNetworkCookieJar> getCookieContainer() const = 0;
  //JsonSerializer

  virtual ~IConnection() = default;

signals:
  void received(const QString& message);
  void error(const QException& error);
  void closed();
  void reconnecting();
  void reconnected();
  void stateChanged(ConnectionState oldState, ConnectionState newState);
  void connectionSlow();
};

}
Q_DECLARE_INTERFACE(signalr::IConnection, "IConnection")

#endif //ICONNECTION_HPP
