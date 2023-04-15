#include "db.h"

DB::DB(QObject *parent) : QObject(parent)
{

}

bool DB::addDivision()
{
  bool res = false;
  DialogDivision dialogDivision;
  if (dialogDivision.exec()==QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("insert into division (title, \"comment\") values (:title, :comment) ;");
    query.bindValue(":title",dialogDivision.title());
    query.bindValue(":comment",dialogDivision.comment());
    res = query.exec();
  }
  return res;
}

bool DB::editDivision(int divisionId)
{
  DialogDivision dialogDivision(0,divisionId);
  if (dialogDivision.exec()==QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("UPDATE public.division "
                  "SET title=:title, \"comment\"=:comment "
                  "WHERE id=:divisionId;" );
    query.bindValue(":title",dialogDivision.title());
    query.bindValue(":comment",dialogDivision.comment());
    query.bindValue(":divisionId",divisionId);
    return query.exec();
  }
  return false;
}

bool DB::delDivision(int divisionId)
{
  QSqlQuery query("select * from division where id="+QString::number(divisionId));
  if (query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20)
      comment = comment.left(17)+"...";
    QString title = query.value("title").toString();
    if (QMessageBox::question(0,"Удаление подразделения",
                              QString("Вы действительно хотите удалить \n%1 (%2)?").arg(title).arg(comment))==QMessageBox::Yes)  {
       return query.exec("delete from division where id = "+QString::number(divisionId));
    }
  } else {
    QMessageBox::warning(0,"Ошибка удаления","Не удалось удалить подразделение с кодом "+QString::number(divisionId));
  }
  return false;
}

bool DB::addSVT()
{
  DialogSVT dialogSvt;
  if (dialogSvt.exec()==QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("INSERT INTO svt "
                  " (title, address, \"user\", password, \"comment\", person_id) "
                  " VALUES( :title, :address, :user, :password, :comment, :persson_id ) returning id;");
    query.bindValue(":title",dialogSvt.title());
    query.bindValue(":address",dialogSvt.address());
    query.bindValue(":user",dialogSvt.user());
    query.bindValue(":password",dialogSvt.password());
    query.bindValue(":person_id",dialogSvt.personId());
    query.bindValue(":comment",dialogSvt.comment());
    query.exec();
    if (!query.next()) {
      return false;
    }
    int svtId = query.value(0).toInt();
    QSet<int> divisionIds = dialogSvt.divisonIds();
    DB::updateSvtToDivision(svtId,divisionIds);
    emit MainWindow::w()->svtChanged();
    return true;
  }
  return false;
}

bool DB::delSVT(int SVTId)
{
  QSqlQuery query("select * from SVT where id="+QString::number(SVTId));
  if (query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20)
      comment = comment.left(17)+"...";
    QString title = query.value("title").toString();
    if (QMessageBox::question(0,"Удаление СВТ",
                              QString("Вы действительно хотите удалить \n%1 (%2)?").arg(title).arg(comment))==QMessageBox::Yes)  {

       bool res = query.exec("delete from SVT where id = "+QString::number(SVTId));
       emit MainWindow::w()->svtChanged();
       return res;
    }
  } else {
    QMessageBox::warning(0,"Ошибка удаления","Не удалось удалить СВТ с кодом "+QString::number(SVTId));
  }
  return false;

}

bool DB::editSVT(int SVTId)
{
  DialogSVT dialogSVT(0,SVTId);
  if (dialogSVT.exec()==QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("UPDATE SVT "
                  "SET title=:title,  address=:address, \"user\"=:user, "
                  "person_id=:person_id, password=:password, \"comment\"=:comment "
                  "WHERE id=:SVTId;" );
    query.bindValue(":title",dialogSVT.title());
    query.bindValue(":address",dialogSVT.address());
    query.bindValue(":user",dialogSVT.user());
    query.bindValue(":password",dialogSVT.password());
    query.bindValue(":person_id",dialogSVT.personId());
    query.bindValue(":comment",dialogSVT.comment());
    query.bindValue(":SVTId",SVTId);
    query.exec();
    QSet<int> divisionIds = dialogSVT.divisonIds();
    DB::updateSvtToDivision(SVTId,divisionIds);
    emit MainWindow::w()->svtChanged();
    return true;
  }
  return false;
}

