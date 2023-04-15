#ifndef DIALOGSVT_H
#define DIALOGSVT_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSet>
#include <QProcess>
#include "db.h"
#include <QFileInfo>
#include <QDir>
namespace Ui {
  class DialogSVT;
}

class DialogSVT : public QDialog
{
  Q_OBJECT
  int m_svtId = 0;

public:
  explicit DialogSVT(QWidget *parent = 0, int svtId= 0);
  ~DialogSVT();
  int svtId();
  QString title();
  QString address();
  QString user();
  QString password();
  int personId();
  QString comment();

  QSet<int> divisonIds();
private slots:
  void on_buttonBox_accepted();

  void on_pushButton_clicked();

  void on_pbAddDivision_clicked();

  void on_pbSshAddKey_clicked();

private:
  Ui::DialogSVT *ui;
  void initPerson();
  void initDivisionList();
};

#endif // DIALOGSVT_H
