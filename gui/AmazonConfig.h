#ifndef AmazonConfigH
#define AmazonConfigH 1

#include <QDialog>
#include "ui_AmazonConfig.h"

class QSettings;

class AmazonConfig : public QDialog
{
  Q_OBJECT
public:
  AmazonConfig( QWidget * parent = 0, Qt::WindowFlags flag = 0 );
  virtual ~AmazonConfig( void );

public slots:
  virtual void accept( void );

protected slots:
  virtual void pushLoad( void );
  virtual void pushSave( void );
  virtual void serverEdited( const QString &server );

protected:
  QSettings * sets;

private:
  Ui::AmazonConfig ui;
};

#endif