bool DB::addPerson()
{
  DialogPerson dialogPerson;
  if (dialogPerson.exec()==QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("INSERT INTO Person "
                  " (fam, \"name\", otch, \"comment\") "
                  " VALUES( :fam, :name, :otch, :comment ) returning id;");
    query.bindValue(":fam",dialogPerson.fam());
    query.bindValue(":name",dialogPerson.name());
    query.bindValue(":otch",dialogPerson.otch());
    query.bindValue(":comment",dialogPerson.comment());
    query.exec();
    if (!query.next()) {
      return false;
    }
    int personId = query.value(0).toInt();
    QSet<int> divisionIds = dialogPerson.divisonIds();
    DB::updatePersonToDivision(personId,divisionIds);
    emit MainWindow::w()->personChanged();
    return true;
  }
  return false;

}

bool DB::delPerson(int personId)
{
  QSqlQuery query("select * from person where id="+QString::number(personId));
  if (query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20)
      comment = comment.left(17)+"...";
    QString text = QString("%1 %2. %3. (%4)").arg(query.value("fam").toString())
                   .arg(query.value("name").toString()[0])
                   .arg(query.value("otch").toString()[0])
                   .arg(comment);
    if (QMessageBox::question(0,"Удаление отвественного",
                              QString("Вы действительно хотите удалить запись \n\"%1\"?").arg(text))==QMessageBox::Yes)  {
      bool res = query.exec("delete from person where id = "+QString::number(personId));
      emit MainWindow::w()->personChanged();
      return res;
    }
  } else {
    QMessageBox::warning(0,"Ошибка удаления","Не удалось удалить ответственного с кодом "+QString::number(personId));
  }
  return false;
}

bool DB::editPerson(int personId)
{
    DialogPerson dialogPerson(0,personId);
    if (dialogPerson.exec()==QDialog::Accepted) {
      QSqlQuery query;
      query.prepare("UPDATE person "
                    "SET fam=:fam,  \"name\"=:name, otch=:otch, \"comment\"=:comment "
                    "WHERE id=:personId;" );
      query.bindValue(":fam",dialogPerson.fam());
      query.bindValue(":name",dialogPerson.name());
      query.bindValue(":otch",dialogPerson.otch());
      query.bindValue(":comment",dialogPerson.comment());
      query.bindValue(":personId",personId);
      query.exec();
      QSet<int> divisionIds = dialogPerson.divisonIds();
      DB::updatePersonToDivision(personId,divisionIds);
      emit MainWindow::w()->personChanged();
      return true;
    }
    return false;
}

bool DB::updateSvtToDivision(int svtId, QSet<int> divisionIds)
{
  QSqlQuery query;
  QSqlDatabase db = QSqlDatabase::database();
  db.transaction();
  query.exec("delete from svt_to_division where svt_id="+QString::number(svtId));
  query.prepare("INSERT INTO public.svt_to_division"
                " (svt_id, division_id)"
                "VALUES(:svtId, :divisionId);");
  for (int divisionId:divisionIds) {
    query.bindValue(":svtId",svtId);
    query.bindValue(":divisionId",divisionId);
    query.exec();
  }
  if (!db.commit()) {
    db.rollback();
    return false;
  }
  return true;
}

bool DB::updatePersonToDivision(int personId, QSet<int> divisionIds)
{
  QSqlQuery query;
  QSqlDatabase db = QSqlDatabase::database();
  db.transaction();
  query.exec("delete from person_to_division where person_id="+QString::number(personId));
  query.prepare("INSERT INTO person_to_division"
                " (person_id, division_id)"
                "VALUES(:personId, :divisionId);");
  for (int divisionId:divisionIds) {
    query.bindValue(":personId",personId);
    query.bindValue(":divisionId",divisionId);
    query.exec();
  }
  if (!db.commit()) {
    db.rollback();
    return false;
  }
  return true;
}

bool DB::updateDeviceToSvt(int deviceId, QSet<int> svtIds)
{
  QSqlQuery query;
  QSqlDatabase db = QSqlDatabase::database();
  db.transaction();
  QSet<int> changedSvtIds = svtIds;
  query.exec("select * from device_to_svt where device_id="+QString::number(deviceId));
  while(query.next()) {
    changedSvtIds.insert(query.value("svt_id").toInt());
  }
  query.exec("delete from device_to_svt where device_id="+QString::number(deviceId));
  query.prepare("INSERT INTO device_to_svt "
                " (device_id, svt_id)"
                "VALUES(:deviceId, :svtId);");
  for (int svtId:svtIds) {
    query.bindValue(":deviceId",deviceId);
    query.bindValue(":svtId",svtId);
    query.exec();
  }
  QSqlQuery ruleQuery;
  ruleQuery.prepare("insert into rules (date,svt_id) values (now(),:svtId)");
  for (int svtId:changedSvtIds) {
    ruleQuery.bindValue(":svtId",svtId);
    ruleQuery.exec();
  }
  if (!db.commit()) {
    db.rollback();
    return false;
  }
  emit MainWindow::w()->rulesChanged();
  return true;
}

