#ifndef WEBSOCKETWRAPPERREQUEST_HPP
#define WEBSOCKETWRAPPERREQUEST_HPP

#include <QWebSocket>
#include <QNetworkCookie>
#include <QAuthenticator>

#include "http/irequest.hpp"
#include "signalr/iconnection.hpp"


namespace signalr::transports::websockets {

class WebSocketWrapperRequest : public http::IRequest {
public:
  WebSocketWrapperRequest(std::shared_ptr<QWebSocket> pWebSocket, QNetworkRequest &request, std::shared_ptr<IConnection> pIConnection);

  void setRequestHeaders(QList<QPair<QString, QString> > &headers) override;

  QString getAccept() const override;
  void setAccept(QString accept) override;

  QString getUserAgent() const override;
  void setUserAgent(QString userAgent) override;

  void abort() override;

  QAuthenticator getCredential() const;
  void setCredentials(QAuthenticator credential);

  QList<QNetworkCookie> getCookieContainer() const;
  void setCookieContainer(QList<QNetworkCookie> cookieContainer);

  QNetworkProxy getProxy() const;
  void setProxy(QNetworkProxy proxy);

  void setClientCertificate(QSslConfiguration configuration);

  void prepareRequest();

  QNetworkRequest getNetworkRequest() const;

private:
  QAuthenticator m_Credential;
  std::shared_ptr<QWebSocket> m_pWebSocket;
  std::shared_ptr<IConnection> m_pIConnection;
  QNetworkRequest m_Request;
};

}

#endif //WEBSOCKETWRAPPERREQUEST_HPP
