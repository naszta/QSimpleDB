#ifndef QHmacSha1H
#define QHmacSha1H 1

#include <QObject>
#include <QByteArray>
#include "winExport.h"

class sdblibEXPORT QHmacSha1 : public QObject
{
  Q_OBJECT
public:
  QHmacSha1( const QByteArray &password, QObject * parent = 0 );
  virtual ~QHmacSha1( void );

  static QByteArray CalcHmacSha1( const QByteArray &baseString, const QByteArray &key );
};

#endif