bool DB::addMNI()
{
  DialogMni dialogMni;
  if (dialogMni.exec() == QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("INSERT INTO public.device "
                  " (\"VID\", \"PID\", \"serial\", manufacturer, "
                  " product, title, \"comment\", person_id, reg_num) "
                  " VALUES(:vid, :pid, :serial, :manufacturer, "
                  " :product, :title, :comment, :person_id, :reg_num ) "
                  " returning id;");
    query.bindValue(":vid",dialogMni.vid());
    query.bindValue(":pid",dialogMni.pid());
    query.bindValue(":serial",dialogMni.serial());
    query.bindValue(":manufacturer",dialogMni.manufacturer());
    query.bindValue(":product",dialogMni.product());
    query.bindValue(":title",dialogMni.title());
    query.bindValue(":comment",dialogMni.comment());
    query.bindValue(":person_id",dialogMni.regWho());
    query.bindValue(":reg_num",dialogMni.regNum());
    query.exec();
    if (!query.next()) {
      return false;
    }
    int mniId = query.value(0).toInt();
    QSet<int> svtIds = dialogMni.svtIds();
    DB::updateDeviceToSvt(mniId,svtIds);
    emit MainWindow::w()->mniChanged();

    return true;
  }
  return false;
}

bool DB::delMNI(int MNIId)
{
  QSqlQuery query("select * from device where id="+QString::number(MNIId));
  if (query.next()) {
    QString comment = query.value("comment").toString();
    if (comment.length()>20) {
      comment = comment.left(17)+"...";
    }
    QString title = query.value("title").toString()+" "+query.value("manufacturer").toString()+" "+query.value("product").toString() ;
    if (QMessageBox::question(0,"Удаление MNI",
                              QString("Вы действительно хотите удалить \n"
                                      "%1\n"
                                      "уч. номер %2 серийный номер %3\n"
                                      " (%4)?")
                              .arg(title)
                              .arg(query.value("reg_num").toString())
                              .arg(query.value("serial").toString())
                              .arg(comment)
                              ) == QMessageBox::Yes)  {
       bool res = query.exec("delete from device where id = "+QString::number(MNIId));
       emit MainWindow::w()->mniChanged();
       emit MainWindow::w()->rulesChanged();
       return res;
    }
  } else {
    QMessageBox::warning(0,"Ошибка удаления","Не удалось удалить МНИ с кодом "+QString::number(MNIId));
  }
  return false;
}

bool DB::editMNI(int MNIId)
{
  DialogMni dialogMni(0,MNIId);
  if (dialogMni.exec() == QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("UPDATE device "
                  "SET \"VID\"=:vid, \"PID\"=:pid, serial=:serial, "
                  "manufacturer=:manufacturer, product=:product, "
                  "title=:title, \"comment\"=:comment, person_id=:person_id, "
                  "reg_num=:reg_num "
                  "WHERE id=:MNIId");
    query.bindValue(":vid",dialogMni.vid());
    query.bindValue(":pid",dialogMni.pid());
    query.bindValue(":serial",dialogMni.serial());
    query.bindValue(":manufacturer",dialogMni.manufacturer());
    query.bindValue(":product",dialogMni.product());
    query.bindValue(":title",dialogMni.title());
    query.bindValue(":comment",dialogMni.comment());
    query.bindValue(":person_id",dialogMni.regWho());
    query.bindValue(":reg_num",dialogMni.regNum());
    query.bindValue(":MNIId",MNIId);
    query.exec();
    QSet<int> svtIds = dialogMni.svtIds();
    DB::updateDeviceToSvt(MNIId,svtIds);
    emit MainWindow::w()->mniChanged();
    return true;
  }
  return false;

}

QMap<int, int> DB::freshRules()
{
  QMap<int, int> res;
  QSqlQuery query("select max(version),svt_id from rules group by svt_id;");
  while(query.next()) {
    res[query.value("svt_id").toInt()]=query.value(0).toInt();
  }
  return res;
}
