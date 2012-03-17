#include <cstdlib>

#include <QApplication>

#include "MainWindow.h"

#include "SimpleDBVersion.h"


int main( int argc, char ** argv )
{
  QApplication app( argc, argv );

  app.setApplicationName( "SimpleDBGui" );
  app.setOrganizationName( "Nasztanovics Ferenc" );
  app.setOrganizationDomain( "naszta.hu" );
#if QT_VERSION >= 0x040400
  app.setApplicationVersion( SimpleDB_VERSION );
#endif

  MainWindow GUI;

  GUI.show();

  return app.exec();
}

#ifdef WIN32
#include <Windows.h>

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  return main( 1, &lpCmdLine );
}

#endif
