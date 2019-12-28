#ifndef HUBPROGRESSUPDATE_HPP
#define HUBPROGRESSUPDATE_HPP

#include <QObject>
#include <QString>
#include <QJsonDocument>

namespace signalr {
namespace hubs {

class HubProgressUpdate {
  Q_GADGET
public:
  Q_INVOKABLE HubProgressUpdate(){}
  Q_PROPERTY(QString I MEMBER m_Id);
  Q_PROPERTY(QJsonDocument D MEMBER m_Data);

public:
  QString getId() const;
  void setId(QString id);

  QJsonDocument getData() const;
  void setData(QJsonDocument data);

private:
  QString m_Id;
  QJsonDocument m_Data;
};

} //end hubs
} //end signalr
Q_DECLARE_METATYPE(signalr::hubs::HubProgressUpdate)

#endif //HUBPROGRESSUPDATE_HPP
