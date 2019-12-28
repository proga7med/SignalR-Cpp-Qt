#include "signalr/transports/websockettransport.hpp"

#include "infra/urlbuilder.hpp"
#include "signalr/iconnection.hpp"
#include "http/defaulthttpclient.hpp"
#include "http/httpwebrequestwrapper.hpp"
#include "http/httprequestmessagewrapper.hpp"
#include "websockets/clientwebsockethandler.hpp"
#include "websockets/websocketwrapperrequest.hpp"

namespace signalr {
namespace transports {

WebSocketTransport::WebSocketTransport()
    : WebSocketTransport(std::make_shared<http::DefaultHttpClient>()) {
}

WebSocketTransport::WebSocketTransport(std::shared_ptr<http::IHttpClient> pHttpClient)
    : ClientTransportBase(std::move(pHttpClient), QString("webSockets")){
  m_ReconnectDelay = TimeDelta::FromSeconds(2);
  m_pWebSocket = std::make_shared<QWebSocket>();
  m_pHandler = std::make_shared<websockets::ClientWebSocketHandler>(std::shared_ptr<WebSocketTransport>(this));
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

  QNetworkRequest request = QNetworkRequest(QUrl(uri));
  auto webSocketWrapperRequest = std::dynamic_pointer_cast<http::IRequest>(std::make_shared<websockets::WebSocketWrapperRequest>(m_pWebSocket, request, m_pConnection));
  m_pConnection->prepareRequest(webSocketWrapperRequest);

  return m_pHandler->openAsync(request);
}

QtPromise::QPromise<void> WebSocketTransport::send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) {
  if(pConnection == nullptr) throw QException();
  if(!m_pWebSocket->isValid()) throw QException();

  return m_pHandler->sendTextMessageAsync(data);
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
  m_pWebSocket->close(QWebSocketProtocol::CloseCode::CloseCodeAbnormalDisconnection);
}

}
}
