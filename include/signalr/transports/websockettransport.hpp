#ifndef WEBSOCKETTRANSPORT_HPP
#define WEBSOCKETTRANSPORT_HPP

#include <QtWebSockets/QWebSocket>

#include <QtPromise>

#include "transports/clienttransportbase.hpp"

namespace signalr {
namespace transports {

class WebSocketTransport : public ClientTransportBase {
public:
  WebSocketTransport() = default;
  WebSocketTransport(std::shared_ptr<http::IHttpClient> pHttpClient);
  virtual QtPromise::QPromise<void> performConnect();
  virtual QtPromise::QPromise<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) override;
  virtual void lostConnection(std::shared_ptr<IConnection> pConnection) override;

  virtual bool isKeepAliveSupported() const override;

  void onMessage(const QString& message);
  void onOpen();
  void onClose();
  void doReconnect();
  void onError(const QException& error);

  virtual ~WebSocketTransport() = default;

protected:
  virtual void onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData) override;
  virtual void onStartFailed() override;

private:
  virtual QtPromise::QPromise<void> performConnect(const QString& url);

private:
  QWebSocket m_WebSocket;
  QString m_ConnectionData;
  std::shared_ptr<IConnection> m_pConnection;
  TimeDelta m_ReconnectDelay;
};

}
}

#endif //WEBSOCKETTRANSPORT_HPP
