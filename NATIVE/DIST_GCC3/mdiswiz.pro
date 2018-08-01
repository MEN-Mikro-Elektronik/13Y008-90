#
# Build mdiswizard for MDIS system package
# make sure that you have sourced ./builddist.env first!
#
CONFIG += static
CONFIG -= debug


DEPENDPATH  += ../../COM ../../COM/models
INCLUDEPATH += ../../COM ../../COM/models

include ( ../../COM/mdiswiz.pro )


unix {
    #DESTDIR    = ../bin
    OBJECTS_DIR = .obj
    MOC_DIR     = .moc
    QMAKE_CXXFLAGS_WARN_ON=-Wall
    LIBS += -static -lpthread
    QMAKE_LIBS_X11 = -L/usr/X11R6/lib -lXinerama -ldl -lXext -lX11 -lm -lpthread

    # installation rules
    target.path = ../../../../BIN
    target.files = mdiswiz
    INSTALLS += target

}

unix {
   #???SOURCES += libdl.c
}
