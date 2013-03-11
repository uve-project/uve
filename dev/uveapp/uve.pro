TEMPLATE = app

CONFIG += qt rtti silent
CONFIG += debug_and_release
CONFIG += debug
# CONFIG += release

# In order to avoid warnings with the parser
QMAKE_CXXFLAGS          += -Wno-write-strings -Wall
QMAKE_CXXFLAGS_WARN_ON  += -Wno-reorder

# Compilation/Application directory
DESTDIR = dist
UI_DIR = tmp/ui
RCC_DIR = tmp/rcc
win32{
    DEFINES += TARGET_WIN
    OBJECTS_DIR = tmp/obj/win
    MOC_DIR = tmp/moc/win
    CONFIG(release) {
        DESTDIR = ../tools/win/installer/bin/bin
        CONFIG -= console
    }
}
unix{
    OBJECTS_DIR = tmp/obj/linux
    MOC_DIR = tmp/moc/linux
}

# QT components
QT += xml
QT += webkit
QT += xmlpatterns

# Application name
TARGET = uve

HEADERS       = \
    src/parser/vhdlutils.h \
    src/parser/vhdlparser.h \
    src/parser/messagewidget.h \
    src/routing/routingboard.h \
    src/window/mdichild.h \
    src/window/mainwindow.h \
    src/uvmmodel/uvmvirtualsequencer.h \
    src/uvmmodel/uvmverificationcomponent.h \
    src/uvmmodel/uvmtop.h \
    src/uvmmodel/uvmtestcase.h \
    src/uvmmodel/uvmtestbench.h \
    src/uvmmodel/uvmsequencer.h \
    src/uvmmodel/uvmscoreboard.h \
    src/uvmmodel/uvmport.h \
    src/uvmmodel/uvmmonitor.h \
    src/uvmmodel/uvminterface.h \
    src/uvmmodel/uvmdriver.h \
    src/uvmmodel/uvmdesign.h \
    src/uvmmodel/uvmconnection.h \
    src/uvmmodel/uvmconfig.h \
    src/uvmmodel/uvmcomponent.h \
    src/uvmmodel/uvmcollector.h \
    src/uvmmodel/uvmagent.h \
    src/uvmmodel/uvmpackage.h \
    src/uvmmodel/componenttype.h \
    src/uvmmodel/namespaceregister.h \
    src/uvmmodel/uvmsequence.h \
    src/uvmmodel/uvmproject.h \
    src/uvmmodel/uvmsignal.h \
    src/uvmmodel/uvmsequenceitem.h \
    src/uvmmodel/xmlvisitor.h \
    src/uvmmodel/uvmvisitor.h \
    src/uvmview/uvmvirtualsequencerview.h \
    src/uvmview/uvmverificationcomponentview.h \
    src/uvmview/uvmtopview.h \
    src/uvmview/uvmtestcaseview.h \
    src/uvmview/uvmtestbenchview.h \
    src/uvmview/uvmsequencerview.h \
    src/uvmview/uvmscoreboardview.h \
    src/uvmview/uvmportview.h \
    src/uvmview/uvmmonitorview.h \
    src/uvmview/uvmdriverview.h \
    src/uvmview/uvmdesignview.h \
    src/uvmview/uvmconnectionview.h \
    src/uvmview/uvmcomponentview.h \
    src/uvmview/uvmcollectorview.h \
    src/uvmview/uvmagentview.h \
    src/uvmview/uvminterfaceview.h \
    src/uvmview/packagefileaccessor.h \
    src/uvmview/configfileaccessor.h \
    src/uvmview/accessor.h \
    src/uvmview/sequenceitemaccessor.h \
    src/uvmview/sequenceaccessor.h \
    src/uvmview/graphicalprojectview.h \
    src/uvmview/headerfileaccessor.h \
    src/widget/renameprojectdialog.h \
    src/widget/filetreewidgetitem.h \
    src/widget/quicksearchwidget.h \
    src/widget/helpwidget.h \
    src/widget/filetreewidget.h \
    src/widget/filelineedit.h \
    src/widget/namelist.h \
    src/widget/stringlistmodel.h \
    src/widget/processconsole.h \
    src/widget/messageconsole.h \
    src/widget/nameandnumberlist.h \
    src/processing/uvexmlexport.h \
    src/processing/svexport.h \
    src/processing/uvexmlimport.h \
    src/parser/vhdl.lex \
    src/parser/vhdl.yacc \
    src/tests/newprojectwizardtester.h \
    src/tests/testgenerator.h \
    src/processing/svexportabstract.h \
    src/processing/namesetter.h \
    src/wizard/projectdialogview.h \
    src/wizard/envdialogview.h \
    src/wizard/vcdialogview.h \
    src/wizard/agentdialogview.h \
    src/wizard/signalsdialogview.h \
    src/wizard/wizarddataconverter.h \
    src/wizard/vipconnectordialog.h \
    src/wizard/newprojectwizard.h \
    src/wizard/viewdata.h \
    src/current_version.h \
    src/uveconstants.h \
    src/uvesettings.h \
    src/parser/vhdlclasses.h \
    src/widget/xmlhighlighter.h \
    src/widget/uvexmlvalidator.h \
    src/uvmview/otherfilesaccessor.h

