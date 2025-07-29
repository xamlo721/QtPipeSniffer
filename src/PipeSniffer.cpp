#include "PipeSniffer.h"

PipeSniffer::PipeSniffer(int argc, char *argv[])
    : QApplication(argc, argv) {

    //log src
    QObject::connect(&m_pipeSocket_src, &PipeSocket::dataReceived, this, &PipeSniffer::slotOnSrcPipeReadData);

    //log dst
    QObject::connect(&m_pipeSocket_dst, &PipeSocket::dataReceived, this, &PipeSniffer::slotOnDstPipeReadData);

    //swap
    QObject::connect(&m_pipeSocket_src, &PipeSocket::dataReceived, &m_pipeSocket_dst, &PipeSocket::writeData);

    //swap
    QObject::connect(&m_pipeSocket_dst, &PipeSocket::dataReceived, &m_pipeSocket_src, &PipeSocket::writeData);
}


void PipeSniffer::slotOnSrcPipeReadData(const QByteArray &data) {
    this->onPipeReadData(data, true);
}

void PipeSniffer::slotOnDstPipeReadData(const QByteArray &data) {
    this->onPipeReadData(data, false);
}

void PipeSniffer::onPipeReadData(const QByteArray &data, bool isSrcSide) {

    QString hexData = data.toHex();

    hexData.prepend(isSrcSide ? " --> " : " <-- ");
    emit signalOnDataRecieved(hexData);
}


void PipeSniffer::slotOnPipeConnect(QString pipeName_src, QString pipeName_dst) {

    m_pipeSocket_src.setPipeName(pipeName_src);
    m_pipeSocket_src.setAutoReconnect(true, 3000);

    // Подключение к каналу
    if (!m_pipeSocket_src.connectToPipe(pipeName_src)) {
        qDebug() << "Failed to connect to src pipe: " << pipeName_src;
    } else {
        qDebug() << "Pipe src : " << pipeName_src << " connected";
    }

    m_pipeSocket_dst.setPipeName(pipeName_dst);
    m_pipeSocket_dst.setAutoReconnect(true, 3000);

    // Подключение к каналу
    if (!m_pipeSocket_dst.connectToPipe(pipeName_dst)) {
        qDebug() << "Failed to connect to dst pipe: " << pipeName_dst;
    } else {
        qDebug() << "Pipe src : " << pipeName_dst << " connected";
    }
}

