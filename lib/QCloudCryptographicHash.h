#ifndef QCloudCryptographicHashH
#define QCloudCryptographicHashH 1

#include <QObject>
#include <QByteArray>
#include "winExport.h"

class sdblibEXPORT QCloudCryptographicHash : public QObject
{
  Q_OBJECT
public:
  static QByteArray sha256_hash( const QByteArray &data );
};

#endif
