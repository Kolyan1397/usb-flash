#ifndef DIALOGDIVISION_H
#define DIALOGDIVISION_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
  class DialogDivision;
}

class DialogDivision : public QDialog
{
  Q_OBJECT
  int m_divisionId = 0;
public:
  explicit DialogDivision(QWidget *parent = 0, int divisionId = 0);
  ~DialogDivision();

  int divisionId();
  QString title();
  QString comment();

private slots:
  void on_buttonBox_accepted();

private:
  Ui::DialogDivision *ui;
};

#endif // DIALOGDIVISION_H
