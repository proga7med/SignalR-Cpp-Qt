#include "signalr/hubs/hubprogressupdate.hpp"

namespace signalr {
namespace hubs {

QString HubProgressUpdate::getId() const {
  return m_Id;
}

void HubProgressUpdate::setId(QString id) {
  m_Id = std::move(id);
}

QJsonDocument HubProgressUpdate::getData() const {
  return m_Data;
}

void HubProgressUpdate::setData(QJsonDocument data) {
  m_Data = std::move(data);
}

}
}
