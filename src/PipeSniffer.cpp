#include "PipeSniffer.h"

PipeSniffer::PipeSniffer(int argc, char *argv[])
    : QApplication(argc, argv) {

    QObject::connect(&m_pipeSocket, &PipeSocket::dataReceived, this, &PipeSniffer::signalOnDataRecieved);


}


void PipeSniffer::slotOnPipeConnect(QString pipeName) {

    this->pipeName = pipeName;

    m_pipeSocket.setPipeName(pipeName);
    m_pipeSocket.setAutoReconnect(true, 3000);


    // Подключение к каналу
    if (!m_pipeSocket.connectToPipe(pipeName)) {
        qDebug() << "Failed to connect to pipe: " << pipeName;
    } else {
        qDebug() << "Pipe: " << pipeName << " connected";
    }
}

