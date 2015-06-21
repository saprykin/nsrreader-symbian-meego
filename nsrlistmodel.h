#ifndef NSRLISTMODEL_H
#define NSRLISTMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class NSRListModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit NSRListModel (QObject *parent = 0);
	virtual ~NSRListModel () {}

	int rowCount (const QModelIndex &parent = QModelIndex ()) const;
	int columnCount (const QModelIndex &parent = QModelIndex ()) const;
	QVariant headerData (int		section,
			     Qt::Orientation	orientation,
			     int		role = Qt::DisplayRole) const;
	QVariant data (const QModelIndex &index, int role) const;
	QModelIndex index (int row, int column, const QModelIndex &parent) const;
	QModelIndex parent (const QModelIndex &child) const;
	Qt::ItemFlags flags (const QModelIndex &index) const;

	void setModelData (const QStringList& list);

private:
	QStringList	_list;
};

#endif // NSRLISTMODEL_H
