#ifndef HUBINVOCATION_HPP
#define HUBINVOCATION_HPP

#include <QHash>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QJsonSerializer>
#include <QJsonObject>
#include <QJsonDocument>

namespace signalr {
namespace hubs {

class HubInvocation {
  Q_GADGET
public:
   Q_INVOKABLE HubInvocation(QObject *parent = nullptr){}

  Q_PROPERTY(QString I READ getCallbackId WRITE setCallbackId MEMBER m_CallbackId);
  Q_PROPERTY(QString H READ getHub WRITE setHub MEMBER m_Hub);
  Q_PROPERTY(QString M READ getMethod WRITE setMethod MEMBER m_Method);
  Q_PROPERTY(QJsonArray A READ getArgs WRITE setArgs MEMBER m_Args);
  Q_PROPERTY(QMap<QString, QJsonDocument> S READ getState WRITE setState MEMBER m_State);

public:
  QString getCallbackId() const;
  void setCallbackId(QString callbackId);

  QString getHub() const;
  void setHub(QString hub);

  QString getMethod() const;
  void setMethod(QString method);

  QJsonArray getArgs() const;
  void setArgs(QJsonArray args);

  QMap<QString, QJsonDocument> getState() const;
  void setState(QMap<QString, QJsonDocument> state);

private:
  QString m_CallbackId;
  QString m_Hub;
  QString m_Method;
  QJsonArray m_Args;
  QMap<QString, QJsonDocument> m_State;
};

}
}
Q_DECLARE_METATYPE(signalr::hubs::HubInvocation)

#endif //HUBINVOCATION_HPP
