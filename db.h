#ifndef DB_H
#define DB_H

#include <QObject>
#include "dialogmni.h"
#include "dialogdivision.h"
#include "dialogsvt.h"
#include "dialogperson.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

class DB : public QObject
{
  Q_OBJECT
public:
  explicit DB(QObject *parent = 0);

  static bool addDivision();
  static bool editDivision(int divisionId);
  static bool delDivision(int divisionId);

  static bool addSVT();
  static bool delSVT(int SVTId);
  static bool editSVT(int SVTId);


  static bool addPerson();
  static bool delPerson(int personId);
  static bool editPerson(int personId);


  static bool updateSvtToDivision(int svtId,QSet<int> divisionIds);
  static bool updatePersonToDivision(int personId,QSet<int> divisionIds);
  static bool updateDeviceToSvt(int personId,QSet<int> divisionIds);



  static bool addMNI();
  static bool delMNI(int MNIId);
  static bool editMNI(int MNIId);

  static QMap<int,int> freshRules();

signals:
  void mniChanged();

public slots:
};

#endif // DB_H