SOURCES       = \
    src/main.cpp \
    src/parser/vhdlutils.cpp \
    src/parser/vhdlparser.cpp \
    src/parser/messagewidget.cpp \
    src/routing/routingboard.cpp \
    src/window/mdichild.cpp \
    src/window/mainwindow.cpp \
    src/uvmmodel/uvmvirtualsequencer.cpp \
    src/uvmmodel/uvmverificationcomponent.cpp \
    src/uvmmodel/uvmtop.cpp \
    src/uvmmodel/uvmtestcase.cpp \
    src/uvmmodel/uvmtestbench.cpp \
    src/uvmmodel/uvmsequencer.cpp \
    src/uvmmodel/uvmscoreboard.cpp \
    src/uvmmodel/uvmport.cpp \
    src/uvmmodel/uvmmonitor.cpp \
    src/uvmmodel/uvminterface.cpp \
    src/uvmmodel/uvmdriver.cpp \
    src/uvmmodel/uvmdesign.cpp \
    src/uvmmodel/uvmconnection.cpp \
    src/uvmmodel/uvmconfig.cpp \
    src/uvmmodel/uvmcomponent.cpp \
    src/uvmmodel/uvmcollector.cpp \
    src/uvmmodel/uvmagent.cpp \
    src/uvmmodel/uvmproject.cpp \
    src/uvmmodel/uvmsignal.cpp \
    src/uvmmodel/uvmsequenceitem.cpp \
    src/uvmmodel/namespaceregister.cpp \
    src/uvmmodel/uvmsequence.cpp \
    src/uvmmodel/uvmvisitor.cpp \
    src/uvmview/uvmvirtualsequencerview.cpp \
    src/uvmview/uvmverificationcomponentview.cpp \
    src/uvmview/uvmtopview.cpp \
    src/uvmview/uvmtestcaseview.cpp \
    src/uvmview/uvmtestbenchview.cpp \
    src/uvmview/uvmsequencerview.cpp \
    src/uvmview/uvmscoreboardview.cpp \
    src/uvmview/uvmportview.cpp \
    src/uvmview/uvmmonitorview.cpp \
    src/uvmview/uvmdriverview.cpp \
    src/uvmview/uvmdesignview.cpp \
    src/uvmview/uvmconnectionview.cpp \
    src/uvmview/uvmcomponentview.cpp \
    src/uvmview/uvmcollectorview.cpp \
    src/uvmview/uvmagentview.cpp \
    src/uvmview/uvminterfaceview.cpp \
    src/uvmview/packagefileaccessor.cpp \
    src/uvmview/configfileaccessor.cpp \
    src/uvmview/accessor.cpp \
    src/uvmview/sequenceitemaccessor.cpp \
    src/uvmview/sequenceaccessor.cpp \
    src/uvmview/graphicalprojectview.cpp \
    src/wizard/newprojectwizard.cpp \
    src/wizard/projectdialogview.cpp \
    src/wizard/envdialogview.cpp \
    src/wizard/vcdialogview.cpp \
    src/wizard/agentdialogview.cpp \
    src/wizard/signalsdialogview.cpp \
    src/wizard/vipconnectordialog.cpp \
    src/widget/filelineedit.cpp \
    src/wizard/wizarddataconverter.cpp \
    src/widget/namelist.cpp \
    src/widget/stringlistmodel.cpp \
    src/widget/processconsole.cpp \
    src/widget/messageconsole.cpp \
    src/widget/nameandnumberlist.cpp \
    src/widget/renameprojectdialog.cpp \
    src/widget/filetreewidgetitem.cpp \
    src/widget/quicksearchwidget.cpp \
    src/widget/helpwidget.cpp \
    src/widget/filetreewidget.cpp \
    src/uvmmodel/xmlvisitor.cpp \
    src/uvmview/headerfileaccessor.cpp \
    src/uvmmodel/uvmpackage.cpp \
    src/tests/newprojectwizardtester.cpp \
    src/tests/testgenerator.cpp \
    src/processing/uvexmlimport.cpp \
    src/processing/svexportabstract.cpp \
    src/processing/namesetter.cpp \
    src/processing/uvexmlexport.cpp \
    src/processing/svexport.cpp \
    src/uvesettings.cpp \
    src/widget/xmlhighlighter.cpp \
    src/widget/uvexmlvalidator.cpp \
    src/uvmview/otherfilesaccessor.cpp

