#include "MainWindow.h"

#include <QApplication>
#include "PipeSocket.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

//    MainWindow w;

//    w.show();

    QString pipeName = "\\\\.\\pipe\\vmware_pipe_0";

    PipeSocket m_pipeSocket;
    m_pipeSocket.setPipeName(pipeName);
    m_pipeSocket.setAutoReconnect(true, 3000);

    // Подключение к каналу
    if (!m_pipeSocket.connectToPipe(pipeName)) {
        qDebug() << "Failed to connect to pipe: " << pipeName;
    } else {
        qDebug() << "Pipe: " << pipeName << " connected";
    }

    return a.exec();
}
