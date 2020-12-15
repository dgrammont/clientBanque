#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialoginfosclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)


{
    socketClientBanque = new QTcpSocket;
    ui->setupUi(this);
    ui->groupBox_ope->setEnabled(false);
    socketClientBanque = new QTcpSocket;
    connect(socketClientBanque,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,
            &MainWindow::onTcpSocket_error);
    connect(socketClientBanque,&QTcpSocket::connected,this,&MainWindow::onTcpSocket_conneted);
    connect(socketClientBanque,&QTcpSocket::disconnected,this,&MainWindow::onTcpSocket_disconnected);
    connect(socketClientBanque,&QTcpSocket::stateChanged,this,&MainWindow::onTcpSocket_stateChanged);
    connect(socketClientBanque,&QTcpSocket::aboutToClose,this,&MainWindow::onTcpSocket_aboutToClose);
    connect(socketClientBanque,&QTcpSocket::bytesWritten,this,&MainWindow::onTcpSocket_bytesWritten);
    connect(socketClientBanque,&QTcpSocket::readChannelFinished,this,&MainWindow::onTcpSocket_readChannelFinished);
    connect(socketClientBanque,&QTcpSocket::readyRead,this,&MainWindow::onTcpSocket_readyRead);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete  socketClientBanque;


}



void MainWindow::EnvoiedinformationStruc()
{
    qDebug()<<this<<"Y";
    QChar commande='Y';

    clientInfoStruc.numCompte = ui->lineEdit_numcompt->text().toInt();

    quint16 taille=0;
    typeClient message;
    QBuffer tampon;

    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);
    //construction de la trame
    out<<taille<<commande;
    out << clientInfoStruc.nom
        << clientInfoStruc.prenom
        << clientInfoStruc.ville
        << clientInfoStruc.agence
        << clientInfoStruc.numCompte;
    //calcul de la taille de la trame
    taille=tampon.size()-sizeof(taille);
    //placement sur la première postion du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille reel da la trame
    out<<taille;
    //envoi du QByteArray du tampon via la socket
    socketClientBanque->write(tampon.buffer());

}

void MainWindow::DemanderAgances()
{

    QChar commande;
    quint16 taille=0;
    QString message;
    QBuffer tampon;

    commande='A';
    qDebug()<<this<<commande;
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);
    //construction de la trame
    out<<taille<<commande;
    //calcul de la taille de la trame
    taille=tampon.size()-sizeof(taille);
    //placement sur la première postion du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille reel da la trame
    out<<taille;
    //envoi du QByteArray du tampon via la socket
    socketClientBanque->write(tampon.buffer());

}



void MainWindow::on_pushButton_connexion_clicked()
{
    if(ui->pushButton_connexion->text()=="Connexion"){
        socketClientBanque->connectToHost(ui->lineEdit_adresse->text(),ui->lineEdit_port->text().toUInt());
    }
    else {
        socketClientBanque->disconnectFromHost();

    }
}

void MainWindow::onTcpSocket_conneted()
{
    ui->listWidget_etat->addItem("connecté");
    ui->groupBox_ope->setEnabled(true);
    ui->lineEdit_port->setEnabled(false);
    ui->lineEdit_adresse->setEnabled(false);
    ui->pushButton_connexion->setText("Deconnexion");
    ui->lineEdit_montant->setEnabled(false);
    ui->pushButton_envoi->setEnabled(false);
    ui->radioButton_depot->setEnabled(false);
    ui->radioButton_solde->setEnabled(false);
    ui->radioButton_retrait->setEnabled(false);


}

void MainWindow::onTcpSocket_disconnected()
{
    ui->listWidget_etat->addItem("déconnecté");
    ui->groupBox_ope->setEnabled(false);
    ui->lineEdit_adresse->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    ui->pushButton_connexion->setText("Connexion");
    ui->lineEdit_montant->setText("");
    ui->lineEdit_numcompt->setText("");
    ui->lineEdit_message->setText("");


}

void MainWindow::onTcpSocket_error(QAbstractSocket::SocketError socketError)
{
    ui->listWidget_etat->addItem(socketClientBanque->errorString());
}

void MainWindow::onTcpSocket_hostFoud()
{
    ui->listWidget_etat->addItem("hôte trouvé");
}

