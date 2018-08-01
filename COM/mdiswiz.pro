#
# Common pro file for all mdiswizards
#
# Include this file from the native project file
#

#TEMPLATE = app

#CONFIG += debug

CXXFLAGS += Wno-unused

INCLUDEPATH += models 

# enable wDebug() in debug configuration
debug {
    DEFINES += WIZDEBUG
}

# use XML and port code from Qt 3 to Qt 4
QT += xml qt3support

# Input
HEADERS += cfgmakefile.h \
           configuration.h \
           descfile.h \
           descriptor.h \
           devgenericprop.h \
           dipbutton.h \
           dipswitch.h \
           tristatebutton.h \
           tristateswitch.h \
           execprogram.h \
           hexeditwidget.h \
           hexspinbox.h \
           hwcomponent.h \
           install.h \
           instnamewidget.h \
           mdiswiz.h \
           mdiswizdialogs.h \
           modelfactory.h \
           namemng.h \
           osfactory.h \
           oslinux.h \
           osvxworks.h \
           osos9.h \
		   osqnx.h \
           packagedb.h \
           packagefile.h \
           propertiesdlg.h \
           resourcemng.h \
           setting.h \
           swmodule.h \
           swmodulestab.h \
           targetos.h \
           typestr.h \
           util.h \
           vmsgbox.h \
           wizexcept.h \
           models/esmcarrier.h \
           models/comexprcarrier.h \
           models/modbbisa201.h \
           models/modbbisa201proptab.h \
           models/modbbisa203n.h \
           models/modbbisa203nproptab.h \
           models/modbbischam.h \
           models/modbbischamisa.h \
           models/modbbisisa.h \
           models/modbbispcigen.h \
           models/modbbispcigenproptab.h \
           models/modbbissmbpcigen.h \
           models/modbbissmbisagen.h \
           models/modbbispcimmod.h \
           models/modbbispcimmodproptab.h \
           models/modbbisd203fam.h \
           models/modbbisd203famproptab.h \
           models/modcpua11.h \
           models/modcpua12.h \
           models/modcpua17.h \
           models/modcpua19.h \
           models/modcpua21.h \
           models/modcpub11.h \
           models/modcpucomexpr.h \           
           models/modcpucpcipc.h \
           models/modcpucpciserpc.h \
           models/modcpud2.h \
           models/modcpud4.h \
           models/modcpud6.h \
           models/modcpuesm.h \
           models/modcpuesmexpr.h \
           models/modcpuesmini.h \
           models/modcpuf1n.h \
           models/modcpuf2.h \
           models/modcpuf6.h \
           models/modcpuf7.h \
           models/modcpuf9.h \
           models/modcpuf14.h \
           models/modcpuf19p.h \
           models/modcpug20.h \
           models/modcpupc.h \
           models/modcpupp1.h \
           models/modcpusc24.h \
           models/modexta500.h \
           models/modextcomexpr.h \
           models/modextec01.h \
           models/modextf501.h \
           models/modextf503.h \
           models/modmdisgeneric.h \
           models/modmdischam.h \
           models/modmdischamproptab.h \

#           helpviewer\helpwindow.h \


SOURCES += ../COM/cfgmakefile.cpp \
           configuration.cpp \
           descfile.cpp \
           descriptor.cpp \
           devgenericprop.cpp \
           dipbutton.cpp \
           dipswitch.cpp \
           tristatebutton.cpp \
           tristateswitch.cpp \
           execprogram.cpp \
           hexeditwidget.cpp \
           hwcomponent.cpp \
           install.cpp \
           instnamewidget.cpp \
           main.cpp \
           mdiswiz.cpp \
           modelfactory.cpp \
           namemng.cpp \
           osfactory.cpp \
           oslinux.cpp \
           osos9.cpp \
		   osqnx.cpp \
           osvxworks.cpp \
           packagedb.cpp \
           packagefile.cpp \
           propertiesdlg.cpp \
           pkg2dsc.cpp \
           pkg2inf.cpp \
           pkgauto.cpp \
           resourcemng.cpp \
           setting.cpp \
           swmodule.cpp \
           swmodulestab.cpp \
           util.cpp \
           vmsgbox.cpp \
           xmldump.cpp \
           models/esmcarrier.cpp \
           models/comexprcarrier.cpp \
           models/modbbisa201.cpp \
           models/modbbisa201proptab.cpp \
           models/modbbisa203n.cpp \
           models/modbbisa203nproptab.cpp \
           models/modbbischam.cpp \
           models/modbbischamisa.cpp \
           models/modbbisisa.cpp \
           models/modbbispcigen.cpp \
           models/modbbispcigenproptab.cpp \
           models/modbbissmbpcigen.cpp \
           models/modbbissmbisagen.cpp \
           models/modbbispcimmod.cpp \
           models/modbbispcimmodproptab.cpp \
           models/modbbisd203fam.cpp \
           models/modbbisd203famproptab.cpp \
           models/modcpua11.cpp \
           models/modcpua12.cpp \
           models/modcpua17.cpp \
           models/modcpua19.cpp \
           models/modcpua21.cpp \
           models/modcpub11.cpp \
           models/modcpucomexpr.cpp \           
           models/modcpucpcipc.cpp \
           models/modcpucpciserpc.cpp \
           models/modcpud2.cpp \
           models/modcpud4.cpp \
           models/modcpud6.cpp \
           models/modcpuesm.cpp \
           models/modcpuesmexpr.cpp \
           models/modcpuesmini.cpp \
           models/modcpuf1n.cpp \
           models/modcpuf2.cpp \
           models/modcpuf6.cpp \
           models/modcpuf7.cpp \
           models/modcpuf9.cpp \
           models/modcpuf14.cpp \
           models/modcpuf19p.cpp \
           models/modcpug20.cpp \
           models/modcpupc.cpp \
           models/modcpupp1.cpp \
           models/modcpusc24.cpp \
           models/modexta500.cpp \
           models/modextcomexpr.cpp \
           models/modextec01.cpp \
           models/modextf501.cpp \
           models/modextf503.cpp \
           models/modmdisgeneric.cpp \
           models/modmdischam.cpp \
           models/modmdischamproptab.cpp \
