#ifndef QAwsXmlParseH
#define QAwsXmlParseH 1

#include <QObject>
#include <QString>
#include "winExport.h"

class sdblibEXPORT QAwsXmlParse : public QObject
{
  Q_OBJECT
public:
  QAwsXmlParse( QObject * parent = 0 );
  virtual ~QAwsXmlParse( void );

public slots:
  virtual void parse( const QString &text );

signals:
  void result( const QString &text );
};

#endif
