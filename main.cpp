#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Worksheet");

    MainWindow window;
    window.show();

    return application.exec();
}
