#include "colortablemodel.h"


ColorTableModel::ColorTableModel(QObject *parent, QSqlDatabase db) :
  QSqlRelationalTableModel(parent, db)
{

}

QVariant ColorTableModel::data(const QModelIndex &item, int role) const
{
  if(role == Qt::BackgroundRole) {
    if(item.column()>6) {
      int svtId = data(index(item.row(),0),Qt::DisplayRole).toInt();
      int version = data(index(item.row(),8),Qt::DisplayRole).toInt();
      QColor color;
      if (version < actualRules[svtId]) {
        color = QColor(0xF0,0xA0,0xA0);
      } else {
        color = QColor(0xA0,0xF0,0xA0);
      }
      return QBrush(color);
    }
  }
  return QSqlRelationalTableModel::data(item,role);
}
