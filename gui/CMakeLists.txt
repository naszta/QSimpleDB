PROJECT(SimpleDBBin)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SimpleDBLib_SOURCE_DIR}
)

SET(SimpleDBGuiSrcs
  main.cxx
  MainWindow.cxx
  AmazonConfig.cxx
  ItemEdit.cxx
)

SET(SimpleDBGuiHdrs
)

SET(SimpleDBGuiQtHdrs
  MainWindow.h
  AmazonConfig.h
  ItemEdit.h
)

SET(UIS
  MainWindow.ui
  AmazonConfig.ui
  ItemEdit.ui
)

SET(RCCS
  resources.qrc
)

QT4_ADD_RESOURCES(RCCSrcs ${RCCS})  
QT4_WRAP_UI(UISrcs ${UIS})
QT4_WRAP_CPP(MOCSrcs ${SimpleDBGuiQtHdrs})

SOURCE_GROUP("Generated" FILES ${RCCSrcs})
SOURCE_GROUP("Generated" FILES ${UISrcs})
SOURCE_GROUP("Generated" FILES ${MOCSrcs})

ADD_EXECUTABLE(SimpleDBGui WIN32 ${SimpleDBGuiSrcs} ${SimpleDBGuiHdrs} ${SimpleDBGuiQtHdrs} ${UISrcs} ${MOCSrcs} ${RCCSrcs})

SET_TARGET_PROPERTIES(SimpleDBGui PROPERTIES
  VERSION "${SimpleDB_MAJOR_VERSION}.${SimpleDB_MINOR_VERSION}"
  SOVERSION "${SimpleDB_MAJOR_VERSION}.${SimpleDB_MINOR_VERSION}"
)

TARGET_LINK_LIBRARIES(SimpleDBGui
  ${QT_LIBRARIES}
  SimpleDBLib
)

INSTALL(TARGETS SimpleDBGui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib )

MESSAGE( STATUS "SimpleDBGui has been added." )
