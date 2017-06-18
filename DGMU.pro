#-------------------------------------------------
#
# Project created by QtCreator 2017-05-14T19:52:39
#
#-------------------------------------------------

QT       += core gui datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = DGMU
TEMPLATE = app


SOURCES += main.cpp\
        Windows/mainwindow.cpp \
    Windows/drawwindow.cpp \
    Algorithm/nsmualgorithm.cpp \
    #Graph/division.cpp \
    #Parameter/parameter.cpp \
    #Parameter/simulationparameters.cpp \
    #Pattern/Com/communicator.cpp \
    #Graph/graph.cpp \
    Graph/3D/graph3d.cpp \
    #Graph/3D/surfacegraph.cpp \
    Graph/2D/graph2d.cpp \
    qcustomplot/qcustomplot.cpp \
    Windows/srwindow.cpp \
    Algorithm/bissection.cpp

HEADERS  += Windows/mainwindow.h \
    Windows/drawwindow.h \
    Algorithm/strategy.h \
    Algorithm/nsmualgorithm.h \
    Algorithm/option.h \
    #Graph/division.h \
    #Parameter/simulationparameters.h \
    Parameter/parameter.h \
    Pattern/Com/communicator.h \
    Graph/graph.h \
    #Graph/3D/surfacegraph.h \
    Graph/3D/graph3d.h \
    Graph/2D/graph2d.h \
    qcustomplot/qcustomplot.h \
    Windows/srwindow.h \
    Algorithm/bissection.h


FORMS    += Windows/mainwindow.ui \
    Windows/drawwindow_tmp.ui \
    Windows/srwindow.ui

QMAKE_CXXFLAGS += /bigobj
