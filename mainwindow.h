#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QBuffer>
#include <QDataStream>
#include <QHostAddress>
#include <qregexp.h>
#include <QThread>
#include <info.h>
#include "definition.h"
#include "dialoginfosclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void EnvoiedinformationStruc();
    void DemanderAgances();


private slots:
    void on_pushButton_connexion_clicked();

    void onTcpSocket_conneted();
    void onTcpSocket_disconnected();
    void onTcpSocket_error(QAbstractSocket::SocketError socketError);
    void onTcpSocket_hostFoud();
    void onTcpSocket_stateChanged(QAbstractSocket::SocketState socketState);
    void onTcpSocket_aboutToClose();
    void onTcpSocket_bytesWritten(quint64 bytes);
    void onTcpSocket_readChannelFinished();
    void onTcpSocket_readyRead();


    void on_pushButton_envoi_clicked();

    void on_pushButton_numcompt_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socketClientBanque;

    typeClient clientInfoStruc;

};
#endif // MAINWINDOW_H
