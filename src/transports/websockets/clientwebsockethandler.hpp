#ifndef CLIENTWEBSOCKETHANDLER_HPP
#define CLIENTWEBSOCKETHANDLER_HPP

#include <memory>

#include <QtPromise>
#include <QWebSocket>

#include "signalr/transports/websockettransport.hpp"

namespace signalr::transports::websockets {

class ClientWebSocketHandler : public QObject {
  Q_OBJECT
public:
  ClientWebSocketHandler(std::shared_ptr<WebSocketTransport> pWebSocketTransport);

public:
  QtPromise::QPromise<void> sendTextMessageAsync(const QString& message);
  QtPromise::QPromise<void> openAsync(const QNetworkRequest &request);

private slots:
  void onConnected();
  void onDisconnected();
  void onTextMessageReceived(const QString& message);
  void onError(QAbstractSocket::SocketError error);
  void onStateChanged(QAbstractSocket::SocketState state);

private:
  std::shared_ptr<WebSocketTransport> m_pWebSocketTransport;
};

}

#endif //CLIENTWEBSOCKETHANDLER_HPP
