#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::m_w = 0;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->mainToolBar->setVisible(false);
  m_w=this;
  initDivision();
  initSVT();
  initPerson();
  initMNI();
  initReport();
  connect(this,SIGNAL(mniChanged()),tmMni,SLOT(select()));
  connect(this,SIGNAL(svtChanged()),tmSVT,SLOT(select()));
  connect(this,SIGNAL(personChanged()),tmPerson,SLOT(select()));
  connect(this,SIGNAL(divisionChanged()),tmDivision,SLOT(select()));
  connect(this,SIGNAL(rulesChanged()),this,SLOT(updateRulesVersion()));

  //    ui->tableView->addAction(ui->actAddMni);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actAddMni_triggered()
{
  if (DB::addMNI()) tmMni->select();
}


void MainWindow::initDivision()
{
  tmDivision = new QSqlTableModel(this);
  tmDivision->setTable("division");
  tmDivision->setHeaderData(1,Qt::Horizontal,"Наименование");
  tmDivision->setHeaderData(2,Qt::Horizontal,"Комментарий");
  tmDivision->setEditStrategy(QSqlTableModel::OnRowChange);

  ui->tvDivision->setModel(tmDivision);
  ui->tvDivision->setColumnHidden(0,1);
  ui->tvDivision->addAction(ui->actAddDivision);
  ui->tvDivision->addAction(ui->actDelDivision);
  ui->tvDivision->addAction(ui->actEditDivision);

  tmDivision->select();
  ui->tvDivision->resizeColumnsToContents();
}

void MainWindow::initSVT()
{
  tmSVT = new ColorTableModel(this);
  tmSVT->actualRules = DB::freshRules();
  tmSVT->setTable("svt");
  tmSVT->setHeaderData(1,Qt::Horizontal,"Наименование");
  tmSVT->setHeaderData(2,Qt::Horizontal,"IP-адрес");
  tmSVT->setHeaderData(3,Qt::Horizontal,"Административный\nлогин");
  tmSVT->setHeaderData(4,Qt::Horizontal,"Административный\nпароль");
  tmSVT->setHeaderData(5,Qt::Horizontal,"Комментарий");
  tmSVT->setHeaderData(6,Qt::Horizontal,"Ответственный");
  tmSVT->setHeaderData(7,Qt::Horizontal,"Правила\nприменены");
  tmSVT->setHeaderData(8,Qt::Horizontal,"Версия\nправил");
  tmSVT->setEditStrategy(QSqlTableModel::OnRowChange);
  tmSVT->setJoinMode(QSqlRelationalTableModel::LeftJoin);
  tmSVT->setRelation(6,QSqlRelation("person","id","fam||' '||\"name\"||' '||otch"));

  ui->tvSVT->setModel(tmSVT);
  ui->tvSVT->setColumnHidden(0,1);
  ui->tvSVT->addAction(ui->actAddSVT);
  ui->tvSVT->addAction(ui->actDelSVT);
  ui->tvSVT->addAction(ui->actEditSVT);
  ui->tvSVT->addAction(ui->actDeployRules);
  ui->tvSVT->addAction(ui->actDeployAll);
  tmSVT->select();
  ui->tvSVT->resizeColumnsToContents();

  QToolBar* toolBar = new QToolBar(this);
  ui->tab_5->layout()->addWidget(toolBar);
  toolBar->addAction(ui->actAddSVT);
  toolBar->addAction(ui->actEditSVT);
  toolBar->addAction(ui->actDelSVT);
  toolBar->addAction(ui->actDeployRules);
  toolBar->addAction(ui->actDeployAll);

}

void MainWindow::initPerson()
{
  tmPerson = new QSqlTableModel(this);
  tmPerson->setTable("person");
  tmPerson->setHeaderData(1,Qt::Horizontal,"Фамилия");
  tmPerson->setHeaderData(2,Qt::Horizontal,"Имя");
  tmPerson->setHeaderData(3,Qt::Horizontal,"Отчество");
  tmPerson->setHeaderData(4,Qt::Horizontal,"Комментарий");
  tmPerson->setEditStrategy(QSqlTableModel::OnRowChange);

  ui->tvPerson->setModel(tmPerson);
  ui->tvPerson->setColumnHidden(0,1);
  ui->tvPerson->addAction(ui->actAddPerson);
  ui->tvPerson->addAction(ui->actDelPerson);
  ui->tvPerson->addAction(ui->actEditPerson);
  tmPerson->select();
  ui->tvPerson->resizeColumnsToContents();

}

void MainWindow::initMNI()
{
  tmMni = new QSqlRelationalTableModel(this);
  tmMni->setTable("device");
  tmMni->setHeaderData(1,Qt::Horizontal,"VID");
  tmMni->setHeaderData(2,Qt::Horizontal,"PID");
  tmMni->setHeaderData(3,Qt::Horizontal,"Серийный\nномер");
  tmMni->setHeaderData(4,Qt::Horizontal,"Производитель");
  tmMni->setHeaderData(5,Qt::Horizontal,"Продукт");
  tmMni->setHeaderData(6,Qt::Horizontal,"Наименование");
  tmMni->setHeaderData(7,Qt::Horizontal,"Примечание");
  tmMni->setHeaderData(8,Qt::Horizontal,"На кого\nзарегистрирован");
  tmMni->setHeaderData(9,Qt::Horizontal,"Рег. номер");
  tmMni->setEditStrategy(QSqlTableModel::OnRowChange);
  tmMni->setJoinMode(QSqlRelationalTableModel::LeftJoin);
  tmMni->setRelation(8,QSqlRelation("person","id","fam||' '||\"name\"||' '||otch"));

  ui->tvMni->setModel(tmMni);
  ui->tvMni->setColumnHidden(0,1);
  ui->tvMni->horizontalHeader()->moveSection(8,0);
  ui->tvMni->horizontalHeader()->moveSection(9,0);
  ui->tvMni->addAction(ui->actAddMni);
  ui->tvMni->addAction(ui->actDelMni);
  ui->tvMni->addAction(ui->actEditMni);
  tmMni->select();
  ui->tvMni->resizeColumnsToContents();


}

