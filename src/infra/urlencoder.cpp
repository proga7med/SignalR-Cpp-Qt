#include "urlencoder.hpp"

#include <QException>

namespace signalr::infra {

QString UrlEncoder::urlEncode(const QString& str) {
  if(str.isNull())
    return nullptr;

  QByteArray bytes = str.toUtf8();
  QByteArray encodedBytes = urlEncode(bytes, 0, bytes.length());
  return QString::fromUtf8(encodedBytes.data(), encodedBytes.length());
}

QByteArray UrlEncoder::urlEncode(QByteArray bytes, int offset, int count) {
  if(!validateUrlEncodingParameters(bytes, offset, count)) return nullptr;

  int cSpaces = 0;
  int cUnsafe = 0;

  for (int i = 0; i < count; i++) {
    char ch = (char)bytes[offset + i];
    if (ch == ' ') cSpaces++;
    else if (!isUrlSafeChar(ch))
        cUnsafe++;
  }

  if (cSpaces == 0 && cUnsafe == 0)
     return bytes;

  QByteArray expandedBytes = QByteArray(count + cUnsafe * 2, ' ');
  int pos = 0;

  for (int i = 0; i < count; i++) {
      QByteArray dummy = QByteArray(" ");
      QByteRef b = dummy[0];
      char ch = b;

      if (isUrlSafeChar(ch)) {
        expandedBytes[pos++] = b;
      }
      else if(ch == ' ') {
        expandedBytes[pos++] = '+';
      }
      else {
        expandedBytes[pos++] = '%';
        expandedBytes[pos++] = intToHex((b >> 4) & 0xf);
        expandedBytes[pos++] = intToHex(b & 0x0f);
      }
  }

  return expandedBytes;
}

char UrlEncoder::intToHex(int n) {
  if (n <= 9) return (char)(n + (int)'0');
  else return (char)(n - 10 + (int)'a');
}

bool UrlEncoder::isUrlSafeChar(char ch) {
  if ((ch >= 'a' && ch <= 'z') || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9')
    return true;

  switch (ch) {
    case '-':
    case '_':
    case '.':
    case '!':
    case '*':
    case '(':
    case ')':
      return true;
  }

  return false;
}

bool UrlEncoder::validateUrlEncodingParameters(QByteArray bytes, int offset, int count) {
  if(bytes.isNull() && count == 0) return false;
  if(bytes.isNull()) throw QException();
  if(offset < 0 || offset > bytes.length()) throw QException();
  if(count < 0 || offset + count > bytes.length()) throw QException();
  return true;
}

}
