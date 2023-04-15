#include "deployer.h"

Deployer::Deployer(QObject *parent) : QObject(parent)
{

}

QString Deployer::writeRuleFile(int SVTId)
{
   QSqlQuery svtQuery("select * from svt where id="+QString::number(SVTId));
   if (!svtQuery.next()) {
     qDebug() << "svt with id  "<<SVTId << " not found" ;
     return QString();
   }
   QFile rulesFile(QString("99-flash-admin-%1.rules").arg(svtQuery.value("address").toString()));
   QFile templateFile("files/99-flash-admin.template");
   templateFile.open(QIODevice::ReadOnly| QIODevice::Text);
   rulesFile.open(QIODevice::WriteOnly| QIODevice::Text);
   QTextStream fout(&rulesFile);
   while (!templateFile.atEnd()) {
     QByteArray line = templateFile.readLine();
     if (!line.startsWith(QByteArray("###---###"))) { // ordinal line
       fout << line;
     } else { // white-list template line
       fout << line ;
       QString tLine = QString(templateFile.readLine());
       QSqlQuery query("select * from device_to_svt "
                       "left join device on (device.id = device_to_svt.device_id) "
                       "where svt_id="+QString::number(SVTId));
       while (query.next()) {

         QString whiteLine = tLine.arg(query.value("serial").toString()).
             arg(query.value("VID").toString()).
             arg(query.value("PID").toString());
         fout << whiteLine;
       }
     }
   }
   rulesFile.close();
   templateFile.close();
   return rulesFile.fileName();
}

bool Deployer::doDeploy(const QString &rulesFilename, int SVTId)
{
  QSqlQuery svtQuery("select * from svt where id="+QString::number(SVTId));
  if (!svtQuery.next()) {
    qDebug() << "svt with id  "<<SVTId << " not found" ;
    return false;
  }
  QString address = svtQuery.value("address").toString();
  QString user = svtQuery.value("user").toString();
  QString password = svtQuery.value("password").toString();
  bool res = true;
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  res &= deployFile(rulesFilename,"/etc/udev/rules.d/99-flash-admin.rules",address, user);
  res &= deployFile("files/deny-flash.py","/usr/local/sbin/",address, user);
  res &= deployFile("files/notify.sh","/usr/local/sbin/",address, user);
  res &= execCommand("udevadm control --reload-rules",address,user);
  QApplication::restoreOverrideCursor();
  if (!res)
    return res;
  int version = DB::freshRules()[SVTId];
  svtQuery.exec("update svt set deploy_time=now(), rule_version="+
                QString::number(version)+
                "  where id="+QString::number(SVTId));
  emit MainWindow::w()->svtChanged();
  return true;

}

bool Deployer::deployFile(const QString& src, const QString& dst, const QString& address, const QString& user, int timeout)
{
  QProcess proc;
  proc.setProgram("scp");
  proc.setArguments(QStringList()<<QString("-o ConnectTimeout=%1").arg(timeout/1000)
                    <<src<<
                    QString("%1@%2:%3").arg(user).arg(address).arg(dst));
  proc.start();
  char buf[1024] = {0};
  while (!proc.waitForFinished(100)) {
    QApplication::processEvents();
    qint64 n = proc.read(buf,sizeof(buf));
    QString s(buf);
    if (s.contains("(yes/no)?")) {
      proc.write("yes\n");
    }
    if (s.contains("Permission denied")) {
      return false;
    }
  }
  if (proc.exitCode()) {
    return false;
  }
  return true;
}

bool Deployer::execCommand(const QString &command,  const QString& address, const QString& user,int timeout)
{
  QProcess proc;
  proc.setProgram("ssh");
  proc.setArguments(QStringList()<<QString("-o ConnectTimeout=%1").arg(timeout/1000)
                    << QString("%1@%2").arg(user).arg(address)
                    << command);
  proc.start();
  char buf[1024] = {0};
  while (!proc.waitForFinished(100)) {
    QApplication::processEvents();
    qint64 n = proc.read(buf,sizeof(buf));
    QString s(buf);
    if (s.contains("(yes/no)?")) {
      proc.write("yes\n");
    }
    if (s.contains("Permission denied")) {
      return false;
    }
  }
  if (proc.exitCode()) {
    return false;
  }
  return true;
}
