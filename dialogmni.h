#ifndef DIALOGADDMNI_H
#define DIALOGADDMNI_H

#include <QDialog>
#include <QProcess>
#include <QRegularExpression>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QValidator>
#include <QSqlQuery>
#include <QSqlError>
#include <QListWidgetItem>
#include <QVariant>
#include "mainwindow.h"
#include "db.h"


namespace Ui {
class DialogMni;
}

struct Flash {
    QString VID;
    QString PID;
    QString title;
    QString manufacturer;
    QString product;
    QString serial;
};

class DialogMni : public QDialog
{
    Q_OBJECT
  int m_deviceId = 0;

public:
    explicit DialogMni(QWidget *parent = 0, int deviceId = 0);
    ~DialogMni();
  int regWho();
  QString regNum();
  QString title();
  QString serial();
  QString manufacturer();
  QString product();
  QString vid();
  QString pid();
  QString comment();
  QSet<int> svtIds();

private slots:
    void on_actAutoLoadMNI_triggered();
    void colorizeValid(const QString& text);

    void on_buttonBox_accepted();

    void chbChanged();

    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_pbAddUser_clicked();

    void on_pbAddSVT_clicked();

private:
    Ui::DialogMni *ui;

    void initPersonCB();
    void initSvtList();

    QList<Flash> flashes;
    void fillFields(const Flash& flash);
    QPalette validPalette;
    QPalette invalidPalette;

    QSet<int> personSvt;
    QSet<int> divisionSvt;
    QSet<int> allSvt;
    QSet<int> manualCheckedSvt;
    QSet<int> resultSvt;

    void recheckSvtList();


};

#endif // DIALOGADDMNI_H
