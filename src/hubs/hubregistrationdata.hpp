#ifndef HUBREGISTRATIONDATA_HPP
#define HUBREGISTRATIONDATA_HPP

#include <QObject>
#include <QString>
#include <QJsonSerializer>

namespace signalr {
namespace hubs {

class HubRegistrationData {
  Q_GADGET
public:
  Q_INVOKABLE HubRegistrationData(QObject* parent = nullptr) {
    QJsonSerializer::registerListConverters<HubRegistrationData>();
  }
  Q_PROPERTY(QString Name READ getName WRITE setName MEMBER m_Name)

public:
  QString getName() const;
  void setName(QString name);

private:
  QString m_Name;
};

}
}
Q_DECLARE_METATYPE(signalr::hubs::HubRegistrationData)


#endif //HUBREGISTRATIONDATA_HPP
