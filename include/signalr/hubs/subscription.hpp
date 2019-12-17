#ifndef SUBCRIPTION_HPP
#define SUBCRIPTION_HPP

#include <QList>
#include <QObject>
#include <QJsonValue>

namespace signalr {
namespace hubs {

class Subscription /*: public QObject*/ {
//    Q_OBJECT
public:
    Subscription() = default;

    void onReceived(const QList<QJsonValue> &data);

//signals:
//    void received(const QList<QJsonValue>&);
};


}
}




#endif //SUBCRIPTION_HPP
