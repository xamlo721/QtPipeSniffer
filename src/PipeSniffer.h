#ifndef PIPESNIFFER_H
#define PIPESNIFFER_H

#include <QApplication>
#include "PipeSocket.h"

class PipeSniffer : public QApplication {

    Q_OBJECT

    private:
        PipeSocket m_pipeSocket_src;
        PipeSocket m_pipeSocket_dst;

    public:
        PipeSniffer(int argc, char *argv[]);


    private:
        void onPipeReadData(const QByteArray &data, bool isSrcSide);

        QString splitByTwoSymbols(QString &str) const;

    signals:
        void signalOnDataRecieved(const QString & data);

    public slots:
        void slotOnPipeConnect(QString pipeName_src, QString pipeName_dst);

    private slots:
        void slotOnSrcPipeReadData(const QByteArray &data);
        void slotOnDstPipeReadData(const QByteArray &data);


};

#endif // PIPESNIFFER_H
