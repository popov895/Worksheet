QT += \
    core \
    gui \
    widgets

TARGET = Worksheet

TEMPLATE = app

DEFINES += \
    QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    TableModel.cpp \
    InsertColumnsDialog.cpp \
    InsertRowsDialog.cpp \
    NewTableDialog.cpp \
    CsvDocument.cpp

HEADERS += \
    MainWindow.h \
    TableModel.h \
    InsertColumnsDialog.h \
    InsertRowsDialog.h \
    NewTableDialog.h \
    CsvDocument.h

FORMS += \
    MainWindow.ui \
    InsertColumnsDialog.ui \
    InsertRowsDialog.ui \
    NewTableDialog.ui

RESOURCES += \
    resources.qrc

win32 {
    RC_FILE = Worksheet.rc
}
