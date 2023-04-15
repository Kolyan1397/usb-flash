#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSettings>
#include <QDebug>
#include <QtSql/QSqlError>
#include "logindialog.h"
#include <stdio.h>
#include <stdlib.h>


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QString txt;
  switch (type) {
    case QtDebugMsg:
      txt =QString("Debug: %1 (%2:%3, %3)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
      break;
    case QtWarningMsg:
      txt =QString("Warning: %1 (%2:%3, %3)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
      break;
    case QtCriticalMsg:
      txt =QString("Critical: %1 (%2:%3, %3)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
      break;
    case QtFatalMsg:
      txt =QString("FATAL: %1 (%2:%3, %3)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
      abort();
      break;
    default:
      txt =QString("Something wrong: %1 (%2:%3, %3)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
      break;
  }
  QFile outFile("flash-admin.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream ts(&outFile);
  ts << txt << endl;
  if (msg.contains("ОШИБКА:  нет доступа")) {
    QMessageBox::warning(0,"Ошибка доступа", "Данная операция не разрешена текущему пользователю");
  }
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(myMessageOutput);
  QApplication a(argc, argv);
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
  LoginDialog *loginDialog = new LoginDialog;
  if (loginDialog->exec()) {
    db.setHostName(loginDialog->dbHost);
    db.setPort(loginDialog->dbPort);
    db.setDatabaseName(loginDialog->dbName);
    db.setUserName(loginDialog->dbUser);
    db.setPassword(loginDialog->dbPass);
    if (!db.open()) {
      QMessageBox::critical(0,"Ошибка подключения к БД",db.lastError().text());
      qDebug()<<db.lastError().text();
      return 1;
    }
    if(!db.driver()->subscribeToNotification("employee_changed"))
          qWarning()<<db.driver()->lastError().databaseText();
    QSettings settings("AFSO","flash-admin");
    settings.setValue("database/host",QVariant(db.hostName()));
    settings.setValue("database/port",QVariant(db.port()));
    settings.setValue("database/name",QVariant(db.databaseName()));
    if (loginDialog->saveData()) {
      settings.setValue("database/user",QVariant(db.userName()));
      settings.setValue("database/pass",QVariant(loginDialog->crypt(db.password())));//TODO: сделать зашифрованный пароль
    }

    MainWindow w;
    w.show();
    return a.exec();
  } else {
    return 2;
  }
}
