#ifndef DIALOGPERSON_H
#define DIALOGPERSON_H

#include <QDialog>
#include <QPalette>
#include <QValidator>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "db.h"
namespace Ui {
  class DialogPerson;
}

class DialogPerson : public QDialog
{
  Q_OBJECT
  int m_personId = 0;
public:
  explicit DialogPerson(QWidget *parent = 0, int personId = 0);
  ~DialogPerson();
  QString fam();
  QString name();
  QString otch();
  QString comment();
  QSet<int> divisonIds();
private slots:
  void on_buttonBox_accepted();
  void colorizeEdit(const QString& text);


  void on_pbAddDivision_clicked();

private:
  Ui::DialogPerson *ui;
  QPalette   validPalette = palette();
  QPalette invalidPalette = palette();
  void initDivisionList();

};

#endif // DIALOGPERSON_H
