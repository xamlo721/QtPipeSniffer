#include "MainWindow.h"

#include "PipeSniffer.h"

int main(int argc, char *argv[]) {

    PipeSniffer application(argc, argv);

    MainWindow w;

    w.show();

    QObject::connect(&application, &PipeSniffer::signalOnStatusHandled, &w, &MainWindow::setConnectStatus);
    QObject::connect(&application, &PipeSniffer::signalManageButtonState, &w, &MainWindow::setManageButtonState);
    QObject::connect(&application, &PipeSniffer::signalManageButtonText, &w, &MainWindow::setManageButtonText);
    QObject::connect(&application, &PipeSniffer::signalOnDataRecieved, &w, &MainWindow::displaySnifferData);

    QObject::connect(&w, &MainWindow::signalOnPipeConnectRequest, &application, &PipeSniffer::slotOnPipeConnect);

    return application.exec();
}
