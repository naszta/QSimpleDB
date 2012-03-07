#ifndef ItemEditH
#define ItemEditH 1

#include <QDialog>
#include "ui_ItemEdit.h"


class QSettings;

class ItemEdit : public QDialog
{
  Q_OBJECT
public:
  ItemEdit( QWidget * parent = 0, Qt::WindowFlags flag = 0 );
  virtual ~ItemEdit( void );

protected:
  QSettings * sets;

private:
  Ui::ItemEdit ui;
};

#endif
