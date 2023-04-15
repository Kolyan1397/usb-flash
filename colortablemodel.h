#ifndef COLORTABLEMODEL_H
#define COLORTABLEMODEL_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QBrush>
#include <QColor>

class ColorTableModel : public QSqlRelationalTableModel
{
public:
  ColorTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
  QMap<int,int> actualRules;
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};

#endif // COLORTABLEMODEL_H
