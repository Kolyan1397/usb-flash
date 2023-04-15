#include "dialogperson.h"
#include "ui_dialogperson.h"

DialogPerson::DialogPerson(QWidget *parent, int personId) :
  QDialog(parent),
  m_personId(personId),
  ui(new Ui::DialogPerson)
{
  ui->setupUi(this);
  if (personId > 0 ) {
    setWindowTitle("Редактирование сведений об ответсвенном лице");
  } else {
    setWindowTitle("Добавление ответственного лица");
  }
  QRegExpValidator* validator = new QRegExpValidator(QRegExp("[А-ЯЁ][А-ЯЁа-яё]+"));
  validPalette.setBrush(QPalette::Base,QBrush(QColor(0xA0,0xFF,0xA0)));
  invalidPalette.setBrush(QPalette::Base,QBrush(QColor(0xFF,0xA0,0xA0)));
  ui->leFam->setValidator(validator);
  ui->leName->setValidator(validator);
  ui->leOtch->setValidator(validator);
  initDivisionList();
  if (personId) {
    QSqlQuery query("select * from person where id="+QString::number(personId));
    if (query.next()) {
      ui->leFam->setText(query.value("fam").toString());
      ui->leName->setText(query.value("name").toString());
      ui->leOtch->setText(query.value("otch").toString());
      ui->leComment->setText(query.value("comment").toString());
    }
  }
}

DialogPerson::~DialogPerson()
{
  delete ui;
}

QString DialogPerson::fam()
{
  return ui->leFam->text();
}

QString DialogPerson::name()
{
  return ui->leName->text();
}

QString DialogPerson::otch()
{
  return ui->leOtch->text();
}

QString DialogPerson::comment()
{
  return ui->leComment->text();
}

QSet<int> DialogPerson::divisonIds()
{
  QSet<int> res;
  for (int i = 0; i < ui->listWidget->count(); ++i) {
    QListWidgetItem * item = ui->listWidget->item(i);
    if (item->checkState() == Qt::Checked) {
      res.insert(item->data(Qt::UserRole).toInt());
    }
  }
  return res;
}

void DialogPerson::on_buttonBox_accepted()
{
  QString text = fam();
  int pos;
  if (ui->leFam->validator()->validate(text,pos)!=QValidator::Acceptable) {
    QMessageBox::critical(0,"Ошибка добавления","Не верно указана фамилия.");
    ui->leFam->setFocus();
    return;
  }
  text = name();
  if (ui->leName->validator()->validate(text,pos)!=QValidator::Acceptable) {
    QMessageBox::critical(0,"Ошибка добавления","Не верно указано имя.");
    ui->leName->setFocus();
    return;
  }
  text = otch();
  if (ui->leOtch->validator()->validate(text,pos)!=QValidator::Acceptable) {
    QMessageBox::critical(0,"Ошибка добавления","Не верно указано отчество.");
    ui->leOtch->setFocus();
    return;
  }
  accept();
}

void DialogPerson::colorizeEdit(const QString &text)
{
  QString _text = text;
  if (QLineEdit *le = qobject_cast<QLineEdit*>(sender())) {
    if (le->validator()) {
      int pos = 0;
      if (le->validator()->validate(_text,pos)==QValidator::Acceptable) {
        le->setPalette(validPalette);
      } else {
        le->setPalette(invalidPalette);
      }
    }
  }
}

void DialogPerson::on_pbAddDivision_clicked()
{
  if (DB::addDivision()) {
    initDivisionList();
  }
}

void DialogPerson::initDivisionList()
{
  ui->listWidget->clear();
  QSqlQuery query("select * from division;");
  while(query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20)
      comment = comment.left(17)+"...";
    QString title = query.value("title").toString();
    QString itemText = QString("%1 (%2)").arg(title).arg(comment);
    QListWidgetItem *item = new QListWidgetItem(itemText,ui->listWidget);
    item->setData(Qt::UserRole,query.value("id"));
    item->setFlags(Qt::ItemIsUserCheckable|item->flags());
    item->setCheckState(Qt::Unchecked);
  }
  if (m_personId) {
    query.exec("select * from person_to_division where person_id="+QString::number(m_personId));
    QSet<int> divisonIds;
    while (query.next()) {
      divisonIds.insert(query.value("division_id").toInt());
    }
    for (int i = 0; i < ui->listWidget->count(); ++i) {
      QListWidgetItem * item = ui->listWidget->item(i);
      if (divisonIds.contains(item->data(Qt::UserRole).toInt())) {
        item->setCheckState(Qt::Checked);
      } else {
        item->setCheckState(Qt::Unchecked);
      }
    }
  }
}
