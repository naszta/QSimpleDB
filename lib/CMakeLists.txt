PROJECT(SimpleDBLib)

FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_DONT_USE_QTGUI 1)
SET(QT_USE_QTNETWORK 1)
SET(QT_USE_QTXML 1)
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(SimpleDBLibSrcs
  QSendData.cxx
  QSimpleDB.cxx
  QHmacSha1.cxx
  QAwsXmlParse.cxx
)

SET(SimpleDBLibHdrs
  winExport.h
)

SET(SimpleDBLibQtHdrs
  QSendData.h
  QSimpleDB.h
  QHmacSha1.h 
  QAwsXmlParse.h
  QCloudCryptographicHash.h
)

SET(UIS
)

SET(RCCS
)

IF(WIN32)
  SET(SimpleDBLibSrcs ${SimpleDBLibSrcs}
    QCloudCryptographicHashWin.cxx
  )
ELSE(WIN32)
  SET(SimpleDBLibSrcs ${SimpleDBLibSrcs}
    QCloudCryptographicHashUnix.cxx
  )
ENDIF(WIN32)

QT4_ADD_RESOURCES(RCCSrcs ${RCCS})  
QT4_WRAP_UI(UISrcs ${UIS})
QT4_WRAP_CPP(MOCSrcs ${SimpleDBLibQtHdrs})

SOURCE_GROUP("Generated" FILES ${RCCSrcs})
SOURCE_GROUP("Generated" FILES ${UISrcs})
SOURCE_GROUP("Generated" FILES ${MOCSrcs})

IF(WIN32)
  SET(WIN_VERSION ${CMAKE_CURRENT_BINARY_DIR}/SimpleDBLib.rc)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/SimpleDBLib.rc.in ${WIN_VERSION} @ONLY IMMEDIATE)
  SOURCE_GROUP("Resources" FILES ${WIN_VERSION} )
ENDIF(WIN32) 

ADD_LIBRARY(SimpleDBLib SHARED ${SimpleDBLibSrcs} ${SimpleDBLibHdrs} ${SimpleDBLibQtHdrs} ${UISrcs} ${MOCSrcs} ${RCCSrcs} ${WIN_VERSION})

SET_TARGET_PROPERTIES(SimpleDBLib PROPERTIES
  VERSION "${SimpleDB_MAJOR_VERSION}.${SimpleDB_MINOR_VERSION}"
  SOVERSION "${SimpleDB_MAJOR_VERSION}.${SimpleDB_MINOR_VERSION}"
)

TARGET_LINK_LIBRARIES(SimpleDBLib
  ${QT_LIBRARIES}
)

INSTALL(TARGETS SimpleDBLib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)

MESSAGE( STATUS "SimpleDBLib has been added." )
