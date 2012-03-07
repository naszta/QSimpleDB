#include "ItemEdit.h"

#include <QSettings>

ItemEdit::ItemEdit( QWidget * parent /* = 0 */, Qt::WindowFlags flag /* = 0 */ )
  : QDialog( parent, flag ), sets( new QSettings )
{
  ui.setupUi(this);

  this->restoreGeometry( this->sets->value( "MainWindow/ItemEdit", this->saveGeometry() ).toByteArray() );
}

ItemEdit::~ItemEdit( void )
{
  if ( this->sets != 0 )
  {
    this->sets->setValue( "MainWindow/ItemEdit", this->saveGeometry() );

    delete this->sets;
    this->sets = 0;
  }
}
