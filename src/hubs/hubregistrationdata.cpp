#include "hubregistrationdata.hpp"

namespace signalr {
namespace hubs {

QString HubRegistrationData::getName() const {
  return m_Name;
}

void HubRegistrationData::setName(QString name) {
  m_Name = std::move(name);
}

}
}
