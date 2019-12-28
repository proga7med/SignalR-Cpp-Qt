#include "httprequestmessagewrapper.hpp"

#include <QHash>
#include <QException>

namespace signalr {
namespace http {

HttpRequestMessageWrapper::HttpRequestMessageWrapper(QNetworkRequest& networkRequest) {
  m_NetworkRequest = networkRequest;
}

QString HttpRequestMessageWrapper::getUserAgent() const {
  return m_UserAgent;
}

void HttpRequestMessageWrapper::setUserAgent(QString userAgent) {
  m_UserAgent = std::move(userAgent);
}

QString HttpRequestMessageWrapper::getAccept() const {
  return m_Accept;
}
	
void HttpRequestMessageWrapper::setAccept(QString accept) {
  m_Accept = std::move(accept);
}

void HttpRequestMessageWrapper::abort() {
  emit cancel();
}

void HttpRequestMessageWrapper::setRequestHeaders(QList<QPair<QString, QString>>& headers) {
  if(!m_UserAgent.isNull())
    m_NetworkRequest.setRawHeader("User-Agent", m_UserAgent.toLocal8Bit());

  if(!m_Accept.isNull())
    m_NetworkRequest.setRawHeader("Accept", m_Accept.toLocal8Bit());

  for (auto const& header : headers) {
    m_NetworkRequest.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
  }
}

}
}
