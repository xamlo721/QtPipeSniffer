#include "PipeSniffer.h"

#include <QRegularExpression>
#include <QTimer>

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

    // Сплитим строку по байту
    hexData = this->splitHexByte(hexData);

    const QString srcSideStr = "[" + this->m_pipeSocket_src.pipeName() + " --> " + this->m_pipeSocket_dst.pipeName() + "]    ";
    const QString dstSideStr = "[" + this->m_pipeSocket_src.pipeName() + " <-- " + this->m_pipeSocket_dst.pipeName() + "]    ";
    hexData.prepend(isSrcSide ? srcSideStr : dstSideStr);
    emit signalOnDataRecieved(hexData);
}


void PipeSniffer::slotOnPipeConnect(QString pipeName_src, QString pipeName_dst) {

    // Если мы подключены
    if(this->connectFlag) {

        this->connectFlag = false;

        this->m_pipeSocket_src.disconnectFromPipe();
        this->m_pipeSocket_dst.disconnectFromPipe();

        emit signalManageButtonText("Connect");

        // Выключим кнопку и поставим таймер на включение
        emit signalManageButtonState(false);
        QTimer::singleShot(1000, this, &PipeSniffer::slotOnManageButtonDisableTimeout);

        emit signalConnectionStatusChanged(EnumConnectionStatus::Disconnected);

        emit signalOnDataRecieved("Pipe src : " + pipeName_src + " disconnected");
        emit signalOnDataRecieved("Pipe dst : " + pipeName_dst + " disconnected");
        emit signalOnDataRecieved("Successfull disconnect pipes!");

        return;
    }


    // Выключим кнопку и поставим таймер на включение
    emit signalManageButtonState(false);
    QTimer::singleShot(1000, this, &PipeSniffer::slotOnManageButtonDisableTimeout);

    m_pipeSocket_src.setPipeName(pipeName_src);
    m_pipeSocket_src.setAutoReconnect(true, 3000);

    bool stupidFlagSrc = false, stupidFlagDst = false;
    // Подключение к каналу
    stupidFlagSrc = m_pipeSocket_src.connectToPipe(pipeName_src);
    if (!stupidFlagSrc) {
        emit signalOnDataRecieved("Failed to connect to src pipe: " + pipeName_src);
    } else {
        emit signalOnDataRecieved("Pipe src : " + pipeName_src + " connected");
    }

    m_pipeSocket_dst.setPipeName(pipeName_dst);
    m_pipeSocket_dst.setAutoReconnect(true, 3000);

    // Подключение к каналу
    stupidFlagDst = m_pipeSocket_dst.connectToPipe(pipeName_dst);
    if (!stupidFlagDst) {
        emit signalOnDataRecieved("Failed to connect to dst pipe: " + pipeName_dst);
    } else {
        emit signalOnDataRecieved("Pipe dst : " + pipeName_dst + " connected");
    }


    // Установка статуса
    if(!stupidFlagSrc || !stupidFlagDst) {
        m_pipeSocket_src.disconnectFromPipe();
        m_pipeSocket_dst.disconnectFromPipe();
        emit signalConnectionStatusChanged(EnumConnectionStatus::Disconnected);
        emit signalOnDataRecieved("Failed to connect pipes! Connection not set!");
    } else {
        this->connectFlag = true;
        emit signalConnectionStatusChanged(EnumConnectionStatus::Connected);
        emit signalManageButtonText("Disconnect");
        emit signalOnDataRecieved("Successfull connect pipes!");
    }
}

/**
 * @brief splitHexByte - метод разделения 16-тиричной строки по байтам
 * @param str - 16-тиричная строка
 * @return Разделенная по байтам 16-тиричная строка
 */
QString PipeSniffer::splitHexByte(QString &str) const {
    return str.replace(QRegularExpression("(.{2})(?!$)"), "\\1 ");
}

/**
 * @brief slotOnManageButtonDisableTimeout - слот для обработки таймаута выключения кнопки управления соединением
 */
void PipeSniffer::slotOnManageButtonDisableTimeout() {
    emit signalManageButtonState(true);
}
