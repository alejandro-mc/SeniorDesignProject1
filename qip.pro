TEMPLATE    = app
TARGET      = qip
QT 	   += widgets printsupport
OBJECTS_DIR = ./obj
MOC_DIR     = ./moc


win32-msvc2013 {
	INCLUDEPATH 	+= ./IP/win/header
	LIBS 		+= -L./IP/win/lib
	LIBS 		+= -lIP_d
	QMAKE_CXXFLAGS  += /MP /Zi
}


macx{
        QMAKE_MAC_SDK = macosx10.11
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
	INCLUDEPATH += ./IP/mac/header
	LIBS        += -L./IP/mac/lib
	LIBS        += -lIP_d
}

unix:!macx {
	CONFIG += C++11
	INCLUDEPATH += ./IP/linux/header
	LIBS        += -L./IP/linux/lib
	LIBS        += -lIP_d
}


# Input
HEADERS +=	MainWindow.h	\
		ImageFilter.h	\
		qcustomplot.h	\
		Dummy.h		\
		Threshold.h	\
		Contrast.h	\
		Quantize.h	\
		HistStretch.h	\
		Blur_Sharpen.h	\
		Median.h	\
		Sharpen.h	\

		
SOURCES +=	main.cpp	\ 
		MainWindow.cpp 	\
		ImageFilter.cpp	\
		qcustomplot.cpp	\
		Dummy.cpp	\
		Threshold.cpp	\
		Contrast.cpp	\
		Quantize.cpp	\
		HistStretch.cpp	\
		Blur_Sharpen.cpp\
		Median.cpp	\
		Sharpen.cpp	\

unix:!macx: LIBS += -L$$PWD/IP/linux/lib/ -lIP_d

INCLUDEPATH += $$PWD/IP/linux
DEPENDPATH += $$PWD/IP/linux

unix:!macx: PRE_TARGETDEPS += $$PWD/IP/linux/lib/libIP_d.a
