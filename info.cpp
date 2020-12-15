#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

}

Info::~Info()
{
    delete ui;
}



void Info::on_pushButton_clicked()
{
  nom = ui->lineEditNom->text();
  prenom = ui->lineEditPrenom->text();
  ville = ui->lineEditVille->text();
  idAgence = ui->comboBoxidAgance->currentIndex()+1;
  emit InfoEnvoie();


}
