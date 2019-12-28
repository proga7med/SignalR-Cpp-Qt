#ifndef WEBSOCKETTRANSPORT_HPP
#define WEBSOCKETTRANSPORT_HPP

#include <QtWebSockets/QWebSocket>

#include <QtPromise>

#include "transports/clienttransportbase.hpp"

namespace signalr {
namespace transports {

namespace websockets {
  class ClientWebSocketHandler;
}

class WebSocketTransport : public ClientTransportBase, public std::enable_shared_from_this<WebSocketTransport>  {
public:
  WebSocketTransport();
  WebSocketTransport(std::shared_ptr<http::IHttpClient> pHttpClient);

public:
  virtual QtPromise::QPromise<void> performConnect();
  virtual QtPromise::QPromise<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) override;
  virtual void lostConnection(std::shared_ptr<IConnection> pConnection) override;
  virtual bool isKeepAliveSupported() const override;
  virtual ~WebSocketTransport() = default;

public:
  void onMessage(const QString& message);
  void onOpen();
  void onClose();
  void doReconnect();
  void onError(const QException& error);

protected:
  virtual void onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData) override;
  virtual void onStartFailed() override;

private:
  virtual QtPromise::QPromise<void> performConnect(const QString& url);

private:
  friend class websockets::ClientWebSocketHandler;

private:
  std::shared_ptr<QWebSocket> m_pWebSocket;
  QString m_ConnectionData;
  std::shared_ptr<IConnection> m_pConnection;
  TimeDelta m_ReconnectDelay;
  std::shared_ptr<websockets::ClientWebSocketHandler> m_pHandler;
};

}
}

#endif //WEBSOCKETTRANSPORT_HPP
