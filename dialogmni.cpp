#include "dialogmni.h"
#include "ui_dialogmni.h"

DialogMni::DialogMni(QWidget *parent, int deviceId) :
  QDialog(parent),
  m_deviceId(deviceId),
  ui(new Ui::DialogMni)
{
  ui->setupUi(this);
  ui->buttonBox->addAction(ui->actAutoLoadMNI);
  ui->lePID->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{4}")));
  ui->leVID->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{4}")));
  ui->leSerial->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]+")));
  ui->leRegNumber->setValidator(new QRegExpValidator(QRegExp(".+")));
  ui->cbRegWho->setValidator(new QRegExpValidator(QRegExp(".+")));
  validPalette.setBrush(QPalette::Base,QBrush(QColor(0xA0,0xFF,0xA0)));
  invalidPalette.setBrush(QPalette::Base,QBrush(QColor(0xFF,0xA0,0xA0)));

  ui->leSerial->addAction(ui->actAutoLoadMNI,QLineEdit::TrailingPosition);
  ui->lePID->addAction(ui->actAutoLoadMNI,QLineEdit::TrailingPosition);
  ui->leVID->addAction(ui->actAutoLoadMNI,QLineEdit::TrailingPosition);
  ui->leManufacturer->addAction(ui->actAutoLoadMNI,QLineEdit::TrailingPosition);
  ui->leProduct->addAction(ui->actAutoLoadMNI,QLineEdit::TrailingPosition);


  for (auto le:this->findChildren<QLineEdit *>()) {
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(colorizeValid(QString)));
  }
  initPersonCB();
  initSvtList();
  if (deviceId) {
    setWindowTitle("Редактирование МНИ");
    QSqlQuery query("select * from device "
                    "left join person on (device.person_id = person.id)   "
                    "where device.id="+QString::number(deviceId));
    if (query.next()) {
      m_deviceId = deviceId;
      ui->leManufacturer->setText(query.value("manufacturer").toString());
      ui->lePID->setText(query.value("PID").toString());
      ui->leProduct->setText(query.value("product").toString());
      ui->leSerial->setText(query.value("serial").toString());
      ui->leTitle->setText(query.value("title").toString());
      ui->leVID->setText(query.value("VID").toString());
      ui->cbRegWho->setCurrentIndex(ui->cbRegWho->findData(query.value("person_id")));
      ui->leRegNumber->setText(query.value("reg_num").toString());
    }
  }
}

DialogMni::~DialogMni()
{
  delete ui;
}

int DialogMni::regWho(){
  return ui->cbRegWho->currentData().toInt();
}

QString DialogMni::regNum()
{
  return ui->leRegNumber->text();
}

QString DialogMni::title()
{
  return ui->leTitle->text();
}

QString DialogMni::serial()
{
  return ui->leSerial->text();
}

QString DialogMni::manufacturer()
{
  return ui->leManufacturer->text();
}

QString DialogMni::product()
{
  return ui->leProduct->text();
}

QString DialogMni::vid()
{
  return ui->leVID->text();
}

QString DialogMni::pid()
{
  return ui->lePID->text();
}

QString DialogMni::comment()
{
  return ui->leComment->text();
}

QSet<int> DialogMni::svtIds()
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

