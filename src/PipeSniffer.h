#ifndef PIPESNIFFER_H
#define PIPESNIFFER_H

#include <QApplication>
#include "PipeSocket.h"

class PipeSniffer : public QApplication {

    Q_OBJECT

    private:

        QString pipeName_src;
        QString pipeName_dst;
        PipeSocket m_pipeSocket_src;
        PipeSocket m_pipeSocket_dst;

    public:
        PipeSniffer(int argc, char *argv[]);


    signals:
        void signalOnDataRecieved(const QByteArray & data);

    public slots:
        void slotOnPipeConnect(QString pipeName_src, QString pipeName_dst);


};

#endif // PIPESNIFFER_H
