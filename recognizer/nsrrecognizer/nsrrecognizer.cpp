#include "nsrrecognizer.h"

#include <apmrec.h>
#include <f32file.h>
#include <ImplementationProxy.h>

const TUid KRecognizerUid = {/*0x2005FFA7*/0x20049F7A};
const TInt KRecognizerImplUid = {/*0x2005FFA7*/0x20049F7A};
const TInt KMaxBufferSize = 128;

_LIT8(KMIMEPdf, "application/pdf");
_LIT8(KMIMEDjvu, "image/vnd.djvu");
_LIT8(KMIMETiff, "image/tiff");
_LIT8(KMIMEText, "text/plain");
_LIT(KPdfExtension, ".pdf");
_LIT(KDjvuExtension, ".djvu");
_LIT(KTiffExtension, ".tiff");
_LIT(KTifExtension, ".tif");
_LIT(KTextExtension, ".txt");
 
NSRRecognizer::NSRRecognizer ()
    : CApaDataRecognizerType (KRecognizerUid, CApaDataRecognizerType::ENormal)
{
    iCountDataTypes = 4;
}
 
NSRRecognizer::~NSRRecognizer ()
{
}
 
TUint NSRRecognizer::PreferredBufSize ()
{
    return KMaxBufferSize; 
}
 
TDataType NSRRecognizer::SupportedDataTypeL (TInt aIndex) const
{
	if (aIndex == 0)
		return TDataType(KMIMEPdf);
	else if (aIndex == 1)
		return TDataType(KMIMEDjvu);
	else if (aIndex == 2)
		return TDataType(KMIMETiff);
	else
		return TDataType(KMIMEText);
}
 
void NSRRecognizer::DoRecognizeL (const TDesC& aName, const TDesC8& aBuffer)
{
    TParse parse;
    parse.Set(aName, NULL, NULL);
    TPtrC ext = parse.Ext();
    if (ext.CompareF(KPdfExtension) == 0) {
        iConfidence = ECertain;
        iDataType = TDataType(KMIMEPdf);
    } else if (ext.CompareF(KDjvuExtension) == 0) {
        iConfidence = ECertain;
        iDataType = TDataType(KMIMEDjvu);
    } else if (ext.CompareF(KTiffExtension) == 0 || ext.CompareF(KTifExtension) == 0) {
    	iConfidence = ECertain;
    	iDataType = TDataType(KMIMETiff);
    } else if (ext.CompareF(KTextExtension) == 0) {
    	iConfidence = ECertain;
    	iDataType = TDataType(KMIMEText);
    }
}
 
CApaDataRecognizerType* NSRRecognizer::CreateRecognizerL()
{
    return new (ELeave) NSRRecognizer();
}
 
const TImplementationProxy ImplementationTable[] =
{
    IMPLEMENTATION_PROXY_ENTRY(KRecognizerImplUid, NSRRecognizer::CreateRecognizerL)
};
 
EXPORT_C const TImplementationProxy* ImplementationGroupProxy (TInt& aTableCount)
{
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
}

