#include "nsrlistselector.h"
#include "ui_nsrlistselector.h"
#include "nsrlanguage.h"

#include <QPainter>

NSRListSelector::NSRListSelector (QWidget *parent) :
	QWidget (parent),
	ui (new Ui::NSRListSelector),
	_selectedIdx (-1)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	ui->setupUi (this);

	_model = new NSRListModel (this);
	ui->treeView->setModel (_model);
	ui->treeView->setRootIsDecorated (false);
	ui->cancelButton->setText (lang->mainCancel);

	connect (ui->treeView, SIGNAL (clicked (QModelIndex)),
		 this, SLOT (onItemClicked (QModelIndex)));
}

NSRListSelector::~NSRListSelector ()
{
	delete ui;
}

void NSRListSelector::execDialog ()
{
	show ();
	_loop.exec ();
	close ();
}

void NSRListSelector::setPromptText (const QString &text)
{
	ui->promptLabel->setText (text);
}

void NSRListSelector::setItems (const QStringList &list)
{
	_model->setModelData (list);
}

int NSRListSelector::getSelectedIndex () const
{
	return _selectedIdx;
}

void NSRListSelector::paintEvent (QPaintEvent *ev)
{
	Q_UNUSED (ev);

	QStyleOption opt;
	opt.init (this);
	QPainter p (this);
	style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void NSRListSelector::on_cancelButton_clicked ()
{
	_selectedIdx = -1;
	_loop.quit ();
}

void NSRListSelector::onItemClicked (const QModelIndex &idx)
{
	_selectedIdx = idx.row ();
	_loop.quit ();
}
