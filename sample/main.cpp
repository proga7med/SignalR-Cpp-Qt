#include <QCoreApplication>
#include <QFile>
#include <QSslKey>
#include <QJsonSerializer>

#include <memory>

#include <QObject>
#include <signalr/hubs/hubconnection.hpp>
#include <signalr/hubs/ihubconnection.hpp>
#include <signalr/transports/websockettransport.hpp>

QString stateAsString(ConnectionState state) {
  switch (state) {
     case ConnectionState::Connected:
      return "Connected";
  case ConnectionState::Connecting:
      return "Connecting";
  case ConnectionState::Disconnected:
      return "Disconnected";
  case ConnectionState::Reconnecting:
      return "Reconnecting";
  case ConnectionState::Disconnecting:
      return "Disconnecting";
  default:
      return "Unknown";
  }
}

void broadcastMessage(const QString& name, const QString& message) {
  qDebug() << name << " " << message;
}

int main(int argc, char** argv) {
  QCoreApplication a(argc, argv);

  std::shared_ptr<signalr::hubs::IHubConnection> hubConnection
            = std::make_shared<signalr::hubs::HubConnection>("http://localhost:49781/");

  auto hubProxy = std::dynamic_pointer_cast<signalr::hubs::HubProxy>(hubConnection->createHubProxy("ChatHub"));
  auto transport = std::make_shared<signalr::transports::WebSocketTransport>();

  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::received, [&](const QString& message){ qDebug() << message; });
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::error, [&] (const QException& error) { qDebug() << error.what(); });
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::closed, [&]() {});
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::reconnecting, [&] () { qDebug() << "Reconnecting"; });
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::stateChanged, [&](ConnectionState oldState, ConnectionState newState){  qDebug() << "Old State" << stateAsString(oldState) << "New State" << stateAsString(newState); });
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::connectionSlow, [&]() {});
  QObject::connect(hubConnection.get(), &signalr::hubs::IHubConnection::reconnected, [&]() { qDebug() << "Connected"; });

  hubProxy->on<QString, QString>(hubProxy, "broadcastMessage", &broadcastMessage);

  hubConnection->start(transport).wait().then([&](){
      QString name = "Sam";
      QString message = "Hi there!";
      QVariantList args;
      args << name << message;
      hubProxy->invoke<void, int>("Send", [&](int pre){ qDebug() << QString("{0}% complete").arg(QString::number(pre)); }, args);
  });

  return a.exec();
}
