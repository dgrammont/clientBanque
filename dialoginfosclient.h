#ifndef DIALOGINFOSCLIENT_H
#define DIALOGINFOSCLIENT_H

#include <QDialog>
#include <QStringList>
#include <QDebug>
#include "definition.h"



namespace Ui {
class DialogInfosClient;
}

class DialogInfosClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfosClient(QWidget *parent = nullptr);
    ~DialogInfosClient();
    typeClient obtenirStruc();
    void obtenirAgence(QString listAgance);

private slots:
    void on_buttonBox_accepted();

private:
        typeClient client;
    Ui::DialogInfosClient *ui;
};

#endif // DIALOGINFOSCLIENT_H
