#ifndef QSendDataH
#define QSendDataH 1

#include <QObject>
#include <QString>
#include <QUrl>
#include <QByteArray>
#include "winExport.h"

sdblibEXPORT QByteArray AWSEncode( const QString &src );
sdblibEXPORT QByteArray AWSEncode( const QByteArray &src );

class QSendDataPrivate;

class sdblibEXPORT QSendData : public QObject
{
  Q_OBJECT
public:
  QSendData( QObject * parent = 0 );
  virtual ~QSendData( void );

  virtual void setAmazonCredinals( const QUrl &server, const QString &AKID, const QString &SAK );
  virtual void addPair( const QString &key, const QString &value );
  virtual void clear( void );
  virtual void sendRequest( void );

signals:
  void finished( const QString &result_text );

protected:
  QUrl server;
  QString AKID;
  QString SAK;

private:
  QSendDataPrivate * store;
};

#endif
