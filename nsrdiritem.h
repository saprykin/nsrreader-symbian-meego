#ifndef NSRDIRITEM_H
#define NSRDIRITEM_H

#include <QString>

class NSRDirItem
{
public:
	NSRDirItem (const QString& name, const QString& fullName, bool isDir);

	QString getName () const {
		return _name;
	}
	void setName (const QString& name) {
		_name = name;
	}

	QString getFullName () const {
		return _fullName;
	}
	void setFullName (const QString& name) {
		_fullName = name;
	}

	bool isDir () const {
		return _isDir;
	}
	void setDir (bool isDir) {
		_isDir = isDir;
	}

private:
	QString	_name;
	QString	_fullName;
	bool	_isDir;
};

#endif // NSRDIRITEM_H
