#ifndef CONNECTIONSTATE_HPP
#define CONNECTIONSTATE_HPP

enum class ConnectionState {
  Disconnected = 0,
  Connecting = 1,
  Connected = 2,
  Reconnecting = 3,
  Disconnecting = 4
};

#endif //CONNECTIONSTATE_HPP
