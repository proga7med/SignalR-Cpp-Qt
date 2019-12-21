#ifndef HUBCONNECTION_HPP
#define HUBCONNECTION_HPP

#include <mutex>

#include <QHash>

#include "connection.hpp"
#include "hubs/hubproxy.hpp"
#include "signalr/hubs/ihubconnection.hpp"

namespace signalr {
namespace hubs {

class HubConnection : public IHubConnection, public Connection, std::enable_shared_from_this<HubConnection> {
  Q_OBJECT
public:
  HubConnection() = default;
  HubConnection(QString url);
  HubConnection(QString url, bool useDefaultUrl);
  HubConnection(QString url, QString queryString);
  HubConnection(QString url, QString queryString, bool useDefaultUrl);
  HubConnection(QString url, QList<QPair<QString, QString> > queryString);

  virtual std::shared_ptr<IHubProxy> createHubProxy(const QString &hubName) override;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<http::IHttpClient> pHttpClient) override;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<transports::IClientTransport> pClientTransport) override;
  virtual void stop() override;
  virtual void stop(const TimeDelta& timeout);
  virtual void removeCallback(const QString& callbackId) override;
  //virtual void setClientCertificate(QSslConfiguration configuration) override;
  virtual ~HubConnection() = default;

protected:
  virtual QString onSending() override;
  virtual void onMessageReceived(const QJsonDocument &message) override;
  virtual void onReceived(const QJsonDocument& data) override;
  virtual void onError(const QException& error) override;
  virtual void onReconnecting() override;
  virtual void onReconnected() override;
  virtual void onConnectionSlow() override;

private:
  static QString getUrl(QString &url, bool useDefaultUrl);

private:
  QString m_Url;
  std::mutex m_Mutex;
  int m_callbackId;
  QHash<QString, std::shared_ptr<HubProxy>> m_Hubs;
  QHash<QString, std::function<void(HubResult)>> m_callback;
};

} //end hubs
} //end signalr

#endif //HUBCONNECTION_HPP