void DialogMni::on_actAutoLoadMNI_triggered()
{
  if (QMessageBox::question(this,"Определение носителя",
                            "Вставьте добавляемый носитель в разъем USB и после этого нажмите кнопку OK\n"
                            "Для отказа от автоматического определения нажмите кнопку Cancel",QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok ) {
    QRegularExpression re("Bus \\d{3} Device \\d{3}: ID [a-f\\d]{4}:[a-f\\d]{4} (.*)[\\s\\S]*?"
                          "idVendor[\\s]+0x([0-9a-f]{4})[\\s\\S]*?"
                          "idProduct[\\s]+0x([0-9a-f]{4})[\\s\\S]*?"
                          "iManufacturer[\\s]+\\d (.*)[\\s\\S]*?"
                          "iProduct[\\s]+\\d (.*)[\\s\\S]*?"
                          "iSerial[\\s]+\\d (.*)[\\s\\S]*?"
                          "bInterfaceClass +(\\d)",QRegularExpression::MultilineOption);

    QProcess proc;
    proc.start("lsusb",QStringList()<<"-v");
    proc.waitForFinished();
    QString out = proc.readAllStandardOutput();
    int offset = 0;
    QRegularExpressionMatch match = re.match(out,offset);
    while(match.hasMatch()) {
      match = re.match(out,offset);
      if (match.hasMatch())  {
        //        qDebug() << match.captured(1);
        if (match.captured(7) == "8") { // Mass Storage
          Flash flash;
          flash.title = match.captured(1);
          flash.VID = match.captured(2);
          flash.PID = match.captured(3);
          flash.manufacturer = match.captured(4);
          flash.product = match.captured(5);
          flash.serial = match.captured(6);
          flashes.append(flash);
        }
      }
      offset = match.capturedEnd();
    }
    if (flashes.empty()) {
      QMessageBox::critical(this,"Ошибка определения","Носитель не найден");
      return;
    }
    if (flashes.count()==1) {
      fillFields(flashes[0]);
      return;
    }
    QStringList items;
    for(auto flash: flashes) {
      items.append(flash.title);
    }
    bool ok;
    QString item = QInputDialog::getItem(this,"Выберите носитель","Носитель:",items,0,false,&ok);
    if (!ok) return;
    fillFields(flashes[items.indexOf(item)]);
  }

}

void DialogMni::colorizeValid(const QString &text)
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

void DialogMni::fillFields(const Flash &flash)
{
  ui->leManufacturer->setText(flash.manufacturer);
  ui->leProduct->setText(flash.product);
  ui->leTitle->setText(flash.title);
  ui->leSerial->setText(flash.serial);
  ui->leVID->setText(flash.VID);
  ui->lePID->setText(flash.PID);
  return;
}

void DialogMni::recheckSvtList()
{
  resultSvt = personSvt+divisionSvt+allSvt+manualCheckedSvt;
  disconnect(ui->listWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(on_listWidget_itemChanged(QListWidgetItem*)));
  for (int i = 0; i < ui->listWidget->count(); ++i) {
    QListWidgetItem * item = ui->listWidget->item(i);
    if (resultSvt.contains(item->data(Qt::UserRole).toInt())) {
      item->setCheckState(Qt::Checked);
    } else {
      item->setCheckState(Qt::Unchecked);
    }
  }
  connect(ui->listWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(on_listWidget_itemChanged(QListWidgetItem*)));
}


void DialogMni::on_buttonBox_accepted()
{
  if (ui->cbRegWho->currentText().isEmpty()) {
    QMessageBox::critical(0,"Ошибка добавления","Не указано лицо, на которое зарегистрирован носитель.");
    ui->cbRegWho->setFocus();
    return;
  }
  if (ui->leRegNumber->text().isEmpty()) {
    QMessageBox::critical(0,"Ошибка добавления","Не указан регистрационный номер носителя.");
    ui->leRegNumber->setFocus();
    return;
  }
  if (ui->leSerial->text().isEmpty()) {
    QMessageBox::critical(0,"Ошибка добавления","Не указан серийный номер носителя.");
    ui->leSerial->setFocus();
    return;
  }
  int pos;
  QString PID = ui->lePID->text();
  if (ui->lePID->validator()->validate(PID,pos)!=QValidator::Acceptable) {
    QMessageBox::critical(0,"Ошибка добавления","Не правильно указан идентификатор продукта (PID) носителя.");
    ui->lePID->setFocus();
    return;
  }
  QString VID = ui->leVID->text();
  if (ui->leVID->validator()->validate(VID,pos)!=QValidator::Acceptable) {
    QMessageBox::critical(0,"Ошибка добавления","Не правильно указан идентификатор поставщика (VID) носителя.");
    ui->leVID->setFocus();
    return;
  }
  accept();
}

void DialogMni::chbChanged()
{
  QSqlQuery query;

  if (ui->chbAllSvt->checkState() == Qt::Checked) {
    query.exec("select id from svt");
    while (query.next()) {
      allSvt.insert(query.value("id").toInt());
    }
  } else {
    allSvt.clear();
  }

  int personId = ui->cbRegWho->currentData().toInt();
  if (ui->chbPersonSVT->checkState() == Qt::Checked) {
    query.exec("select id from svt where person_id="+QString::number(personId));
    while (query.next()) {
      personSvt.insert(query.value("id").toInt());
    }
  } else {
    personSvt.clear();
  }

  if (ui->chbDivisionSvt->checkState() == Qt::Checked) {
    query.exec("select svt_id from person_to_division "
               "join svt_to_division on (person_to_division.division_id = svt_to_division.division_id) "
               "where person_id="+QString::number(personId));
    while (query.next()) {
      divisionSvt.insert(query.value(0).toInt());
    }
  } else {
    divisionSvt.clear();
  }
  recheckSvtList();
}

void DialogMni::initSvtList()
{
  ui->listWidget->clear();
  QSqlQuery query("select * from svt;");
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
  if (m_deviceId) {
    QSqlQuery query("select * from device_to_svt where device_id="+QString::number(m_deviceId));
    QSet<int> svtIds;
    while (query.next()) {
      svtIds.insert(query.value("svt_id").toInt());
    }
    for (int i = 0; i < ui->listWidget->count(); ++i) {
      QListWidgetItem * item = ui->listWidget->item(i);
      if (svtIds.contains(item->data(Qt::UserRole).toInt())) {
        item->setCheckState(Qt::Checked);
      } else {
        item->setCheckState(Qt::Unchecked);
      }
    }
  }
}

void DialogMni::on_listWidget_itemChanged(QListWidgetItem *item)
{
  if (item->checkState()==Qt::Checked)  {
    manualCheckedSvt.insert(item->data(Qt::UserRole).toInt());
  } else {
    manualCheckedSvt.remove(item->data(Qt::UserRole).toInt());
  }
}


void DialogMni::on_pbAddUser_clicked()
{
  if(DB::addPerson()) {
    initPersonCB();
    ui->cbRegWho->setCurrentIndex(ui->cbRegWho->count()-1);
  }
}

void DialogMni::initPersonCB()
{
  ui->cbRegWho->clear();
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
    ui->cbRegWho->addItem(fio,query.value("id"));
  }
  ui->cbRegWho->setCurrentIndex(-1);
}

void DialogMni::on_pbAddSVT_clicked()
{
  if (DB::addSVT()) {
    initSvtList();
  }
}
