#ifndef ACT_H
#define ACT_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"

class Act : public QObject
{
  Q_OBJECT
  MainWindow* m_w;
public:
  explicit Act(QObject *parent = 0);

signals:

public slots:
};

#endif // ACT_H