void MainWindow::onTcpSocket_stateChanged(QAbstractSocket::SocketState socketState)
{
    ui->listWidget_etat->addItem("état changé");
}

void MainWindow::onTcpSocket_aboutToClose()
{
    ui->listWidget_etat->addItem("close");
}

void MainWindow::onTcpSocket_bytesWritten(quint64 bytes)
{
    ui->listWidget_etat->addItem("écrire");
}

void MainWindow::onTcpSocket_readChannelFinished()
{
    ui->listWidget_etat->addItem("fin de la communication");
}

void MainWindow::onTcpSocket_readyRead()
{
    qDebug()<<this<<"lecture";
    quint16 taille=0;
    QString message;
    QChar commade;

    if(socketClientBanque->bytesAvailable()>=(quint64)sizeof(taille)){
        QDataStream in (socketClientBanque);

        in>>taille;

        if(socketClientBanque->bytesAvailable() >= (quint64)taille){
            in>>commade;
            in>>message;
        }
    }
    qDebug()<<this<<commade;
    qDebug()<<this<<message;

    if(commade !='A'){
        ui->lineEdit_message->setText(message);
    }
    if(commade == 'A'){
        ui->lineEdit_message->setText("Donnez les information suivante : id d'agence  nom prenom  ville ");
        DialogInfosClient dlgInfo;
        dlgInfo.obtenirAgence(message);
        if(dlgInfo.exec()==QDialog::Accepted)
        {
            clientInfoStruc = dlgInfo.obtenirStruc();
            EnvoiedinformationStruc();
        }

    }
}

void MainWindow::on_pushButton_envoi_clicked()
{
    QChar commande;
    quint16 taille=0;
    QString message;
    QBuffer tampon;
    float val = ui->lineEdit_montant->text().toFloat();

    if(ui->radioButton_solde->isChecked()){
        commande='S';
        tampon.open(QIODevice::WriteOnly);
        //association du tampon au flux de sortie
        QDataStream out(&tampon);
        //construction de la trame
        out<<taille<<commande;
        //calcul de la taille de la trame
        taille=tampon.size()-sizeof(taille);
        //placement sur la première postion du flux pour pouvoir modifier la taille
        tampon.seek(0);
        //modification de la trame avec la taille reel da la trame
        out<<taille;
        //envoi du QByteArray du tampon via la socket
        socketClientBanque->write(tampon.buffer());
    }
    if(val>0){
        if(ui->radioButton_depot->isChecked()){
            commande='D';
        }
        if(ui->radioButton_retrait->isChecked()){
            commande='R';
        }
        tampon.open(QIODevice::WriteOnly);
        //association du tampon au flux de sortie
        QDataStream out(&tampon);
        //construction de la trame
        out<<taille<<commande<<val;
        //calcul de la taille de la trame
        taille=tampon.size()-sizeof(taille);
        //placement sur la première postion du flux pour pouvoir modifier la taille
        tampon.seek(0);
        //modification de la trame avec la taille reel da la trame
        out<<taille;
        //envoi du QByteArray du tampon via la socket
        socketClientBanque->write(tampon.buffer());
    }else{
        ui->lineEdit_message->setText("La valeur saisie est négatif ou n'est pas un nombre");
    }

}

void MainWindow::on_pushButton_numcompt_clicked()
{
    ui->listWidget_etat->addItem("numéro de compte");
    quint16 taille=0;
    QString message;
    QChar commande='N';
    int compte = ui->lineEdit_numcompt->text().toInt();
    QBuffer tampon;
    tampon.open(QIODevice::WriteOnly);
    //association du tampon au flux de sortie
    QDataStream out(&tampon);
    //construction de la trame
    out<<taille<<commande<<compte;
    //calcul de la taille de la trame
    taille=tampon.size()-sizeof(taille);
    //placement sur la première postion du flux pour pouvoir modifier la taille
    tampon.seek(0);
    //modification de la trame avec la taille reel da la trame
    out<<taille;
    //envoi du QByteArray du tampon via la socket
    socketClientBanque->write(tampon.buffer());

    ui->lineEdit_montant->setEnabled(true);
    ui->pushButton_envoi->setEnabled(true);
    ui->radioButton_depot->setEnabled(true);
    ui->radioButton_solde->setEnabled(true);
    ui->radioButton_retrait->setEnabled(true);


}
