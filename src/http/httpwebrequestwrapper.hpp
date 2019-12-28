#ifndef HTTPWEBREQUESTWRAPPER_HPP
#define HTTPWEBREQUESTWRAPPER_HPP

#include <QPair>
#include <QList>
#include <QNetworkProxy>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkCookieJar>

#include "http/irequest.hpp"

namespace signalr::http {

class HttpWebRequestWrapper : public IRequest {

  HttpWebRequestWrapper(QNetworkRequest &request);

  void setRequestHeaders(QList<QPair<QString, QString> > &headers) override;
  void abort() override;

  QString getUserAgent() const override;
  void setUserAgent(QString userAgent) override;

  QString getAccept() const override;
  void setAccept(QString accept) override;

  QAuthenticator getCredential() const;
  void setCredential(QAuthenticator credential);

  std::shared_ptr<QNetworkCookieJar> getCookieContainer() const;
  void setCookieContainer(std::shared_ptr<QNetworkCookieJar> cookieContainer);

  void setClientCertificates(QSslConfiguration certificate);

private:
  QNetworkRequest m_Request;
  QString m_UserAgent;
  QAuthenticator m_Credential;
  std::shared_ptr<QNetworkCookieJar> m_CookieContainer;
  QString m_Accept;
  QNetworkProxy m_Proxy;
  QHash<QString, std::function<void(const QNetworkRequest&, const QString&)>> m_RestrictedHeaderSet;
};

}

#endif //HTTPWEBREQUESTWRAPPER_HPP
