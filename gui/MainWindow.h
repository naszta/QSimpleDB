#ifndef MainWindowH
#define MainWindowH 1

#include <QDialog>
#include "ui_MainWindow.h"

class QSettings;
class QSimpleDB;
class QAwsXmlParse;

class MainWindow : public QDialog
{
  Q_OBJECT
public:
  MainWindow( QWidget * parent = 0, Qt::WindowFlags flag = 0 );
  virtual ~MainWindow( void );

protected slots:
  virtual void pushCreateDB( void );
  virtual void pushRemoveDB( void );
  virtual void pushListDB( void );
  virtual void pushInfoDB( void );

  virtual void pushConfig( void );

protected:
  QSettings * sets;
  QSimpleDB * simple_db;
  QAwsXmlParse * parser;

private:
  Ui::MainWindow ui;
};

#endif
