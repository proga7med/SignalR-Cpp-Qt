#ifndef HUBEXCEPTION_HPP
#define HUBEXCEPTION_HPP

#include <QObject>
#include <QVariant>
#include <QException>

namespace signalr {

class HubException : public QException {
public:
  HubException() = default;
  HubException(QString message,  QVariant errorData) {
    m_Message = message;
    m_ErrorData = errorData;
  }

  virtual void raise() const {
    throw *this;
  }

  virtual QException *clone() const {
    return new HubException(*this);
  }

  virtual char const* what() const {
    return const_cast<const char*>(m_Message.toLatin1().data());
  }

private:
  QString m_Message;
  QVariant m_ErrorData;
};

}

#endif //HUBEXCEPTION_HPP
