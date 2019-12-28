#ifndef DEFAULTHTTPHANDLER_HPP
#define DEFAULTHTTPHANDLER_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include "signalr/iconnection.hpp"

namespace signalr::http {

class DefaultHttpHandler {
public:
  explicit DefaultHttpHandler(std::shared_ptr<IConnection> pIConnection, QNetworkRequest &request, std::shared_ptr<QNetworkAccessManager> pHttpClient);

private:
  void setCredential(std::shared_ptr<QNetworkAccessManager> pHttpClient);
  void setClientCertificate(QNetworkRequest& request);
  void setCookies(std::shared_ptr<QNetworkAccessManager> pHttpClient);
  void setProxy(std::shared_ptr<QNetworkAccessManager> pHttpClient);

private:
  std::shared_ptr<IConnection> m_pIConnection;
};

}

#endif // DEFAULTHTTPHANDLER_HPP
