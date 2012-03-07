#include "QSimpleDB.h"

// http://docs.amazonwebservices.com/AmazonSimpleDB/latest/DeveloperGuide/SDB_API_Operations.html

QSimpleDB::QSimpleDB( QObject * parent )
  : QSendData( parent )
{

}

QSimpleDB::~QSimpleDB( void )
{

}

void QSimpleDB::createDB( const QString &db_name )
{
  this->addPair( "Action", "CreateDomain" );
  this->addPair( "DomainName", db_name );
  this->sendRequest();
  this->clear();
}

void QSimpleDB::removeDB( const QString &db_name )
{
  this->addPair( "Action", "DeleteDomain" );
  this->addPair( "DomainName", db_name );
  this->sendRequest();
  this->clear();
}

void QSimpleDB::detailDB( const QString &db_name )
{
  this->addPair( "Action", "DomainMetadata" );
  this->addPair( "DomainName", db_name );
  this->sendRequest();
  this->clear();
}

void QSimpleDB::listDBs( int max_number, int next_token )
{
  this->addPair( "Action", "ListDomains" );

  if ( ( max_number > 0 ) && ( max_number <= 100 ) )
    this->addPair( "MaxNumberOfDomains", QString::number( max_number ) );

  if ( ( next_token >= 0 ) && ( next_token <= 100 ) )
    this->addPair( "NextToken", QString::number( next_token ) );

  this->sendRequest();
  this->clear();
}

void QSimpleDB::putItem( const QString &db_name, const QString &item_name, const value_store_type &value_pairs )
{
  if ( value_pairs.empty() )
    return;

  this->addPair( "Action", "PutAttributes" );
  this->addPair( "DomainName", db_name );
  this->addPair( "ItemName", item_name );

  for ( int i = 0; i < value_pairs.size(); ++i )
  {
    this->addPair( QString("Attribute.") + QString::number(value_pairs[i].first) + QString(".Name"), value_pairs[i].second.first );
    this->addPair( QString("Attribute.") + QString::number(value_pairs[i].first) + QString(".Value"), value_pairs[i].second.second );
  }

  this->sendRequest();
  this->clear();
}

void QSimpleDB::getItem( const QString &db_name, const QString &item_name, bool consistent_read )
{
  this->addPair( "Action", "GetAttributes" );
  this->addPair( "DomainName", db_name );
  this->addPair( "ItemName", item_name );
  if ( consistent_read )
    this->addPair( "ConsistentRead", "true" );
  this->sendRequest();
  this->clear();
}

void QSimpleDB::getItem( const QString &db_name, const QString &item_name, const value_ask_type &names, bool consistent_read )
{
  this->addPair( "Action", "GetAttributes" );
  this->addPair( "DomainName", db_name );
  this->addPair( "ItemName", item_name );
  if ( consistent_read )
    this->addPair( "ConsistentRead", "true" );

  for ( int i = 0; i < names.size(); ++i )
    this->addPair( QString( "AttributeName." ) + QString::number(i), names[i] );

  this->sendRequest();
  this->clear();
}

void QSimpleDB::removeItem( const QString &db_name, const QString &item_name, const value_store_type &value_pairs )
{
  if ( value_pairs.empty() )
    return;

  this->addPair( "Action", "DeleteAttributes" );
  this->addPair( "DomainName", db_name );
  this->addPair( "ItemName", item_name );

  for ( int i = 0; i < value_pairs.size(); ++i )
  {
    this->addPair( QString("Attribute.") + QString::number(value_pairs[i].first) + QString(".Name"), value_pairs[i].second.first );
    this->addPair( QString("Attribute.") + QString::number(value_pairs[i].first) + QString(".Value"), value_pairs[i].second.second );
  }

  this->sendRequest();
  this->clear();
}

void QSimpleDB::selectItems( const QString &select_text )
{
  this->addPair( "Action", "Select" );
  this->addPair( "SelectExpression", select_text );

  this->sendRequest();
  this->clear();
}
