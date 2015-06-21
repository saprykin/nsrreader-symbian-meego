#ifndef NSRLISTSELECTOR_H
#define NSRLISTSELECTOR_H

#include <QWidget>
#include <QEventLoop>
#include <QModelIndex>

#include "nsrlistmodel.h"

namespace Ui {
class NSRListSelector;
}

class NSRListSelector : public QWidget
{
	Q_OBJECT
	
public:
	explicit NSRListSelector(QWidget *parent = 0);
	~NSRListSelector();

	void execDialog ();
	void setPromptText (const QString& text);
	void setItems (const QStringList& list);
	int getSelectedIndex () const;

protected:
	void paintEvent (QPaintEvent *ev);
	
private slots:
	void on_cancelButton_clicked ();
	void onItemClicked (const QModelIndex& idx);

private:
	Ui::NSRListSelector	*ui;
	QEventLoop		_loop;
	NSRListModel		*_model;
	int			_selectedIdx;
};

#endif // NSRLISTSELECTOR_H
