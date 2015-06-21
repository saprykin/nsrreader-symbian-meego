#include "nsropenfiledialog.h"
#include "ui_nsropenfiledialog.h"
#include "nsrlanguage.h"

#include <QPainter>

NSROpenFileDialog::NSROpenFileDialog (QWidget *parent) :
	QWidget (parent),
	ui (new Ui::NSROpenFileDialog)
{
	const NSRLanguageData *lang = NSRLanguage::instance()->getLanguage ();

	ui->setupUi (this);

	_model = new NSRDirModel (this);
	_model->setRootPath (QDir::homePath ());

	connect (ui->treeView, SIGNAL (clicked (QModelIndex)),
		 this, SLOT (onItemClicked (QModelIndex)));
	connect (_model, SIGNAL (currentPathChanged (QString)),
		 this, SLOT (onDirChanged (QString)));
	connect (_model, SIGNAL (fileSelected ()),
		 this, SLOT (onFileSelected ()));

	ui->treeView->setIconSize (QSize (24, 24));
	ui->treeView->setRootIsDecorated (false);
	ui->treeView->setModel (_model);
	ui->pathLabel->setText (_model->getCurrentPath ());
	ui->cancelButton->setText (lang->mainCancel);
}

NSROpenFileDialog::~NSROpenFileDialog ()
{
	delete ui;
}

void NSROpenFileDialog::setRootPath (const QString &path)
{
	_model->setRootPath (path);
}

void NSROpenFileDialog::setNameFilters (const QStringList &filters)
{
	_model->setNameFilters (filters);
}

void NSROpenFileDialog::execDialog ()
{
	show ();
	_loop.exec ();
	close ();
}

QString NSROpenFileDialog::getSelectedFile () const
{
	return _model->getSelectedFile ();
}

void NSROpenFileDialog::paintEvent (QPaintEvent *ev)
{
	Q_UNUSED (ev);

	QStyleOption opt;
	opt.init (this);
	QPainter p (this);
	style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void NSROpenFileDialog::onItemClicked (const QModelIndex &index)
{
	_model->itemClicked (index);
}

void NSROpenFileDialog::onFileSelected ()
{
	_loop.quit ();
}

void NSROpenFileDialog::onDirChanged (const QString &dir)
{
	ui->pathLabel->setText (dir);
}

void NSROpenFileDialog::on_cancelButton_clicked ()
{
	_loop.quit ();
}
