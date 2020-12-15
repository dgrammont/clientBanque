#include "dialoginfosclient.h"
#include "ui_dialoginfosclient.h"

DialogInfosClient::DialogInfosClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfosClient)
{
    ui->setupUi(this);

}

DialogInfosClient::~DialogInfosClient()
{
    delete ui;
}

typeClient DialogInfosClient::obtenirStruc()
{
    return client;
}

void DialogInfosClient::obtenirAgence(QString _listAgance)
{
    QStringList listDesAgance;
    listDesAgance = _listAgance.split(QLatin1Char(';'));
    qDebug()<<listDesAgance;
    for(int i=0;i<listDesAgance.length();i++){
        ui->comboBoxidAgance->addItem(listDesAgance.value(i));
    }

}

void DialogInfosClient::on_buttonBox_accepted()
{
    client.nom = ui->lineEditNom->text();
    client.prenom = ui->lineEditPrenom->text();
    client.ville = ui->lineEditVille->text();
    client.agence = ui->comboBoxidAgance->currentIndex()+1;
}


