#include "AmazonConfig.h"

#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QUrl>
#include <QMessageBox>

AmazonConfig::AmazonConfig( QWidget * parent /* = 0 */, Qt::WindowFlags flag /* = 0 */ )
  : QDialog( parent, flag ), sets( new QSettings )
{
  ui.setupUi(this);

  connect( ui.pushLoad, SIGNAL(clicked()), this, SLOT(pushLoad()) );
  connect( ui.pushSave, SIGNAL(clicked()), this, SLOT(pushSave()) );
  connect( ui.lineServer, SIGNAL(textEdited(const QString &)), this, SLOT(serverEdited( const QString &)) );

  this->restoreGeometry( this->sets->value( "AmazonConfig/Geometry", this->saveGeometry() ).toByteArray() );
  ui.lineServer->setText( this->sets->value( "AmazonConfig/Server" ).toString() );
  ui.lineAKID->setText( this->sets->value( "AmazonConfig/AKID" ).toString() );
  ui.lineSAK->setText( this->sets->value( "AmazonConfig/SAK" ).toString() );
}

AmazonConfig::~AmazonConfig( void )
{
  if ( this->sets != 0 )
  {
    this->sets->setValue( "AmazonConfig/Geometry", this->saveGeometry() );

    delete this->sets;
    this->sets = 0;
  }
}

void AmazonConfig::accept( void )
{
  this->sets->setValue( "AmazonConfig/SAK", ui.lineSAK->text() );
  this->sets->setValue( "AmazonConfig/AKID", ui.lineAKID->text() );
  this->sets->setValue( "AmazonConfig/Server", ui.lineServer->text() );
  QDialog::accept();
}

void AmazonConfig::pushLoad( void )
{
  QString filename = QFileDialog::getOpenFileName( this, "Load XML config", QDir::currentPath(), "XML files (*.xml);;All files (*)" );
  if ( filename.isEmpty() )
    return;

  QFile * xml_file = new QFile( filename );

  if ( ! xml_file->open( QIODevice::ReadOnly ) )
  {
    delete xml_file;
    return;
  }

  ui.lineServer->setText( "" );
  ui.lineAKID->setText( "" );
  ui.lineSAK->setText( "" );

  {
    QXmlStreamReader xml_reader( xml_file );

    while ( ! xml_reader.atEnd() )
    {
      if ( xml_reader.readNextStartElement() )
      {
        if ( xml_reader.name() == "server" )
          ui.lineServer->setText( xml_reader.readElementText() );
        else if ( xml_reader.name() == "AKID" )
          ui.lineAKID->setText( xml_reader.readElementText() );
        else if ( xml_reader.name() == "SAK" )
          ui.lineSAK->setText( xml_reader.readElementText() );
      }
    }
  }
  
  xml_file->close();
  delete xml_file;
}

void AmazonConfig::pushSave( void )
{
  QString filename = QFileDialog::getSaveFileName( this, "Save XML config", QDir::currentPath(), "XML files (*.xml);;All files (*)" );
  if ( filename.isEmpty() )
    return;
  QFile * xml_file = new QFile( filename );

  if ( ! xml_file->open( QIODevice::WriteOnly | QIODevice::Truncate ) )
  {
    delete xml_file;
    return;
  }

  {
    QXmlStreamWriter xml_writer( xml_file );
    xml_writer.setAutoFormatting(true);
    xml_writer.writeStartDocument();

    xml_writer.writeStartElement( "AmazonConfig" );

    xml_writer.writeTextElement( "server", ui.lineServer->text() );
    xml_writer.writeTextElement( "AKID", ui.lineAKID->text() );
    xml_writer.writeTextElement( "SAK", ui.lineSAK->text() );

    xml_writer.writeEndElement();

    xml_writer.writeEndDocument();
  }

  xml_file->flush();
  xml_file->close();
  delete xml_file;
}

void AmazonConfig::serverEdited( const QString &server )
{
  QUrl server_url(server);
  
  if ( ( server_url.scheme() != "http" ) && ( server_url.scheme() != "https" ) )
  {
    QMessageBox::warning( this, "Invalid server name!", server + QString( " is not http or https url!" ) );
    ui.lineServer->setText( "" );
  }
}
