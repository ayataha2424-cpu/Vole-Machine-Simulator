// GUI layer for the Vole machine simulator (Bonus Task 3).
// Written with AI assistance (allowed for the GUI). The core files are NOT modified.

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
