#ifndef SUBCRIPTION_HPP
#define SUBCRIPTION_HPP

#include <QList>
#include <QObject>
#include <QJsonValue>

namespace signalr {
namespace hubs {

class Subscription : public QObject {
  Q_OBJECT
public:
  Subscription() = default;

  void onReceived(const QJsonArray &data);

signals:
  void received(const QJsonArray&);
};

}
}




#endif //SUBCRIPTION_HPP
