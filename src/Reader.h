#ifndef DISPAR_READER_H
#define DISPAR_READER_H

#include <QtGlobal>

class QIODevice;
class QByteArray;

class Reader {
public:
  Reader(QIODevice &dev, bool littleEndian = true);

  bool isLittleEndian() const;
  void setLittleEndian(bool little);

  /// Read 2 bytes = 16 bits.
  quint16 getUInt16(bool *ok = nullptr);

  /// Read 4 bytes = 32 bits.
  quint32 getUInt32(bool *ok = nullptr);

  /// Read 8 bytes = 64 bits.
  quint64 getUInt64(bool *ok = nullptr);

  char getChar(bool *ok = nullptr);
  unsigned char getUChar(bool *ok = nullptr);
  char peekChar(bool *ok = nullptr);
  unsigned char peekUChar(bool *ok = nullptr);

  QByteArray read(qint64 max);

  qint64 pos() const;
  bool seek(qint64 pos);
  bool atEnd() const;

  bool peekList(std::initializer_list<unsigned char> list);

private:
  template <typename T>
  T getUInt(bool *ok = nullptr);

  QIODevice &dev;
  bool littleEndian;
};

#endif // DISPAR_READER_H