RESOURCES     = uve.qrc

RC_FILE = images/logo.rc

LIBS         += -lqscintilla2

INCLUDEPATH += src/processing \
    src/uvmview \
    src/uvmmodel \
    src/uvmview \
    src/parser \
    src/routing \
    src/window \
    src/view \
    src/wizard \
    src/widget \
    src/tests \
    src/

OTHER_FILES += \
    templates/gve/wave.do \
    templates/gve/todo.txt \
    templates/gve/run.sh \
    templates/gve/run.bat \
    templates/gve/main.do \
    templates/gve/compile_vhdl.do \
    templates/gve/compile_sv.do \
    templates/gve/gvip/gvip_transfer.sv \
    templates/gve/gvip/gvip_slave_seq_lib.sv \
    templates/gve/gvip/gvip_pkg.sv \
    templates/gve/gvip/gvip_master_seq_lib.sv \
    templates/gve/gvip/gvip_if.sv \
    templates/gve/gvip/gvip_env.sv \
    templates/gve/gvip/gvip_bus_monitor.sv \
    templates/gve/gvip/gvip_agent_sequencer.sv \
    templates/gve/gvip/gvip_agent_seq_lib.sv \
    templates/gve/gvip/gvip_agent_monitor.sv \
    templates/gve/gvip/gvip_agent_driver.sv \
    templates/gve/gvip/gvip_agent_collector.sv \
    templates/gve/gvip/gvip_agent.sv \
    templates/gve/gtb/gtb_test_lib.sv \
    templates/gve/gtb/gtb_tb_top.sv \
    templates/gve/gtb/gtb_tb.sv \
    templates/gve/gtb/gtb_scoreboard.sv \
    templates/gve/gtb/gtb_master_seq_lib.sv \
    templates/gve/gvip/gvip_config.sv \
    templates/gve/gvip/gvip_seq_lib.sv

DEFINES += YY_NO_INPUT

win32{
FLEXSOURCES = src/parser/vhdl.lex
BISONSOURCES = src/parser/vhdl.yacc
flex.commands = flex ${QMAKE_FILE_IN} && move lex.yy.c src/parser/lex.yy.c
flex.input = FLEXSOURCES
flex.output = src/parser/lex.yy.c
flex.variable_out = SOURCES
flex.depends = src/parser/y.tab.h
flex.name = flex
flex.options += yylineno
QMAKE_EXTRA_COMPILERS += flex
bison.commands = bison \
    -d \
    -t \
    -y \
    ${QMAKE_FILE_IN} \
    && move y.tab.c src/parser/y.tab.cpp && move y.tab.h src/parser/y.tab.h
bison.input = BISONSOURCES
bison.output = src/parser/y.tab.cpp
bison.variable_out = SOURCES
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison
bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = src/parser/y.tab.h
bisonheader.variable_out = HEADERS
bisonheader.name = bison header
bisonheader.depends = src/parser/y.tab.cpp
QMAKE_EXTRA_COMPILERS += bisonheader
}

unix{
FLEXSOURCES = src/parser/vhdl.lex
BISONSOURCES = src/parser/vhdl.yacc
flex.commands = flex \
    ${QMAKE_FILE_IN} && mv lex.yy.c src/parser/lex.yy.c
flex.input = FLEXSOURCES
flex.output = src/parser/lex.yy.c
flex.variable_out = SOURCES
flex.depends = src/parser/y.tab.h
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex
bison.commands = bison \
    -d \
    -t \
    -y \
    ${QMAKE_FILE_IN} \
    && \
    mv \
    y.tab.c \
    src/parser/y.tab.cpp && mv y.tab.h src/parser/y.tab.h
bison.input = BISONSOURCES
bison.output = src/parser/y.tab.cpp
bison.variable_out = SOURCES
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison
bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = src/parser/y.tab.h
bisonheader.variable_out = HEADERS
bisonheader.name = bison \
    header
bisonheader.depends = src/parser/y.tab.cpp
QMAKE_EXTRA_COMPILERS += bisonheader
}

FORMS += \
    src/wizard/projectdialogview.ui \
    src/wizard/envdialogview.ui \
    src/wizard/vcdialogview.ui \
    src/wizard/agentdialogview.ui \
    src/wizard/signalsdialogview.ui \
    src/wizard/vipconnectordialog.ui \
    src/widget/renameprojectdialog.ui \
    src/widget/uvexmlvalidator.ui

TRANSLATIONS    = uve_fr.ts
