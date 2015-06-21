#ifndef __NSRRECOGNIZER_H__
#define __NSRRECOGNIZER_H__

#include <apmrec.h>

class NSRRecognizer : public CApaDataRecognizerType
{
public:
	NSRRecognizer();
	static CApaDataRecognizerType* CreateRecognizerL();
	virtual ~NSRRecognizer();

public:
	TUint PreferredBufSize();
	TDataType SupportedDataTypeL(TInt aIndex) const;

private:
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
};

#endif /* __NSRRECOGNIZER_H__ */
