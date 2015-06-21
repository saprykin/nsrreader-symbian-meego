#ifndef NSROPENFILEDIALOG_H
#define NSROPENFILEDIALOG_H

#include "nsrdirmodel.h"

#include <QWidget>
#include <QFileSystemModel>
#include <QEventLoop>

namespace Ui {
class NSROpenFileDialog;
}

class NSROpenFileDialog : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSROpenFileDialog (QWidget *parent = 0);
	virtual ~NSROpenFileDialog ();

	void setRootPath (const QString& path);
	void setNameFilters (const QStringList& filters);
	void execDialog ();
	QString getSelectedFile () const;

protected:
	void paintEvent (QPaintEvent *ev);

private slots:
	void onItemClicked (const QModelIndex& index);
	void onFileSelected ();
	void onDirChanged (const QString& dir);
	void on_cancelButton_clicked ();

private:
	Ui::NSROpenFileDialog	*ui;
	QEventLoop		_loop;
	NSRDirModel		*_model;
};

#endif // NSROPENFILEDIALOG_H
