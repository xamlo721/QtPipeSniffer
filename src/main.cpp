#include "MainWindow.h"

#include "PipeSniffer.h"

int main(int argc, char *argv[]) {

    PipeSniffer application(argc, argv);

    MainWindow w;

    w.show();

    QObject::connect(&application, &PipeSniffer::signalOnDataRecieved, &w, &MainWindow::displaySnifferData);

    QObject::connect(&w, &MainWindow::signalOnPipeConnectRequest, &application, &PipeSniffer::slotOnPipeConnect);

    return application.exec();
}
