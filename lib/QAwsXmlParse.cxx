#include "QAwsXmlParse.h"


QAwsXmlParse::QAwsXmlParse( QObject * parent /* = 0 */ )
  : QObject( parent )
{

}

QAwsXmlParse::~QAwsXmlParse( void )
{

}

void QAwsXmlParse::parse( const QString &text  )
{


  emit result(text);
}
