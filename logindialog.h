#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QDir>
#include <QDateTime>
#include <QDebug>

namespace Ui {
  class LoginDialog;
}

class LoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoginDialog(QWidget *parent = 0);
  ~LoginDialog();
  QString dbName;
  QString dbHost;
  QString dbUser;
  QString dbPass;
  bool saveData();
  ushort dbPort;
  QByteArray crypt(const QString &data);
  void cryptInit();
  QString decrypt(const QByteArray &data);
private slots:
  void on_buttonBox_accepted();

  void on_actErase_triggered();


  void on_chbShowPass_stateChanged(int arg1);

private:
  Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
