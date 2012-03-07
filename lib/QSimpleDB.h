#ifndef QSimpleDBH
#define QSimpleDBH 1

#include "QSendData.h"
#include "winExport.h"

#include <QVector>
#include <QPair>

typedef QVector< QPair< int, QPair< QString, QString > > > value_store_type;
typedef QVector< QString > value_ask_type;

class sdblibEXPORT QSimpleDB : public QSendData
{
  Q_OBJECT
public:
  QSimpleDB( QObject * parent = 0 );
  virtual ~QSimpleDB( void );

  virtual void createDB( const QString &db_name );
  virtual void removeDB( const QString &db_name );
  virtual void detailDB( const QString &db_name );
  virtual void listDBs( int max_number = -1, int next_token = -1 );
  virtual void putItem( const QString &db_name, const QString &item_name, const value_store_type &value_pairs );
  virtual void getItem( const QString &db_name, const QString &item_name, bool consistent_read = false );
  virtual void getItem( const QString &db_name, const QString &item_name, const value_ask_type &names, bool consistent_read = false );
  virtual void removeItem( const QString &db_name, const QString &item_name, const value_store_type &value_pairs );
  virtual void selectItems( const QString &select_text );
};

#endif
