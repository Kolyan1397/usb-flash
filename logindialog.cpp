#include "logindialog.h"
#include "ui_logindialog.h"
#include <QSettings>

LoginDialog::LoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
  ui->leLogin->addAction(ui->actErase,QLineEdit::TrailingPosition);
  ui->lePassword->addAction(ui->actErase,QLineEdit::TrailingPosition);
  QSettings settings("AFSO","flash-admin");
  ui->leHost->setText(settings.value("database/host","localhost").toString());
  ui->leDatabase->setText(settings.value("database/name","flash-admin").toString());
  ui->leLogin->setText(settings.value("database/user","admin").toString());
  QByteArray encPass = settings.value("database/pass","").toByteArray();
  QString decPass = decrypt(encPass);
  ui->lePassword->setText(decPass);
  ui->sbPort->setValue(settings.value("database/port",5432).toInt());
}

LoginDialog::~LoginDialog()
{
  delete ui;
}

bool LoginDialog::saveData()
{
  return ui->chbSaveData->isChecked();
}

QByteArray LoginDialog::crypt(const QString &data)
{
  cryptInit();
  QByteArray res;
  QByteArray pass;
  pass.append(data);
  for (int var = 0; var < pass.count(); ++var) {
    uchar key = (rand()%0x100);
    res.append(key^pass.at(var));
  }
  return res;
}

void LoginDialog::cryptInit()
{
  QDir dir(QApplication::applicationDirPath());
  QFileInfo fInfo(dir,".");
  uint32_t seed = fInfo.created().toTime_t();
  srand(seed);
}

QString LoginDialog::decrypt(const QByteArray &data)
{
  cryptInit();
  QByteArray res;
  QByteArray pass;
  pass.append(data);
  for (int var = 0; var < pass.count(); ++var) {
    uchar key = (rand()%0x100);
    res.append(key^pass.at(var));
  }
  return QString(res);
}

void LoginDialog::on_buttonBox_accepted()
{
  dbHost = ui->leHost->text();
  dbPort = ui->sbPort->value();
  dbName = ui->leDatabase->text();
  dbUser = ui->leLogin->text();
  dbPass = ui->lePassword->text();
}

void LoginDialog::on_actErase_triggered()
{
  QSettings settings("AFSO","flash-admin");
  settings.setValue("database/user",QVariant(""));
  settings.setValue("database/pass",QVariant(""));
  ui->leLogin->clear();
  ui->lePassword->clear();
}


void LoginDialog::on_chbShowPass_stateChanged(int arg1)
{
  if (arg1!=Qt::Checked) {
    ui->lePassword->setEchoMode(QLineEdit::Password);
  }
  else {
    ui->lePassword->setEchoMode(QLineEdit::Normal);
  }
}
