#ifndef INFO_H
#define INFO_H

#include <QWidget>

namespace Ui {
class Info;
}

class Info : public QWidget
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();

    QString nom;
    QString prenom;
    QString ville;
    int idAgence;

signals:
    void InfoEnvoie();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Info *ui;



};

#endif // INFO_H
