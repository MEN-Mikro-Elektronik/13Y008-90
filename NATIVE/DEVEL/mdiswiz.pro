#
# Build mdiswizard for MDIS system package
# make sure that you have sourced ./builddist.env first!
#

CONFIG += debug



DEPENDPATH  += ../../COM ../../COM/models
INCLUDEPATH += ../../COM ../../COM/models /home/swserver_disc/QT_460/qt-everywhere-opensource-src-4.6.0/include

include ( ../../COM/mdiswiz.pro )


unix {
    DESTDIR    = .
    OBJECTS_DIR = .obj
    MOC_DIR     = .moc
    QMAKE_CXXFLAGS_WARN_ON=-Wall

    # installation rules
    #target.path = ../../../../BIN
    #target.files = mdiswiz
    #INSTALLS += target

}

unix {
   #???SOURCES += libdl.c
}
