#include "clientwebsockethandler.hpp"


namespace signalr {
namespace transports {
namespace websockets {

ClientWebSocketHandler::ClientWebSocketHandler(std::shared_ptr<WebSocketTransport> pWebSocketTransport) {
  m_pWebSocketTransport = std::move(pWebSocketTransport);
  connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::connected, this, &ClientWebSocketHandler::onConnected);
  connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::disconnected, this, &ClientWebSocketHandler::onDisconnected);
  connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::textMessageReceived, this, &ClientWebSocketHandler::onTextMessageReceived);
  connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::stateChanged, this, &ClientWebSocketHandler::onStateChanged);
  connect(m_pWebSocketTransport->m_pWebSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &ClientWebSocketHandler::onError);
}

QtPromise::QPromise<void> ClientWebSocketHandler::sendTextMessageAsync(const QString& message) {
  return QtPromise::QPromise<void>([=](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject) {
    QtPromise::connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::bytesWritten, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error))
               .then([=](){ onTextMessageReceived(message); })
               .fail([=](QAbstractSocket::SocketError error) { onError(error); });

    m_pWebSocketTransport->m_pWebSocket->sendTextMessage(message);
  });
}

QtPromise::QPromise<void> ClientWebSocketHandler::openAsync(const QNetworkRequest& request) {
  return QtPromise::QPromise<void>([=](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject) {
     QtPromise::connect(m_pWebSocketTransport->m_pWebSocket.get(), &QWebSocket::connected, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error))
             .then([=](){ onConnected(); })
             .fail([=](QAbstractSocket::SocketError error) { onError(error); });

     m_pWebSocketTransport->m_pWebSocket->open(request);
  });
}

void ClientWebSocketHandler::onConnected() {
  m_pWebSocketTransport->onOpen();
}

void ClientWebSocketHandler::onDisconnected() {
  m_pWebSocketTransport->onClose();
}

void ClientWebSocketHandler::onTextMessageReceived(const QString& message) {
  m_pWebSocketTransport->onMessage(message);
}

void ClientWebSocketHandler::onError(QAbstractSocket::SocketError error) {
  m_pWebSocketTransport->onError(QException());
}

void ClientWebSocketHandler::onStateChanged(QAbstractSocket::SocketState state) {
  //m_pWebSocketTransport->on
}

}
}
}
