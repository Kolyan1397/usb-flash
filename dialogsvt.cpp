#include "dialogsvt.h"
#include "ui_dialogsvt.h"

DialogSVT::DialogSVT(QWidget *parent, int svtId) :
  QDialog(parent),
  m_svtId(svtId),
  ui(new Ui::DialogSVT)
{
  ui->setupUi(this);
  setWindowTitle("Добавление СВТ");
  QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp ipRegex ("^" + ipRange
                   + "\\." + ipRange
                   + "\\." + ipRange
                   + "\\." + ipRange + "$");
  QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
  ui->leIp->setValidator(ipValidator);\
  initDivisionList();
  initPerson();
  if (m_svtId) {
    setWindowTitle("Редактирование сведений об СВТ");
    QSqlQuery query("select * from svt where id="+QString::number(svtId));
    if (query.next()) {
      ui->leTitle->setText(query.value("title").toString());
      ui->leIp->setText(query.value("address").toString());
      ui->leUser->setText(query.value("user").toString());
      ui->lePassword->setText(query.value("password").toString());
      ui->leComment->setText(query.value("comment").toString());
      ui->cbPerson->setCurrentIndex(ui->cbPerson->findData(query.value("person_id")));
    }
  }
}

DialogSVT::~DialogSVT()
{
  delete ui;
}

int DialogSVT::svtId()
{
  return m_svtId;
}

QString DialogSVT::title()
{
  return ui->leTitle->text();
}

QString DialogSVT::address()
{
  return ui->leIp->text();
}

QString DialogSVT::user()
{
  return ui->leUser->text();
}

QString DialogSVT::password()
{
  return ui->lePassword->text();
}

int DialogSVT::personId()
{
  return ui->cbPerson->currentData().toInt();
}

QString DialogSVT::comment()
{
  return ui->leComment->text();
}

QSet<int> DialogSVT::divisonIds()
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

void DialogSVT::on_buttonBox_accepted()
{
  if (ui->leTitle->text().isEmpty()) {
    QMessageBox::critical(0,"Ошибка","Не указано наименование СВТ.");
    ui->leTitle->setFocus();
    return;
  }
  accept();
}

void DialogSVT::on_pushButton_clicked()
{
  if (DB::addPerson()) {
    initPerson();
    ui->cbPerson->setCurrentIndex(ui->cbPerson->count()-1);
  }
}

void DialogSVT::initPerson()
{
  ui->cbPerson->clear();
  ui->cbPerson->addItem("<нет ответственного>",QVariant(QVariant::Int));
  QSqlQuery query("select * from person");
  while (query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20)
      comment = comment.left(17)+"...";
    QString fio("%1 %2. %3. (%4)");
    fio = fio.
        arg(query.value("fam").toString()).
        arg(query.value("name").toString()[0]).
        arg(query.value("otch").toString()[0]).
        arg(comment);
    ui->cbPerson->addItem(fio,query.value("id"));
  }
}

void DialogSVT::initDivisionList()
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
  if (m_svtId) {
    query.exec("select * from svt_to_division where svt_id="+QString::number(m_svtId));
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

void DialogSVT::on_pbAddDivision_clicked()
{
  if (DB::addDivision()) {
    initDivisionList();
  }
}

void DialogSVT::on_pbSshAddKey_clicked()
{
  QProcess proc;
  QFileInfo fInfo(QDir::homePath()+"/.ssh/id_rsa.pub");
  if (!fInfo.exists())  {
    proc.setProgram("ssh-keygen");
    proc.setArguments(QStringList());
    proc.start();
    proc.write("\n");
    proc.write("y\n");
    proc.write("\n");
    proc.write("\n");
    proc.waitForFinished(2000);
    if (proc.exitCode()) {
      QMessageBox::critical(this,"Ошибка создания ключа",QString(proc.readAllStandardError()));
      return;
    }
    proc.close();
  }
  proc.setProgram("ssh-copy-id");
  proc.setArguments(QStringList()<<"-i"<<fInfo.canonicalFilePath()<<QString("%1@%2").arg(user()).arg(address()));
  proc.start();
  proc.waitForFinished(2000);

  if (proc.exitCode() || proc.state()==QProcess::Running) {
    QMessageBox::critical(this,"Ошибка записи ключа на контролируемое СВТ",QString(proc.readAllStandardError()));
    return;
  }
  proc.close();
  proc.setProgram("ssh-add");
  proc.setArguments(QStringList());
  proc.start();
  proc.waitForFinished(2000);
  if (proc.exitCode() || proc.state()==QProcess::Running) {
    QMessageBox::critical(this,"Ошибка добавления ключа",QString(proc.readAllStandardError()));
    return;
  }
  proc.close();
  QMessageBox::information(this,"Ключ добавлен","Ключ для безопасного подключения к СВТ создан и добавлен.\n"
                                                "Теперь вы можете применять правила к СВТ без ввода пароля.");

}
