#include "httpresponsemessagewrapper.hpp"

namespace signalr {
namespace http {

HttpResponseMessageWrapper::HttpResponseMessageWrapper(QNetworkReply *pNetworkReply) {
   m_pNetworkReply = pNetworkReply;
}

QString HttpResponseMessageWrapper::readAsString() {
  return m_pNetworkReply->readAll();
}

QDataStream HttpResponseMessageWrapper::getStream() const {
  return m_pNetworkReply->readAll();
}

}
}
