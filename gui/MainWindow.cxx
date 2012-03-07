#include "MainWindow.h"

#include "AmazonConfig.h"
#include "QAwsXmlParse.h"
#include "ItemEdit.h"

#include <QSettings>

#include "QSimpleDB.h"

MainWindow::MainWindow( QWidget * parent /* = 0 */, Qt::WindowFlags flag /* = 0 */ )
  : QDialog( parent, flag ), sets(new QSettings), simple_db( new QSimpleDB(this) ), parser( new QAwsXmlParse(this) )
{
  ui.setupUi(this);

  connect( ui.pushConfig, SIGNAL(clicked()), this, SLOT(pushConfig()) );
  connect( ui.pushCreateDB, SIGNAL(clicked()), this, SLOT(pushCreateDB()) );
  connect( ui.pushRemoveDB, SIGNAL(clicked()), this, SLOT(pushRemoveDB()) );
  connect( ui.pushListDB, SIGNAL(clicked()), this, SLOT(pushListDB()) );
  connect( ui.pushMetaDB, SIGNAL(clicked()), this, SLOT(pushInfoDB()) );
  connect( this->simple_db, SIGNAL(finished(const QString&)), this->parser, SLOT(parse(const QString &)) );
  connect( this->parser, SIGNAL(result(const QString&)), ui.plainAnswer, SLOT(setPlainText(const QString &)) );

  this->simple_db->setAmazonCredinals( this->sets->value( "AmazonConfig/Server" ).toUrl(), this->sets->value( "AmazonConfig/AKID" ).toString(), this->sets->value( "AmazonConfig/SAK" ).toString() );
  this->restoreGeometry( this->sets->value( "MainWindow/Geometry", this->saveGeometry() ).toByteArray() );

  ui.lineDbName->setText( this->sets->value( "MainWindow/DB" ).toString() );
  ui.lineItem->setText( this->sets->value( "MainWindow/Item" ).toString() );
}

MainWindow::~MainWindow( void )
{
  if ( this->sets != 0 )
  {
    this->sets->setValue( "MainWindow/DB", ui.lineDbName->text() );
    this->sets->setValue( "MainWindow/Item", ui.lineItem->text() );
    this->sets->setValue( "MainWindow/Geometry", this->saveGeometry() );
    
    delete this->sets;
    this->sets = 0;
  }
  if ( this->simple_db != 0 )
  {
    delete this->simple_db;
    this->simple_db = 0;
  }
  if ( this->parser != 0 )
  {
    delete this->parser;
    this->parser = 0;
  }
}

void MainWindow::pushCreateDB( void )
{
  this->simple_db->createDB( ui.lineDbName->text() );
}

void MainWindow::pushRemoveDB( void )
{
  this->simple_db->removeDB( ui.lineDbName->text() );
}

void MainWindow::pushListDB( void )
{
  this->simple_db->listDBs();
}

void MainWindow::pushInfoDB( void )
{
  this->simple_db->detailDB( ui.lineDbName->text() );
}

void MainWindow::pushConfig( void )
{
  AmazonConfig * config = new AmazonConfig( this );
  if ( config->exec() == QDialog::Accepted )
  {
    this->simple_db->setAmazonCredinals( this->sets->value( "AmazonConfig/Server" ).toUrl(), this->sets->value( "AmazonConfig/AKID" ).toString(), this->sets->value( "AmazonConfig/SAK" ).toString() );
  }
  delete config;
}

