#ifndef IREQUEST_HPP
#define IREQUEST_HPP

#include <QString>
#include <QPair>

namespace signalr {
namespace http {

class IRequest {
public:
   virtual void abort() = 0;
   virtual void setRequestHeaders(QList<QPair<QString, QString>>& headers) = 0;
   virtual ~IRequest() = default;

   virtual QString getUserAgent() const { return m_UserAgent; }
   virtual void setUserAgent(QString userAgent) { m_UserAgent = std::move(userAgent); }

   virtual QString getAccept() const { return m_Accept; }
   virtual void setAccept(QString accept) { m_Accept = std::move(accept); }

protected:
   QString m_UserAgent;
   QString m_Accept;
};

}
}

#endif //IREQUEST_HPP
