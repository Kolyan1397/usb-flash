#include "dialogdivision.h"
#include "ui_dialogdivision.h"

DialogDivision::DialogDivision(QWidget *parent, int divisionId) :
  QDialog(parent),
  m_divisionId(divisionId),
  ui(new Ui::DialogDivision)
{
  ui->setupUi(this);
  if (m_divisionId) {
    setWindowTitle("Редактирование подразделения");
    QSqlQuery query("select * from division where id = "+QString::number(m_divisionId));
    if (query.next()) {
      ui->leTitle->setText(query.value("title").toString());
      ui->leComment->setText(query.value("comment").toString());
    }
  }
}

DialogDivision::~DialogDivision()
{
  delete ui;
}

int DialogDivision::divisionId()
{
  return m_divisionId;
}

QString DialogDivision::title()
{
  return ui->leTitle->text();
}

QString DialogDivision::comment()
{
  return ui->leComment->text();
}

void DialogDivision::on_buttonBox_accepted()
{
  if (ui->leTitle->text().isEmpty()) {
    QMessageBox::critical(0,"Ошибка","Не указано наименование подразделения.");
    ui->leTitle->setFocus();
    return;
  }
  accept();
}
