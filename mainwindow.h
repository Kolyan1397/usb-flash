#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"dialogmni.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include "db.h"
#include "deployer.h"
#include "colortablemodel.h"
#include <QSqlDatabase>
#include <QSqlDriver>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    static MainWindow* m_w;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actAddMni_triggered();

    void on_actAddDivision_triggered();

    void on_actDelDivision_triggered();

    void on_actEditDivision_triggered();

    void on_actAddSVT_triggered();

    void on_actDelSVT_triggered();

    void on_actEditSVT_triggered();

    void on_actAddPerson_triggered();

    void on_actDelPerson_triggered();

    void on_actEditPerson_triggered();

    void on_actDelMni_triggered();

    void on_actEditMni_triggered();

    void on_actDeployRules_triggered();

    void on_actDeployAll_triggered();

    void updateRulesVersion();

    void parseNotify(const QString& notification);

public:
    Ui::MainWindow *ui;
    static MainWindow* w();
private:
    QSqlTableModel * tmDivision;
    ColorTableModel * tmSVT;
    QSqlRelationalTableModel * tmMni;
    QSqlTableModel * tmPerson;
    QSqlTableModel * tmReport;



    int currentId(QTableView *tableView);

    void initDivision();
    void initSVT();
    void initPerson();
    void initMNI();
    void initReport();

signals:
    void mniChanged();
    void svtChanged();
    void personChanged();
    void divisionChanged();
    void rulesChanged();

};

#endif // MAINWINDOW_H
