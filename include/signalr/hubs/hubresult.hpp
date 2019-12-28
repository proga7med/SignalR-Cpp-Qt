#ifndef HUBRESULT_HPP
#define HUBRESULT_HPP

#include <QString>
#include <QObject>
#include <QVariant>
#include <QJsonObject>

#include "signalr/hubs/hubprogressupdate.hpp"

namespace signalr {
namespace hubs {

class HubResult {
  Q_GADGET
public:
  Q_INVOKABLE HubResult(QObject *parent = nullptr) {}

  Q_PROPERTY(QString I READ getId WRITE setId MEMBER m_Id);
  Q_PROPERTY(HubProgressUpdate P READ getHubProgressUpdate WRITE setHubProgressUpdate MEMBER m_HubProgressUpdate);
  Q_PROPERTY(QJsonDocument R READ getResult WRITE setResult MEMBER m_Result);
  Q_PROPERTY(bool H READ isHubException WRITE setHubException MEMBER m_IsHubException);
  Q_PROPERTY(QString E READ getError WRITE setError MEMBER m_Error);
  Q_PROPERTY(QVariant D READ getErrorData WRITE setErrorData MEMBER m_ErrorData);
  Q_PROPERTY(QMap<QString, QJsonDocument> S READ getState WRITE setState MEMBER m_State);

public:
  QString getId() const;
  void setId(QString id);

  HubProgressUpdate getHubProgressUpdate() const;
  void setHubProgressUpdate(HubProgressUpdate hubProgressUpdate);

  QJsonDocument getResult() const;
  void setResult(QJsonDocument result);

  bool isHubException() const;
  void setHubException(bool hubException);

  QString getError() const;
  void setError(QString error);

  QVariant getErrorData() const;
  void setErrorData(QVariant errorData);

  QMap<QString, QJsonDocument> getState() const;
  void setState(QMap<QString, QJsonDocument> state);

private:
  QString m_Id;
  QString m_Error;
  bool m_IsHubException;
  QJsonDocument m_Result;
  HubProgressUpdate m_HubProgressUpdate;
  QVariant m_ErrorData;
  QMap<QString, QJsonDocument> m_State;
};

}
}
Q_DECLARE_METATYPE(signalr::hubs::HubResult)

#endif //HUBRESULT_HPP
