#include "QSendData.h"

#include <QSslSocket>
#include <QDateTime>

#include "QHmacSha1.h"

#include <ostream>
#include <sstream>
#include <map>
#include <string>

// http://docs.amazonwebservices.com/AmazonSimpleDB/latest/DeveloperGuide/HMACAuth.html
QByteArray AWSEncode( const QString &src )
{
  return AWSEncode( src.toUtf8() );
}

QByteArray AWSEncode( const QByteArray &src )
{
  QByteArray retval;

  char temp, hexa[2], mod_temp;

  for ( int i = 0; i < src.size(); ++i )
  {
    temp = src[i];
    if ( ( temp >= 'A' ) && ( temp <= 'Z' ) )
      retval.push_back( temp );
    else if ( ( temp >= 'a' ) && ( temp <= 'z' ) )
      retval.push_back( temp );
    else if ( ( temp >= '0' ) && ( temp <= '9' ) )
      retval.push_back( temp );
    else if ( ( temp == '-' ) || ( temp == '_' ) || ( temp == '~' ) || ( temp == '.' ) )
      retval.push_back( temp );
    else
    {
      mod_temp = temp & 0x0F;
      if ( mod_temp < 10 )
        hexa[0] = '0' + mod_temp;
      else
        hexa[0] = 'A' + ( mod_temp - 10 );

      temp >>= 4;
      mod_temp = temp & 0x0F;
      if ( mod_temp < 10 )
        hexa[1] = '0' + mod_temp;
      else
        hexa[1] = 'A' + ( mod_temp - 10 );

      retval.push_back( '%' );
      retval.push_back(hexa[1]);
      retval.push_back(hexa[0]);
    }
  }

  return retval;
}

typedef std::map<std::string, std::string> items_type;

class QSendDataPrivate
{
public:
  items_type key_pairs;
};

std::ostream & aws_endline( std::ostream &os )
{
  os.put( 10 );
  return os;
}

void generate_data( items_type &source, std::string &target, const std::string &host, bool string_to_sign )
{
  std::ostringstream items;
  std::ostringstream request;

  for ( items_type::iterator iter = source.begin(); iter != source.end(); ++iter )
  {
    if ( iter != source.begin() )
      items.put('&');
    items << iter->first;
    items.put('=');
    items << iter->second;
  }

  if ( string_to_sign )
  {
    request << "POST";
    aws_endline(request);
    request << host;
    aws_endline(request);
    request << "/";
    aws_endline(request);
  }
  else
  {
    request << "POST / HTTP/1.1";
    aws_endline(request);
    request << "Content-Type: application/x-www-form-urlencoded; charset=utf-8";
    aws_endline(request);
    request << "Host: " << host;
    aws_endline(request);
    request << "Content-Length: " << items.str().size();
    aws_endline(request);
    aws_endline(request);
  }

  request << items.str();

  target = request.str();
}

QSendData::QSendData( QObject * parent )
  : QObject( parent ), store( new QSendDataPrivate )
{

}

QSendData::~QSendData( void )
{
  if ( this->store != 0 )
  {
    delete this->store;
    this->store = 0;
  }
}

void QSendData::setAmazonCredinals( const QUrl &server, const QString &AKID, const QString &SAK )
{
  this->server = server;
  this->AKID = AKID;
  this->SAK = SAK;
}

void QSendData::addPair( const QString &key, const QString &value )
{
  this->store->key_pairs.insert( std::make_pair<std::string,std::string>( AWSEncode( key ).data(), AWSEncode( value ).data() ) );
}

void QSendData::clear( void )
{
  this->store->key_pairs.clear();
}

void QSendData::sendRequest( void )
{
  items_type items;
  QDateTime cur_date(QDateTime::currentDateTimeUtc());
  items_type::iterator temp_iter;

  items.insert( std::make_pair( "AWSAccessKeyId", this->AKID.toStdString() ) );

  for ( temp_iter = this->store->key_pairs.begin(); temp_iter != this->store->key_pairs.end(); ++temp_iter )
    items.insert( std::make_pair<std::string,std::string>( temp_iter->first, temp_iter->second ) );

  items.insert( std::make_pair<std::string,std::string>( "SignatureMethod", "HmacSHA1" ) );
  items.insert( std::make_pair<std::string,std::string>( "SignatureVersion", "2" ) );
  items.insert( std::make_pair<std::string,std::string>( "Timestamp", AWSEncode( cur_date.toString( Qt::ISODate ) ).data() ) );
  items.insert( std::make_pair<std::string,std::string>( "Version", "2009-04-15" ) );

  QByteArray ba_password( this->SAK.toAscii() );

  std::string target;
  generate_data( items, target, this->server.host().toStdString(), true );
  
  QByteArray ba_target;
  ba_target.resize( target.size() );
  memcpy( ba_target.data(), target.c_str(), target.size() );

  QByteArray signature = QHmacSha1::CalcHmacSha1( target.c_str(), ba_password );

  items.insert( std::make_pair<std::string,std::string>( "Signature", AWSEncode( signature.toBase64() ).data() ) );

  generate_data( items, target, this->server.host().toStdString(), false );

  QSslSocket * connection = new QSslSocket( this );
 
  if ( this->server.scheme() == "http" )
  {
    connection->connectToHost( this->server.host(), 80 );
    if ( ! connection->waitForConnected() )
    {
      delete connection;
      return;
    }
  }
  else
  {
    connection->setProtocol( QSsl::SslV2 );
    connection->connectToHostEncrypted( this->server.host(), 443 );
    if ( ! connection->waitForEncrypted() )
    {
      delete connection;
      return;
    }
  }

  connection->write( target.c_str() );

  connection->waitForReadyRead();

  QByteArray result_line, to_parse;

  while ( ! connection->atEnd() )
  {
    result_line = connection->readLine();
    if ( result_line[0] == char(13) )
    {
      result_line = connection->readLine();
      while ( ! connection->atEnd() )
      {
        result_line = connection->readLine();
        if ( result_line[0] == '0' )
          break;
        to_parse += result_line;
      }
    }
  }

  if ( to_parse.size() > 0 )
    emit finished( QString( to_parse ) );

  connection->close();

  connection->waitForDisconnected();

  connection->deleteLater();
}
