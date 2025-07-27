#include "PipeSocket.h"

#include <QDebug>

PipeSocket::PipeSocket(QObject *parent) : QObject(parent),
    m_socket(new QLocalSocket(this)),
    m_reconnectTimer(new QTimer(this)),
    m_autoReconnect(true),
    m_reconnectInterval(3000)
{
    // Настройка таймера переподключения
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &PipeSocket::attemptReconnect);

    // Подключение сигналов сокета
    connect(m_socket, &QLocalSocket::readyRead, this, &PipeSocket::handleReadyRead);
    connect(m_socket, &QLocalSocket::connected, this, [this]() {
        m_reconnectTimer->stop();
        emit connected();
    });

    connect(m_socket, &QLocalSocket::disconnected, this, [this]() {
        emit disconnected();
        if (m_autoReconnect) {
            m_reconnectTimer->start(m_reconnectInterval);
        }
    });

    connect(m_socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), this, &PipeSocket::handleError);
}

PipeSocket::~PipeSocket() {

    disconnectFromPipe();
}

bool PipeSocket::connectToPipe(const QString &pipeName) {

    if (!pipeName.isEmpty()) {
        m_pipeName = pipeName;
    }

    if (m_pipeName.isEmpty()) {
        emit errorOccurred("Pipe name is not set");
        return false;
    }

    if (isConnected()) {
        disconnectFromPipe();
    }

    m_socket->connectToServer(m_pipeName);

    // Ожидание подключения в течение 1 секунды
    if (m_socket->waitForConnected(1000)) {
        return true;
    }

    emit errorOccurred("Connection failed: " + m_socket->errorString());
    return false;
}

void PipeSocket::disconnectFromPipe() {

    if (m_socket->state() != QLocalSocket::UnconnectedState) {
        m_socket->disconnectFromServer();
        if (m_socket->state() != QLocalSocket::UnconnectedState) {
            m_socket->waitForDisconnected(500);
        }
    }
    m_reconnectTimer->stop();
}

bool PipeSocket::isConnected() const {

    return m_socket->state() == QLocalSocket::ConnectedState;
}

QString PipeSocket::pipeName() const {

    return m_pipeName;
}

void PipeSocket::setAutoReconnect(bool enabled, int interval) {

    m_autoReconnect = enabled;
    m_reconnectInterval = interval;
}

void PipeSocket::setPipeName(const QString &name) {

    if (isConnected()) {
        disconnectFromPipe();
    }
    m_pipeName = name;
}

void PipeSocket::writeData(const QByteArray &data) {

    if (!isConnected()) {
        emit errorOccurred("Cannot write - not connected to pipe");
        return;
    }

    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(1000)) {
        emit errorOccurred("Write failed: " + m_socket->errorString());
    }
}

void PipeSocket::handleReadyRead() {

    if (!m_socket->bytesAvailable()) return;

    QByteArray data = m_socket->readAll();
    emit dataReceived(data);

    // Текстовое представление
    QString textData = QString::fromLatin1(data);
    emit rawDataReceived(textData);

    // HEX представление
    QString hexData;
    hexData.reserve(data.size() * 3);
    for (char byte : data) {
        hexData += QString("%1 ").arg(static_cast<quint8>(byte), 2, 16, QLatin1Char('0')).toUpper();
    }
    emit hexDataReceived(hexData);
}

void PipeSocket::handleError(QLocalSocket::LocalSocketError error) {

    QString errorMsg;

    switch (error) {

        case QLocalSocket::ServerNotFoundError:
            errorMsg = "Pipe server not found: " + m_pipeName;
            break;

        case QLocalSocket::ConnectionRefusedError:
            errorMsg = "Connection refused for pipe: " + m_pipeName;
            break;

        case QLocalSocket::PeerClosedError:
            errorMsg = "Connection closed by peer for pipe: " + m_pipeName;
            break;

        default:
            errorMsg = "Pipe error [" + QString::number(error) + "]: " + m_socket->errorString();
    }

    emit errorOccurred(errorMsg);

    // Автопереподключение при ошибках соединения
    if (m_autoReconnect &&
        (error == QLocalSocket::ServerNotFoundError ||
         error == QLocalSocket::ConnectionRefusedError ||
         error == QLocalSocket::PeerClosedError)) {
        m_reconnectTimer->start(m_reconnectInterval);
    }

}

void PipeSocket::attemptReconnect() {

    if (!isConnected() && !m_pipeName.isEmpty()) {
        qDebug() << "Attempting to reconnect to" << m_pipeName;
        m_socket->connectToServer(m_pipeName);
    }

}
