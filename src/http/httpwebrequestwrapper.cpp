#include "httpwebrequestwrapper.hpp"

#include <utility>

#include <QException>

namespace signalr::http {

HttpWebRequestWrapper::HttpWebRequestWrapper(QNetworkRequest& request) {
  m_Request = request;
}

void HttpWebRequestWrapper::setRequestHeaders(QList<QPair<QString, QString>> &headers) {
  for(auto const& header : headers) {
    if(!m_RestrictedHeaderSet.keys().contains(header.first)) {
        m_Request.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
    }
    else {
      std::function<void(const QNetworkRequest&, const QString&)> setHeaderAction;
      if(m_RestrictedHeaderSet.contains(header.first)) {
         setHeaderAction = m_RestrictedHeaderSet[header.first];
         setHeaderAction(m_Request, header.second);
      }
    }

  }
}

void HttpWebRequestWrapper::abort() {
  //
}

QString HttpWebRequestWrapper::getUserAgent() const {
  return m_UserAgent;
}

void HttpWebRequestWrapper::setUserAgent(QString userAgent) {
  m_UserAgent = std::move(userAgent);
}

QString HttpWebRequestWrapper::getAccept() const {
  return m_Accept;
}

void HttpWebRequestWrapper::setAccept(QString accept) {
  m_Accept = std::move(accept);
}

QAuthenticator HttpWebRequestWrapper::getCredential() const {
  return m_Credential;
}

void HttpWebRequestWrapper::setCredential(QAuthenticator credential) {
  m_Credential = std::move(credential);
}

std::shared_ptr<QNetworkCookieJar> HttpWebRequestWrapper::getCookieContainer() const {
  return m_CookieContainer;
}

void HttpWebRequestWrapper::setCookieContainer(std::shared_ptr<QNetworkCookieJar> cookieContainer) {
  m_CookieContainer = std::move(cookieContainer);
}

void HttpWebRequestWrapper::setClientCertificates(QSslConfiguration certificate) {
  if(certificate.isNull()) throw QException();
  m_Request.setSslConfiguration(certificate);
}

}
