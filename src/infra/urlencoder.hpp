#ifndef URLENCODER_HPP
#define URLENCODER_HPP

#include <QString>

namespace signalr::infra {

class UrlEncoder {
public:
  static QString urlEncode(const QString& str);

private:
  static QByteArray urlEncode(QByteArray bytes, int offset, int count);
  static bool validateUrlEncodingParameters(QByteArray bytes, int offset, int count);
  static char intToHex(int n);
  static bool isUrlSafeChar(char ch);
};

}

#endif //URLENCODER_HPP
