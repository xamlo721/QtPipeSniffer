#ifndef PIPESOCKET_H
#define PIPESOCKET_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>

class PipeSocket : public QObject {

    Q_OBJECT

    private:
        QLocalSocket *m_socket;
        QTimer *m_reconnectTimer;
        QString m_pipeName;
        bool m_autoReconnect;
        int m_reconnectInterval;

    public:
        explicit PipeSocket(QObject *parent = nullptr);
        ~PipeSocket();

        // Основные методы
        bool connectToPipe(const QString &pipeName);
        void disconnectFromPipe();
        bool isConnected() const;
        QString pipeName() const;

        // Конфигурация
        void setAutoReconnect(bool enabled, int interval = 3000);
        void setPipeName(const QString &name);

    signals:
        // Сигналы состояния
        void connected();
        void disconnected();
        void errorOccurred(const QString &error);

        // Сигналы данных
        void dataReceived(const QByteArray &data);
        void rawDataReceived(const QString &text);
        void hexDataReceived(const QString &hex);

    public slots:
        void writeData(const QByteArray &data);

    private slots:
        void handleReadyRead();
        void handleError(QLocalSocket::LocalSocketError error);
        void attemptReconnect();

};

#endif // PIPESOCKET_H
