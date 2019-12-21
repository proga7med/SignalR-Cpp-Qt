#include "signalr/transports/websockettransport.hpp"

#include "infra/urlbuilder.hpp"
#include "signalr/iconnection.hpp"
#include "http/httprequestmessagewrapper.hpp"

namespace signalr {
namespace transports {

WebSocketTransport::WebSocketTransport(std::shared_ptr<http::IHttpClient> pHttpClient)
    : ClientTransportBase(std::move(pHttpClient), QString("webSockets")){
  m_ReconnectDelay = TimeDelta::FromSeconds(2);
  //connect signals and slots.
}

bool WebSocketTransport::isKeepAliveSupported() const {
  return true;
}

void WebSocketTransport::onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData) {
  m_pConnection = pConnection;
  m_ConnectionData = connectionData;
  performConnect();
}

QtPromise::QPromise<void> WebSocketTransport::performConnect() {
  return performConnect(UrlBuilder::buildConnect(m_pConnection, getName(), m_ConnectionData));
}

QtPromise::QPromise<void> WebSocketTransport::performConnect(const QString& url) {
  auto uri = UrlBuilder::convertToWebSocketUri(url);
  //trace.
  //m_pConnection->prepareRequest(WebSocketWrapperRequest)
  return QtPromise::QPromise<void>([&](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject) {
     m_WebSocket.open(url);
  });

}

QtPromise::QPromise<void> WebSocketTransport::send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) {
  return QtPromise::QPromise<void>([&](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject) {
    if(pConnection == nullptr) throw QException();
    if(!m_WebSocket.isValid()) throw QException();

    m_WebSocket.sendTextMessage(data);
  });
}

void WebSocketTransport::onStartFailed() {
    //
}

void WebSocketTransport::onMessage(const QString& message) {
  //TODO: set log here.
  processResponse(m_pConnection, message);
}

void WebSocketTransport::onOpen() {
  if(m_pConnection->changeState(ConnectionState::Reconnecting, ConnectionState::Connected))
     m_pConnection->onReconnected();
}

void WebSocketTransport::onClose() {
  //TODO: set log here.
  doReconnect();
}

void WebSocketTransport::doReconnect() {
  auto reconnectUrl = UrlBuilder::buildReconnect(m_pConnection, getName(), m_ConnectionData);
}

void WebSocketTransport::onError(const QException& error) {
    //trace.
  m_pConnection->onError(error);
}

void WebSocketTransport::lostConnection(std::shared_ptr<IConnection> pConnection) {
   //trace
  m_WebSocket.close(QWebSocketProtocol::CloseCode::CloseCodeAbnormalDisconnection);
}

}
}
