#ifndef DEPLOYER_H
#define DEPLOYER_H

#include <QObject>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QProcess>
#include "mainwindow.h"
#include <QApplication>

class Deployer : public QObject
{
  Q_OBJECT
public:
  explicit Deployer(QObject *parent = 0);
  static QString writeRuleFile(int SVTId);
  static bool doDeploy(const QString& rulesFilename,int SVTId);
  static bool deployFile(const QString& src, const QString& dst, const QString& address, const QString& user, int timeout = 2000);
  static bool execCommand(const QString& command,  const QString& address, const QString& user, int timeout = 2000);


signals:

public slots:
};

#endif // DEPLOYER_H
