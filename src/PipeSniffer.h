#ifndef PIPESNIFFER_H
#define PIPESNIFFER_H

#include <QApplication>
#include "PipeSocket.h"

class PipeSniffer : public QApplication {

    Q_OBJECT

    private:

        QString pipeName = "\\\\.\\pipe\\vmware_pipe_0";
        PipeSocket m_pipeSocket;

    public:
        PipeSniffer(int argc, char *argv[]);


    signals:
        void signalOnDataRecieved(const QByteArray & data);

    public slots:
        void slotOnPipeConnect(QString pipeName);


};

#endif // PIPESNIFFER_H