void MainWindow::initReport()
{
   tmReport = new QSqlTableModel(this);
   tmReport->setTable("report");
   tmReport->setHeaderData(1,Qt::Horizontal,"Время");
   tmReport->setHeaderData(2,Qt::Horizontal,"Адрес СВТ");
   tmReport->setHeaderData(3,Qt::Horizontal,"PID");
   tmReport->setHeaderData(4,Qt::Horizontal,"VID");
   tmReport->setHeaderData(5,Qt::Horizontal,"Серийны\nномер");
   tmReport->setHeaderData(6,Qt::Horizontal,"Примечания");
   tmReport->setHeaderData(7,Qt::Horizontal,"Производитель");
   tmReport->setHeaderData(8,Qt::Horizontal,"Продукт");
   QSqlDatabase::database().driver()->subscribeToNotification("report");
   connect(QSqlDatabase::database().driver(),SIGNAL(notification(QString)),this,SLOT(parseNotify(QString)));

   ui->tvReport->setModel(tmReport);
   ui->tvReport->setColumnHidden(0,1);
   ui->tvReport->horizontalHeader()->moveSection(7,3);
   ui->tvReport->horizontalHeader()->moveSection(8,4);
   tmReport->select();
   ui->tvReport->resizeColumnsToContents();
}


void MainWindow::on_actAddDivision_triggered()
{
  if (DB::addDivision()) {
    //    tmDivision->select();
  }
}

void MainWindow::on_actDelDivision_triggered()
{
  int divisionId = currentId(ui->tvDivision);
  if (divisionId) {
    if (DB::delDivision(divisionId)) {
      //      tmDivision->select();
    }
  }

}

void MainWindow::on_actEditDivision_triggered()
{
  int divisionId = currentId(ui->tvDivision);
  if (divisionId) {
    if (DB::editDivision(divisionId)){
      //      tmDivision->select();
    }
  }
}

int MainWindow::currentId(QTableView *tableView)
{
  int res = 0;
  QModelIndex index = tableView->selectionModel()->currentIndex();
  if (index.isValid()) {
    res = tableView->model()->data(tableView->model()->index(index.row(),0)).toInt();
  }
  return res;
}

void MainWindow::on_actAddSVT_triggered()
{
  if (DB::addSVT()){
    //    tmSVT->select();
  }

}

void MainWindow::on_actDelSVT_triggered()
{
  int SVTId = currentId(ui->tvSVT);
  if (SVTId) {
    if (DB::delSVT(SVTId)) {
      //      tmSVT->select();
    }
  }

}

void MainWindow::on_actEditSVT_triggered()
{
  int SVTId = currentId(ui->tvSVT);
  if (SVTId) {
    if (DB::editSVT(SVTId)) {
      //      tmSVT->select();
    }
  }
}


void MainWindow::on_actAddPerson_triggered()
{
  if (DB::addPerson()) {
    //   tmPerson->select();
  }
}

void MainWindow::on_actDelPerson_triggered()
{
  int personId = currentId(ui->tvPerson);
  if (personId) {
    if (DB::delPerson(personId)) {
      //      tmPerson->select();
    }
  }
}

void MainWindow::on_actEditPerson_triggered()
{
  int personId = currentId(ui->tvPerson);
  if (personId) {
    if (DB::editPerson(personId)) {
      //      tmPerson->select();
    }
  }
}

MainWindow *MainWindow::w()
{
  return m_w;
}

void MainWindow::on_actDelMni_triggered()
{
  int IdMni = currentId(ui->tvMni);
  if (IdMni) {
    DB::delMNI(IdMni);
  }
}

void MainWindow::on_actEditMni_triggered()
{
  int IdMni = currentId(ui->tvMni);
  if (IdMni) {
    DB::editMNI(IdMni);
  }

}

void MainWindow::on_actDeployRules_triggered()
{
  int SVTId = currentId(ui->tvSVT);
  if (SVTId) {
    if (Deployer::doDeploy(Deployer::writeRuleFile(SVTId),SVTId)) {
      QMessageBox::information(this,"Применение правил","Правила успешно применены");
    } else {
      QMessageBox::critical(this,"Применение правил","Ошибка применения файлов");
    }
  }

}

void MainWindow::on_actDeployAll_triggered()
{
  int succesCount = 0;
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  for (int i = 0; i < tmSVT->rowCount(); ++i) {
    int SVTId = tmSVT->data(tmSVT->index(i,0)).toInt();
    if (Deployer::doDeploy(Deployer::writeRuleFile(SVTId),SVTId)) {
      succesCount++;
    }
  }
  QApplication::restoreOverrideCursor();
  QMessageBox::information(this,"Применение правил",
                           QString("Правила успешно применены на %1 СВТ из %2").arg(succesCount).arg(tmSVT->rowCount()));
}

void MainWindow::updateRulesVersion()
{
  tmSVT->actualRules = DB::freshRules();
  tmSVT->select();
}

void MainWindow::parseNotify(const QString &notification)
{
  if (notification=="report") {
    tmReport->select();
  }
}
