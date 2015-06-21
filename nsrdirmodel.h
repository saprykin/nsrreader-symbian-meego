#ifndef NSRDIRMODEL_H
#define NSRDIRMODEL_H

#include "nsrdiritem.h"

#include <QDir>
#include <QAbstractItemModel>

class NSRDirModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit NSRDirModel (QObject *parent = 0);
	virtual ~NSRDirModel ();

	int rowCount (const QModelIndex &parent = QModelIndex ()) const;
	int columnCount (const QModelIndex &parent = QModelIndex ()) const;
	QVariant headerData (int		section,
			     Qt::Orientation	orientation,
			     int		role = Qt::DisplayRole) const;
	QVariant data (const QModelIndex &index, int role) const;
	QModelIndex index (int row, int column, const QModelIndex &parent) const;
	QModelIndex parent (const QModelIndex &child) const;
	Qt::ItemFlags flags (const QModelIndex &index) const;

	void setRootPath (const QString& path);
	void setNameFilters (const QStringList& filters);
	void itemClicked (const QModelIndex& index);
	QString getCurrentPath () const;
	QString getSelectedFile () const {
		return _selectedFile;
	}

signals:
	void fileSelected ();
	void currentPathChanged (const QString& path);

private:
	void updateDataFromDir ();
	void cleanList ();

	QList<NSRDirItem *>	_list;
	QDir			_dir;
	QStringList		_filters;
	QString			_selectedFile;
};

#endif // NSRDIRMODEL_H
