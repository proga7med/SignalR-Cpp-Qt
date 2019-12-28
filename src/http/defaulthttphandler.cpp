#include "defaulthttphandler.hpp"

#include <QNetworkReply>
#include <QNetworkCookieJar>

namespace signalr::http {

DefaultHttpHandler::DefaultHttpHandler(std::shared_ptr<signalr::IConnection> pIConnection, QNetworkRequest& request, std::shared_ptr<QNetworkAccessManager> pHttpClient) {
  if(pIConnection == nullptr) throw QException();
  m_pIConnection = std::move(pIConnection);

  setCredential(pHttpClient);
  setCookies(pHttpClient);
  setProxy(pHttpClient);
  setClientCertificate(request);
}

void DefaultHttpHandler::setCredential(std::shared_ptr<QNetworkAccessManager> pHttpClient) {
  QObject::connect(pHttpClient.get(), &QNetworkAccessManager::authenticationRequired, [&](QNetworkReply* reply, QAuthenticator* authenticator) {
    if(authenticator != nullptr) delete authenticator; //To prevent memory leak.
    authenticator = new QAuthenticator(m_pIConnection->getCredentials());
  });
}

void DefaultHttpHandler::setClientCertificate(QNetworkRequest &request) {
  request.setSslConfiguration(m_pIConnection->getCertificate());
}

void DefaultHttpHandler::setCookies(std::shared_ptr<QNetworkAccessManager> pHttpClient) {
  QNetworkCookieJar* jar = new QNetworkCookieJar();
  for(auto const& cookie : m_pIConnection->getCookieContainer())
    jar->insertCookie(cookie);

  pHttpClient->setCookieJar(jar);
}

void DefaultHttpHandler::setProxy(std::shared_ptr<QNetworkAccessManager> pHttpClient) {
  pHttpClient->setProxy(m_pIConnection->getProxy());
}

}
