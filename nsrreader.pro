DEFINES += NSR_SYMBIAN_S3
DEFINES += NSR_SYMBIAN_BELLE

TARGET = nsrreader
DEPLOYMENT.display_name = "NSR Reader"

VERSION = 2.1.2

vendorinfo = \
        "%{\"Saprykin Alexander\"}" \
        ":\"Saprykin Alexander\""

vendor_deployment.pkg_prerules = vendorinfo
DEPLOYMENT += vendor_deployment


fontconfigFiles.sources = config\\fonts.conf	\
			  config\\fonts.dtd
fontconfigFiles.path = c:\\data\\nsrreader
DEPLOYMENT += fontconfigFiles

rssresources =      "SOURCEPATH	."				\
		    "START RESOURCE dialogs_20049f7a.rss"	\
		    "HEADER"					\
		    "TARGETPATH resource\\apps"			\
		    "END"
MMP_RULES += rssresources

RSS_RULES += "datatype_list = {"						\
	     "                DATATYPE {"					\
	     "                              priority = EDataTypePriorityHigh;"	\
	     "                              type = \"application/pdf\";"	\
	     "                         },"					\
	     "                DATATYPE {"					\
	     "                              priority = EDataTypePriorityHigh;"	\
	     "                              type = \"image/vnd.djvu\";"		\
	     "                         },"					\
	     "                DATATYPE {"					\
	     "                              priority = EDataTypePriorityHigh;"	\
	     "                              type = \"image/tiff\";"		\
	     "                         },"					\
	     "                DATATYPE {"					\
	     "                              priority = EDataTypePriorityHigh;"	\
	     "                              type = \"text/plain\";"		\
	     "                         }"					\
	     "                };"

contains(DEFINES, NSR_SYMBIAN_BELLE) {
	addFiles.pkg_postrules = "\"/QtSDK/Symbian/SDKs/SymbianSR1Qt474/epoc32/data/z/resource/apps/dialogs_20049f7a.rsc\" \
				 - \"!:\\resource\\apps\\dialogs_20049f7a.rsc\""
        addRecognizer.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/SymbianBelle/nsrrecognizer.dll\" - \"!:\\sys\\bin\\nsrrecognizer.dll\""
        addRecognizer2.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/SymbianBelle/nsrrecognizer.rsc\" - \"!:\\resource\\plugins\\nsrrecognizer.rsc\""
} else:contains(DEFINES, NSR_SYMBIAN_S3) {
	addFiles.pkg_postrules = "\"/QtSDK/Symbian/SDKs/Symbian3Qt474/epoc32/data/z/resource/apps/dialogs_20049f7a.rsc\" \
				 - \"!:\\resource\\apps\\dialogs_20049f7a.rsc\""
        addRecognizer.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/Symbian^3/nsrrecognizer.dll\" - \"!:\\sys\\bin\\nsrrecognizer.dll\""
        addRecognizer2.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/Symbian^3/nsrrecognizer.rsc\" - \"!:\\resource\\plugins\\nsrrecognizer.rsc\""
} else {
	addFiles.pkg_postrules = "\"/QtSDK/Symbian/SDKs/Symbian1Qt473/epoc32/data/z/resource/apps/dialogs_20049f7a.rsc\" \
				 - \"!:\\resource\\apps\\dialogs_20049f7a.rsc\""
        addRecognizer.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/Symbian^1/nsrrecognizer.dll\" - \"!:\\sys\\bin\\nsrrecognizer.dll\""
        addRecognizer2.pkg_postrules = "\"/Projects/nsrreader-symbian-meego/recognizer/Symbian^1/nsrrecognizer.rsc\" - \"!:\\resource\\plugins\\nsrrecognizer.rsc\""
}

DEPLOYMENT += addFiles
#DEPLOYMENT += addRecognizer
#DEPLOYMENT += addRecognizer2
OTHER_FILES += nativesymbiandlg.hrh

#TARGET.UID3 = 0x20049F7A
TARGET.UID3 = 0xF0234F34

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

TARGET.CAPABILITY += NetworkServices
# ProtServ

contains(DEFINES, NSR_SYMBIAN_S3) {
	TARGET.EPOCHEAPSIZE = 0x20000 0x5000000
} else {
	TARGET.EPOCHEAPSIZE = 0x20000 0x3000000
}

QT += xml gui

DEFINES += HAS_WCHAR HAVE_STDINT_H HAVE_STDLIB_H HAVE_SPLASH HAVE_DIRENT_H HAS_MBSTATE HAVE_STDINCLUDES HAVE_RANDOM \
	ENABLE_LIBJPEG ENABLE_ZLIB

SOURCES += main.cpp mainwindow.cpp		\
	nsrimagecontainer.cpp			\
	nsrstartlabel.cpp			\
	nsrdocumentmenu.cpp			\
	nsrabstractdocument.cpp			\
	nsrpopplerdocument.cpp			\
	nsrscrollarea.cpp			\
	nsrtoolframe.cpp			\
	nsrpreferences.cpp			\
	nsrsettings.cpp				\
	nsrtogglebutton.cpp			\
	nsrsession.cpp				\
	nsranimationlabel.cpp			\
	nsrrenderthread.cpp			\
	nsraboutdialog.cpp			\
	nsrtouchdialog.cpp			\
	nsrtextedit.cpp				\
	nsrkineticscroller.cpp			\
	nsrmaindocument.cpp			\
	nsrdjvudocument.cpp			\
	nsrnewsdialog.cpp			\
	nsrlanguage.cpp				\
	nsrtextdocument.cpp			\
	nsrtiffdocument.cpp			\
	poppler/goo/gfile.cc			\
	poppler/goo/GooHash.cc			\
	poppler/goo/GooList.cc			\
	poppler/goo/GooString.cc		\
	poppler/goo/gmem.cc			\
	poppler/goo/gstrtod.cc			\
	poppler/goo/FixedPoint.cc		\
        poppler/fofi/FoFiBase.cc		\
	poppler/fofi/FoFiEncodings.cc		\
        poppler/fofi/FoFiTrueType.cc		\
        poppler/fofi/FoFiType1.cc		\
	poppler/fofi/FoFiType1C.cc		\
        poppler/poppler/Annot.cc		\
        poppler/poppler/Array.cc 		\
        poppler/poppler/BuiltinFont.cc		\
        poppler/poppler/BuiltinFontTables.cc	\
        poppler/poppler/CachedFile.cc		\
        poppler/poppler/Catalog.cc 		\
        poppler/poppler/CharCodeToUnicode.cc	\
        poppler/poppler/CMap.cc			\
        poppler/poppler/DateInfo.cc		\
        poppler/poppler/Decrypt.cc		\
        poppler/poppler/Dict.cc 		\
        poppler/poppler/Error.cc 		\
        poppler/poppler/FileSpec.cc		\
	poppler/poppler/FlateStream.cc		\
        poppler/poppler/FontEncodingTables.cc	\
        poppler/poppler/Form.cc 		\
        poppler/poppler/FontInfo.cc		\
        poppler/poppler/Function.cc		\
        poppler/poppler/Gfx.cc 			\
        poppler/poppler/GfxFont.cc 		\
        poppler/poppler/GfxState.cc		\
        poppler/poppler/GlobalParams.cc		\
        poppler/poppler/Hints.cc		\
        poppler/poppler/JArithmeticDecoder.cc	\
        poppler/poppler/JBIG2Stream.cc		\
        poppler/poppler/Lexer.cc 		\
        poppler/poppler/Linearization.cc 	\
        poppler/poppler/Link.cc 		\
        poppler/poppler/LocalPDFDocBuilder.cc	\
        poppler/poppler/Movie.cc                \
        poppler/poppler/NameToCharCode.cc	\
        poppler/poppler/Object.cc 		\
        poppler/poppler/OptionalContent.cc	\
        poppler/poppler/Outline.cc		\
        poppler/poppler/OutputDev.cc 		\
        poppler/poppler/Page.cc 		\
        poppler/poppler/PageTransition.cc	\
        poppler/poppler/Parser.cc 		\
        poppler/poppler/PDFDoc.cc 		\
        poppler/poppler/PDFDocEncoding.cc	\
        poppler/poppler/PDFDocFactory.cc	\
        poppler/poppler/PopplerCache.cc		\
        poppler/poppler/PSTokenizer.cc		\
        poppler/poppler/Rendition.cc		\
        poppler/poppler/StdinCachedFile.cc	\
        poppler/poppler/StdinPDFDocBuilder.cc	\
        poppler/poppler/Stream.cc 		\
        poppler/poppler/strtok_r.cpp		\
        poppler/poppler/UnicodeMap.cc		\
        poppler/poppler/UnicodeTypeTable.cc	\
        poppler/poppler/XRef.cc			\
        poppler/poppler/TextOutputDev.cc	\
        poppler/poppler/PageLabelInfo.cc	\
        poppler/poppler/SecurityHandler.cc	\
        poppler/poppler/Sound.cc		\
	poppler/poppler/XpdfPluginAPI.cc	\
	poppler/poppler/JPXStream.cc		\
	poppler/poppler/SplashOutputDev.cc	\
	poppler/splash/Splash.cc		\
	poppler/splash/SplashBitmap.cc		\
	poppler/splash/SplashClip.cc		\
	poppler/splash/SplashFTFont.cc		\
	poppler/splash/SplashFTFontEngine.cc	\
	poppler/splash/SplashFTFontFile.cc	\
	poppler/splash/SplashFont.cc		\
	poppler/splash/SplashFontEngine.cc	\
	poppler/splash/SplashFontFile.cc	\
	poppler/splash/SplashFontFileID.cc	\
	poppler/splash/SplashPath.cc		\
	poppler/splash/SplashPattern.cc		\
	poppler/splash/SplashScreen.cc		\
	poppler/splash/SplashState.cc		\
	poppler/splash/SplashXPath.cc		\
	poppler/splash/SplashXPathScanner.cc	\
	poppler/poppler/DCTStream.cc		\
	poppler/poppler/ViewerPreferences.cc	\
	poppler/poppler/ProfileData.cc		\
	poppler/goo/GooTimer.cc			\
	libjpeg/jutils.c			\
	libjpeg/jquant2.c			\
	libjpeg/jquant1.c			\
	libjpeg/jmemnobs.c			\
	libjpeg/jmemmgr.c			\
	libjpeg/jidctred.c			\
	libjpeg/jidctint.c			\
	libjpeg/jidctfst.c			\
	libjpeg/jidctflt.c			\
	libjpeg/jfdctint.c			\
	libjpeg/jfdctfst.c			\
	libjpeg/jfdctflt.c			\
	libjpeg/jerror.c			\
	libjpeg/jdtrans.c			\
	libjpeg/jdsample.c			\
	libjpeg/jdpostct.c			\
	libjpeg/jdphuff.c			\
	libjpeg/jdmerge.c			\
	libjpeg/jdmaster.c			\
	libjpeg/jdmarker.c			\
	libjpeg/jdmainct.c			\
	libjpeg/jdinput.c			\
	libjpeg/jdhuff.c			\
	libjpeg/jddctmgr.c			\
	libjpeg/jdcolor.c			\
	libjpeg/jdcoefct.c			\
	libjpeg/jdatasrc.c			\
	libjpeg/jdatadst.c			\
	libjpeg/jdapistd.c			\
	libjpeg/jdapimin.c			\
	libjpeg/jctrans.c			\
	libjpeg/jcsample.c			\
	libjpeg/jcprepct.c			\
	libjpeg/jcphuff.c			\
	libjpeg/jcparam.c			\
	libjpeg/jcomapi.c			\
	libjpeg/jcmaster.c			\
	libjpeg/jcmarker.c			\
	libjpeg/jcmainct.c			\
	libjpeg/jcinit.c			\
	libjpeg/jchuff.c			\
	libjpeg/jcdctmgr.c			\
	libjpeg/jccolor.c			\
	libjpeg/jccoefct.c			\
	libjpeg/jcapistd.c			\
	libjpeg/jcapimin.c			\
	libjpeg/jdmrgext.c			\
	libjpeg/jdcolext.c			\
	libjpeg/jdarith.c			\
	libjpeg/jccolext.c			\
	libjpeg/jcarith.c			\
	libjpeg/jaricom.c			\
	libjpeg/jsimd_none.c			\
	libdjvu/ZPCodec.cpp			\
	libdjvu/XMLTags.cpp			\
	libdjvu/XMLParser.cpp			\
	libdjvu/UnicodeByteStream.cpp		\
	libdjvu/MMX.cpp				\
	libdjvu/MMRDecoder.cpp			\
	libdjvu/miniexp.cpp			\
	libdjvu/JPEGDecoder.cpp			\
	libdjvu/JB2Image.cpp			\
	libdjvu/JB2EncodeCodec.cpp		\
	libdjvu/IW44Image.cpp			\
	libdjvu/IW44EncodeCodec.cpp		\
	libdjvu/IFFByteStream.cpp		\
	libdjvu/GURL.cpp			\
	libdjvu/GUnicode.cpp			\
	libdjvu/GThreads.cpp			\
	libdjvu/GString.cpp			\
	libdjvu/GSmartPointer.cpp		\
	libdjvu/GScaler.cpp			\
	libdjvu/GRect.cpp			\
	libdjvu/GPixmap.cpp			\
	libdjvu/GOS.cpp				\
	libdjvu/GMapAreas.cpp			\
	libdjvu/GIFFManager.cpp			\
	libdjvu/GException.cpp			\
	libdjvu/GContainer.cpp			\
	libdjvu/GBitmap.cpp			\
	libdjvu/DjVuToPS.cpp			\
	libdjvu/DjVuText.cpp			\
	libdjvu/DjVuPort.cpp			\
	libdjvu/DjVuPalette.cpp			\
	libdjvu/DjVuNavDir.cpp			\
	libdjvu/DjVuMessageLite.cpp		\
	libdjvu/DjVuMessage.cpp			\
	libdjvu/DjVuInfo.cpp			\
	libdjvu/DjVuImage.cpp			\
	libdjvu/DjVuGlobalMemory.cpp		\
	libdjvu/DjVuGlobal.cpp			\
	libdjvu/DjVuFileCache.cpp		\
	libdjvu/DjVuFile.cpp			\
	libdjvu/DjVuErrorList.cpp		\
	libdjvu/DjVuDumpHelper.cpp		\
	libdjvu/DjVuDocument.cpp		\
	libdjvu/DjVuDocEditor.cpp		\
	libdjvu/DjVuAnno.cpp			\
	libdjvu/DjVmNav.cpp			\
	libdjvu/DjVmDoc.cpp			\
	libdjvu/DjVmDir0.cpp			\
	libdjvu/DjVmDir.cpp			\
	libdjvu/debug.cpp			\
	libdjvu/ddjvuapi.cpp			\
	libdjvu/DataPool.cpp			\
	libdjvu/ByteStream.cpp			\
	libdjvu/BSEncodeByteStream.cpp		\
	libdjvu/BSByteStream.cpp		\
	libdjvu/atomic.cpp			\
	libdjvu/Arrays.cpp			\
	freetype/src/base/ftxf86.c		\
	freetype/src/base/ftwinfnt.c		\
	freetype/src/base/ftutil.c		\
	freetype/src/base/fttype1.c		\
	freetype/src/base/fttrigon.c		\
	freetype/src/base/ftsystem.c		\
	freetype/src/base/ftsynth.c		\
	freetype/src/base/ftstroke.c		\
	freetype/src/base/ftstream.c		\
	freetype/src/base/ftsnames.c		\
	freetype/src/base/ftrfork.c		\
	freetype/src/base/ftpic.c		\
	freetype/src/base/ftpfr.c		\
	freetype/src/base/ftpatent.c		\
	freetype/src/base/ftoutln.c		\
	freetype/src/base/ftotval.c		\
	freetype/src/base/ftobjs.c		\
	freetype/src/base/ftmm.c		\
	freetype/src/base/ftmac.c		\
	freetype/src/base/ftlcdfil.c		\
	freetype/src/base/ftinit.c		\
	freetype/src/base/ftgxval.c		\
	freetype/src/base/ftglyph.c		\
	freetype/src/base/ftgloadr.c		\
	freetype/src/base/ftgasp.c		\
	freetype/src/base/ftfstype.c		\
	freetype/src/base/ftdebug.c		\
	freetype/src/base/ftdbgmem.c		\
	freetype/src/base/ftcid.c		\
	freetype/src/base/ftcalc.c		\
	freetype/src/base/ftbitmap.c		\
	freetype/src/base/ftbdf.c		\
	freetype/src/base/ftbbox.c		\
	freetype/src/base/ftbase.c		\
	freetype/src/base/ftapi.c		\
	freetype/src/base/ftadvanc.c		\
	freetype/src/base/basepic.c		\
	freetype/src/autofit/autofit.c		\
	freetype/src/autofit/afwarp.c		\
	freetype/src/autofit/afpic.c		\
	freetype/src/autofit/afmodule.c		\
	freetype/src/autofit/afloader.c		\
	freetype/src/autofit/aflatin2.c		\
	freetype/src/autofit/aflatin.c		\
	freetype/src/autofit/afindic.c		\
	freetype/src/autofit/afhints.c		\
	freetype/src/autofit/afglobal.c		\
	freetype/src/autofit/afdummy.c		\
	freetype/src/autofit/afcjk.c		\
	freetype/src/autofit/afangles.c		\
	freetype/src/bdf/bdflib.c		\
	freetype/src/bdf/bdfdrivr.c		\
	freetype/src/bdf/bdf.c			\
	freetype/src/bzip2/ftbzip2.c		\
	freetype/src/cache/ftcsbits.c		\
	freetype/src/cache/ftcmru.c		\
	freetype/src/cache/ftcmanag.c		\
	freetype/src/cache/ftcimage.c		\
	freetype/src/cache/ftcglyph.c		\
	freetype/src/cache/ftccmap.c		\
	freetype/src/cache/ftccache.c		\
	freetype/src/cache/ftcbasic.c		\
	freetype/src/cache/ftcache.c		\
	freetype/src/cff/cffpic.c		\
	freetype/src/cff/cffparse.c		\
	freetype/src/cff/cffobjs.c		\
	freetype/src/cff/cffload.c		\
	freetype/src/cff/cffgload.c		\
	freetype/src/cff/cffdrivr.c		\
	freetype/src/cff/cffcmap.c		\
	freetype/src/cff/cff.c			\
	freetype/src/gxvalid/gxvtrak.c		\
	freetype/src/gxvalid/gxvprop.c		\
	freetype/src/gxvalid/gxvopbd.c		\
	freetype/src/gxvalid/gxvmorx5.c		\
	freetype/src/gxvalid/gxvmorx4.c		\
	freetype/src/gxvalid/gxvmorx2.c		\
	freetype/src/gxvalid/gxvmorx1.c		\
	freetype/src/gxvalid/gxvmorx0.c		\
	freetype/src/gxvalid/gxvmorx.c		\
	freetype/src/gxvalid/gxvmort5.c		\
	freetype/src/gxvalid/gxvmort4.c		\
	freetype/src/gxvalid/gxvmort2.c		\
	freetype/src/gxvalid/gxvmort1.c		\
	freetype/src/gxvalid/gxvmort0.c		\
	freetype/src/gxvalid/gxvmort.c		\
	freetype/src/gxvalid/gxvmod.c		\
	freetype/src/gxvalid/gxvlcar.c		\
	freetype/src/gxvalid/gxvkern.c		\
	freetype/src/gxvalid/gxvjust.c		\
	freetype/src/gxvalid/gxvfeat.c		\
	freetype/src/gxvalid/gxvcommn.c		\
	freetype/src/gxvalid/gxvbsln.c		\
	freetype/src/gxvalid/gxvalid.c		\
	freetype/src/gzip/zutil.c		\
	freetype/src/gzip/infutil.c		\
	freetype/src/gzip/inftrees.c		\
	freetype/src/gzip/inflate.c		\
	freetype/src/gzip/infcodes.c		\
	freetype/src/gzip/infblock.c		\
	freetype/src/gzip/ftgzip.c		\
	freetype/src/gzip/adler32.c		\
	freetype/src/lzw/ftzopen.c		\
	freetype/src/lzw/ftlzw.c		\
	freetype/src/otvalid/otvmod.c		\
	freetype/src/otvalid/otvmath.c		\
	freetype/src/otvalid/otvjstf.c		\
	freetype/src/otvalid/otvgsub.c		\
	freetype/src/otvalid/otvgpos.c		\
	freetype/src/otvalid/otvgdef.c		\
	freetype/src/otvalid/otvcommn.c		\
	freetype/src/otvalid/otvbase.c		\
	freetype/src/otvalid/otvalid.c		\
	freetype/src/pcf/pcfutil.c		\
	freetype/src/pcf/pcfread.c		\
	freetype/src/pcf/pcfdrivr.c		\
	freetype/src/pcf/pcf.c			\
	freetype/src/pfr/pfrsbit.c		\
	freetype/src/pfr/pfrobjs.c		\
	freetype/src/pfr/pfrload.c		\
	freetype/src/pfr/pfrgload.c		\
	freetype/src/pfr/pfrdrivr.c		\
	freetype/src/pfr/pfrcmap.c		\
	freetype/src/pfr/pfr.c			\
	freetype/src/psaux/t1decode.c		\
	freetype/src/psaux/t1cmap.c		\
	freetype/src/psaux/psobjs.c		\
	freetype/src/psaux/psconv.c		\
	freetype/src/psaux/psauxmod.c		\
	freetype/src/psaux/psaux.c		\
	freetype/src/psaux/afmparse.c		\
	freetype/src/pshinter/pshrec.c		\
	freetype/src/pshinter/pshpic.c		\
	freetype/src/pshinter/pshmod.c		\
	freetype/src/pshinter/pshinter.c	\
	freetype/src/pshinter/pshglob.c		\
	freetype/src/pshinter/pshalgo.c		\
	freetype/src/psnames/pspic.c		\
	freetype/src/psnames/psnames.c		\
	freetype/src/psnames/psmodule.c		\
	freetype/src/raster/rastpic.c		\
	freetype/src/raster/raster.c		\
	freetype/src/raster/ftrend1.c		\
	freetype/src/raster/ftraster.c		\
	freetype/src/sfnt/ttsbit0.c		\
	freetype/src/sfnt/ttsbit.c		\
	freetype/src/sfnt/ttpost.c		\
	freetype/src/sfnt/ttmtx.c		\
	freetype/src/sfnt/ttload.c		\
	freetype/src/sfnt/ttkern.c		\
	freetype/src/sfnt/ttcmap.c		\
	freetype/src/sfnt/ttbdf.c		\
	freetype/src/sfnt/sfobjs.c		\
	freetype/src/sfnt/sfntpic.c		\
	freetype/src/sfnt/sfnt.c		\
	freetype/src/sfnt/sfdriver.c		\
	freetype/src/smooth/smooth.c		\
	freetype/src/smooth/ftspic.c		\
	freetype/src/smooth/ftsmooth.c		\
	freetype/src/smooth/ftgrays.c		\
	freetype/src/truetype/ttpload.c		\
	freetype/src/truetype/ttpic.c		\
	freetype/src/truetype/ttobjs.c		\
	freetype/src/truetype/ttinterp.c	\
	freetype/src/truetype/ttgxvar.c		\
	freetype/src/truetype/ttgload.c		\
	freetype/src/truetype/ttdriver.c	\
	freetype/src/truetype/truetype.c	\
	freetype/src/type1/type1.c		\
	freetype/src/type1/t1parse.c		\
	freetype/src/type1/t1objs.c		\
	freetype/src/type1/t1load.c		\
	freetype/src/type1/t1gload.c		\
	freetype/src/type1/t1driver.c		\
	freetype/src/type1/t1afm.c		\
	freetype/src/type42/type42.c		\
	freetype/src/type42/t42parse.c		\
	freetype/src/type42/t42objs.c		\
	freetype/src/type42/t42drivr.c		\
	freetype/src/winfonts/winfnt.c		\
	freetype/src/cid/type1cid.c		\
	freetype/src/cid/cidriver.c		\
	freetype/src/cid/cidparse.c		\
	freetype/src/cid/cidobjs.c		\
	freetype/src/cid/cidload.c		\
	freetype/src/cid/cidgload.c		\
	fontconfig/ftglue.c			\
	fontconfig/fcxml.c			\
	fontconfig/fcstr.c			\
	fontconfig/fcserialize.c		\
	fontconfig/fcpat.c			\
	fontconfig/fcname.c			\
	fontconfig/fcmatrix.c			\
	fontconfig/fcmatch.c			\
	fontconfig/fclist.c			\
	fontconfig/fclang.c			\
	fontconfig/fcinit.c			\
	fontconfig/fcfs.c			\
	fontconfig/fcfreetype.c			\
	fontconfig/fcformat.c			\
	fontconfig/fcdir.c			\
	fontconfig/fcdefault.c			\
	fontconfig/fcdbg.c			\
	fontconfig/fccharset.c			\
	fontconfig/fccfg.c			\
	fontconfig/fccache.c			\
	fontconfig/fcblanks.c			\
	fontconfig/fcatomic.c			\
	expat/xmltok_ns.c			\
	expat/xmltok_impl.c			\
	expat/xmltok.c				\
	expat/xmlrole.c				\
	expat/xmlparse.c			\
	zlib/zutil.c				\
	zlib/uncompr.c				\
	zlib/trees.c				\
	zlib/inftrees.c				\
	zlib/inflate.c				\
	zlib/inffast.c				\
	zlib/infback.c				\
	zlib/gzwrite.c				\
	zlib/gzread.c				\
	zlib/gzlib.c				\
	zlib/gzclose.c				\
	zlib/deflate.c				\
	zlib/crc32.c				\
	zlib/compress.c				\
	zlib/adler32.c				\
	libtiff/tif_zip.c			\
	libtiff/tif_write.c			\
	libtiff/tif_warning.c			\
	libtiff/tif_version.c			\
	libtiff/tif_unix.c			\
	libtiff/tif_tile.c			\
	libtiff/tif_thunder.c			\
	libtiff/tif_swab.c			\
	libtiff/tif_strip.c			\
	libtiff/tif_stream.cxx			\
	libtiff/tif_read.c			\
	libtiff/tif_print.c			\
	libtiff/tif_predict.c			\
	libtiff/tif_pixarlog.c			\
	libtiff/tif_packbits.c			\
	libtiff/tif_open.c			\
	libtiff/tif_ojpeg.c			\
	libtiff/tif_next.c			\
	libtiff/tif_lzw.c			\
	libtiff/tif_lzma.c			\
	libtiff/tif_luv.c			\
	libtiff/tif_jbig.c			\
	libtiff/tif_getimage.c			\
	libtiff/tif_flush.c			\
	libtiff/tif_fax3sm.c			\
	libtiff/tif_fax3.c			\
	libtiff/tif_extension.c			\
	libtiff/tif_error.c			\
	libtiff/tif_dumpmode.c			\
	libtiff/tif_dirwrite.c			\
	libtiff/tif_dirread.c			\
	libtiff/tif_dirinfo.c			\
	libtiff/tif_dir.c			\
	libtiff/tif_compress.c			\
	libtiff/tif_color.c			\
	libtiff/tif_codec.c			\
	libtiff/tif_close.c			\
	libtiff/tif_aux.c

INCLUDEPATH += freetype/include
INCLUDEPATH += fontconfig
INCLUDEPATH += libjpeg
INCLUDEPATH += poppler
INCLUDEPATH += poppler/goo
INCLUDEPATH += poppler/fofi
INCLUDEPATH += poppler/include/symbian
INCLUDEPATH += poppler/poppler
INCLUDEPATH += poppler/splash
INCLUDEPATH += libtiff
INCLUDEPATH += libtiff/symbian
INCLUDEPATH += expat

HEADERS += mainwindow.h						\
    nsrabstractdocument.h					\
    nsrpopplerdocument.h					\
    nsrscrollarea.h						\
    nsrtoolframe.h						\
    nsrpreferences.h						\
    nsrsettings.h						\
    nsrtogglebutton.h						\
    nsrsession.h						\
    nsranimationlabel.h						\
    nsrrenderthread.h						\
    nsraboutdialog.h						\
    nsrtouchdialog.h						\
    nsrtextedit.h						\
    nsrimagecontainer.h						\
    nsrstartlabel.h						\
    nsrdocumentmenu.h						\
    iscrollable.h						\
    nsrkineticscroller.h					\
    nsrmaindocument.h						\
    nsrdjvudocument.h						\
    nsrnewsdialog.h						\
    nsrlanguage.h						\
    nsrtextdocument.h						\
    nsrtiffdocument.h						\
    fontconfig/ftglue.h						\
    fontconfig/fcint.h						\
    fontconfig/fcftint.h					\
    fontconfig/fcdeprecate.h					\
    fontconfig/fcarch.h						\
    fontconfig/fc-case/fccase.h					\
    fontconfig/fc-glyphname/fcglyphname.h			\
    fontconfig/fontconfig/fontconfig.h				\
    fontconfig/fontconfig/fcprivate.h				\
    fontconfig/fontconfig/fcfreetype.h				\
    fontconfig/fc-lang/fclang.h					\
    freetype/include/freetype/ttunpat.h				\
    freetype/include/freetype/tttags.h				\
    freetype/include/freetype/tttables.h			\
    freetype/include/freetype/ttnameid.h			\
    freetype/include/freetype/t1tables.h			\
    freetype/include/freetype/ftxf86.h				\
    freetype/include/freetype/ftwinfnt.h			\
    freetype/include/freetype/fttypes.h				\
    freetype/include/freetype/fttrigon.h			\
    freetype/include/freetype/ftsystem.h			\
    freetype/include/freetype/ftsynth.h				\
    freetype/include/freetype/ftstroke.h			\
    freetype/include/freetype/ftsnames.h			\
    freetype/include/freetype/ftsizes.h				\
    freetype/include/freetype/ftrender.h			\
    freetype/include/freetype/ftpfr.h				\
    freetype/include/freetype/ftoutln.h				\
    freetype/include/freetype/ftotval.h				\
    freetype/include/freetype/ftmoderr.h			\
    freetype/include/freetype/ftmodapi.h			\
    freetype/include/freetype/ftmm.h				\
    freetype/include/freetype/ftmac.h				\
    freetype/include/freetype/ftlzw.h				\
    freetype/include/freetype/ftlist.h				\
    freetype/include/freetype/ftlcdfil.h			\
    freetype/include/freetype/ftincrem.h			\
    freetype/include/freetype/ftimage.h				\
    freetype/include/freetype/ftgzip.h				\
    freetype/include/freetype/ftgxval.h				\
    freetype/include/freetype/ftglyph.h				\
    freetype/include/freetype/ftgasp.h				\
    freetype/include/freetype/fterrors.h			\
    freetype/include/freetype/fterrdef.h			\
    freetype/include/freetype/ftcid.h				\
    freetype/include/freetype/ftchapters.h			\
    freetype/include/freetype/ftcache.h				\
    freetype/include/freetype/ftbzip2.h				\
    freetype/include/freetype/ftbitmap.h			\
    freetype/include/freetype/ftbdf.h				\
    freetype/include/freetype/ftbbox.h				\
    freetype/include/freetype/ftadvanc.h			\
    freetype/include/freetype/freetype.h			\
    freetype/include/freetype/config/ftstdlib.h			\
    freetype/include/freetype/config/ftoption.h			\
    freetype/include/freetype/config/ftmodule.h			\
    freetype/include/freetype/config/ftheader.h			\
    freetype/include/freetype/config/ftconfig.h			\
    freetype/include/freetype/internal/tttypes.h		\
    freetype/include/freetype/internal/t1types.h		\
    freetype/include/freetype/internal/sfnt.h			\
    freetype/include/freetype/internal/pshints.h		\
    freetype/include/freetype/internal/psaux.h			\
    freetype/include/freetype/internal/pcftypes.h		\
    freetype/include/freetype/internal/internal.h		\
    freetype/include/freetype/internal/ftvalid.h		\
    freetype/include/freetype/internal/fttrace.h		\
    freetype/include/freetype/internal/ftstream.h		\
    freetype/include/freetype/internal/ftserv.h			\
    freetype/include/freetype/internal/ftrfork.h		\
    freetype/include/freetype/internal/ftpic.h			\
    freetype/include/freetype/internal/ftobjs.h			\
    freetype/include/freetype/internal/ftmemory.h		\
    freetype/include/freetype/internal/ftgloadr.h		\
    freetype/include/freetype/internal/ftdriver.h		\
    freetype/include/freetype/internal/ftdebug.h		\
    freetype/include/freetype/internal/ftcalc.h			\
    freetype/include/freetype/internal/autohint.h		\
    freetype/include/freetype/internal/services/svxf86nm.h	\
    freetype/include/freetype/internal/services/svwinfnt.h	\
    freetype/include/freetype/internal/services/svttglyf.h	\
    freetype/include/freetype/internal/services/svtteng.h	\
    freetype/include/freetype/internal/services/svttcmap.h	\
    freetype/include/freetype/internal/services/svsfnt.h	\
    freetype/include/freetype/internal/services/svpsinfo.h	\
    freetype/include/freetype/internal/services/svpscmap.h	\
    freetype/include/freetype/internal/services/svpostnm.h	\
    freetype/include/freetype/internal/services/svpfr.h		\
    freetype/include/freetype/internal/services/svotval.h	\
    freetype/include/freetype/internal/services/svmm.h		\
    freetype/include/freetype/internal/services/svkern.h	\
    freetype/include/freetype/internal/services/svgxval.h	\
    freetype/include/freetype/internal/services/svgldict.h	\
    freetype/include/freetype/internal/services/svcid.h		\
    freetype/include/freetype/internal/services/svbdf.h		\
    freetype/src/autofit/afwarp.h				\
    freetype/src/autofit/aftypes.h				\
    freetype/src/autofit/afpic.h				\
    freetype/src/autofit/afmodule.h				\
    freetype/src/autofit/afloader.h				\
    freetype/src/autofit/aflatin2.h				\
    freetype/src/autofit/aflatin.h				\
    freetype/src/autofit/afindic.h				\
    freetype/src/autofit/afhints.h				\
    freetype/src/autofit/afglobal.h				\
    freetype/src/autofit/aferrors.h				\
    freetype/src/autofit/afdummy.h				\
    freetype/src/autofit/afcjk.h				\
    freetype/src/autofit/afangles.h				\
    freetype/src/base/ftbase.h					\
    freetype/src/base/basepic.h					\
    freetype/src/bdf/bdferror.h					\
    freetype/src/bdf/bdfdrivr.h					\
    freetype/src/bdf/bdf.h					\
    freetype/src/cache/ftcsbits.h				\
    freetype/src/cache/ftcmru.h					\
    freetype/src/cache/ftcmanag.h				\
    freetype/src/cache/ftcimage.h				\
    freetype/src/cache/ftcglyph.h				\
    freetype/src/cache/ftcerror.h				\
    freetype/src/cache/ftccback.h				\
    freetype/src/cache/ftccache.h				\
    freetype/src/cff/cfftypes.h					\
    freetype/src/cff/cfftoken.h					\
    freetype/src/cff/cffpic.h					\
    freetype/src/cff/cffparse.h					\
    freetype/src/cff/cffobjs.h					\
    freetype/src/cff/cffload.h					\
    freetype/src/cff/cffgload.h					\
    freetype/src/cff/cfferrs.h					\
    freetype/src/cff/cffdrivr.h					\
    freetype/src/cff/cffcmap.h					\
    freetype/src/cid/cidtoken.h					\
    freetype/src/cid/cidriver.h					\
    freetype/src/cid/cidparse.h					\
    freetype/src/cid/cidobjs.h					\
    freetype/src/cid/cidload.h					\
    freetype/src/cid/cidgload.h					\
    freetype/src/cid/ciderrs.h					\
    freetype/src/gxvalid/gxvmorx.h				\
    freetype/src/gxvalid/gxvmort.h				\
    freetype/src/gxvalid/gxvmod.h				\
    freetype/src/gxvalid/gxvfeat.h				\
    freetype/src/gxvalid/gxverror.h				\
    freetype/src/gxvalid/gxvcommn.h				\
    freetype/src/gxvalid/gxvalid.h				\
    freetype/src/gzip/zutil.h					\
    freetype/src/gzip/zlib.h					\
    freetype/src/gzip/zconf.h					\
    freetype/src/gzip/infutil.h					\
    freetype/src/gzip/inftrees.h				\
    freetype/src/gzip/inffixed.h				\
    freetype/src/gzip/infcodes.h				\
    freetype/src/gzip/infblock.h				\
    freetype/src/lzw/ftzopen.h					\
    freetype/src/otvalid/otvmod.h				\
    freetype/src/otvalid/otvgpos.h				\
    freetype/src/otvalid/otverror.h				\
    freetype/src/otvalid/otvcommn.h				\
    freetype/src/otvalid/otvalid.h				\
    freetype/src/pcf/pcfutil.h					\
    freetype/src/pcf/pcfread.h					\
    freetype/src/pcf/pcferror.h					\
    freetype/src/pcf/pcfdrivr.h					\
    freetype/src/pcf/pcf.h					\
    freetype/src/pfr/pfrtypes.h					\
    freetype/src/pfr/pfrsbit.h					\
    freetype/src/pfr/pfrobjs.h					\
    freetype/src/pfr/pfrload.h					\
    freetype/src/pfr/pfrgload.h					\
    freetype/src/pfr/pfrerror.h					\
    freetype/src/pfr/pfrdrivr.h					\
    freetype/src/pfr/pfrcmap.h					\
    freetype/src/psaux/t1decode.h				\
    freetype/src/psaux/t1cmap.h					\
    freetype/src/psaux/psobjs.h					\
    freetype/src/psaux/psconv.h					\
    freetype/src/psaux/psauxmod.h				\
    freetype/src/psaux/psauxerr.h				\
    freetype/src/psaux/afmparse.h				\
    freetype/src/pshinter/pshrec.h				\
    freetype/src/pshinter/pshpic.h				\
    freetype/src/pshinter/pshnterr.h				\
    freetype/src/pshinter/pshmod.h				\
    freetype/src/pshinter/pshglob.h				\
    freetype/src/pshinter/pshalgo.h				\
    freetype/src/psnames/pstables.h				\
    freetype/src/psnames/pspic.h				\
    freetype/src/psnames/psnamerr.h				\
    freetype/src/psnames/psmodule.h				\
    freetype/src/raster/rastpic.h				\
    freetype/src/raster/rasterrs.h				\
    freetype/src/raster/ftrend1.h				\
    freetype/src/raster/ftraster.h				\
    freetype/src/raster/ftmisc.h				\
    freetype/src/sfnt/ttsbit.h					\
    freetype/src/sfnt/ttpost.h					\
    freetype/src/sfnt/ttmtx.h					\
    freetype/src/sfnt/ttload.h					\
    freetype/src/sfnt/ttkern.h					\
    freetype/src/sfnt/ttcmapc.h					\
    freetype/src/sfnt/ttcmap.h					\
    freetype/src/sfnt/ttbdf.h					\
    freetype/src/sfnt/sfobjs.h					\
    freetype/src/sfnt/sfntpic.h					\
    freetype/src/sfnt/sferrors.h				\
    freetype/src/sfnt/sfdriver.h				\
    freetype/src/smooth/ftspic.h				\
    freetype/src/smooth/ftsmooth.h				\
    freetype/src/smooth/ftsmerrs.h				\
    freetype/src/smooth/ftgrays.h				\
    freetype/src/truetype/ttpload.h				\
    freetype/src/truetype/ttpic.h				\
    freetype/src/truetype/ttobjs.h				\
    freetype/src/truetype/ttinterp.h				\
    freetype/src/truetype/ttgxvar.h				\
    freetype/src/truetype/ttgload.h				\
    freetype/src/truetype/tterrors.h				\
    freetype/src/truetype/ttdriver.h				\
    freetype/src/type1/t1tokens.h				\
    freetype/src/type1/t1parse.h				\
    freetype/src/type1/t1objs.h					\
    freetype/src/type1/t1load.h					\
    freetype/src/type1/t1gload.h				\
    freetype/src/type1/t1errors.h				\
    freetype/src/type1/t1driver.h				\
    freetype/src/type1/t1afm.h					\
    freetype/src/type42/t42types.h				\
    freetype/src/type42/t42parse.h				\
    freetype/src/type42/t42objs.h				\
    freetype/src/type42/t42error.h				\
    freetype/src/type42/t42drivr.h				\
    freetype/src/winfonts/winfnt.h				\
    freetype/src/winfonts/fnterrs.h				\
    expat/xmltok_impl.h						\
    expat/xmltok.h						\
    expat/xmlrole.h						\
    expat/winconfig.h						\
    expat/utf8tab.h						\
    expat/symbianconfig.h					\
    expat/nametab.h						\
    expat/macconfig.h						\
    expat/latin1tab.h						\
    expat/internal.h						\
    expat/iasciitab.h						\
    expat/expat_external.h					\
    expat/expat.h						\
    expat/asciitab.h						\
    expat/ascii.h						\
    expat/amigaconfig.h						\
    libjpeg/jversion.h						\
    libjpeg/jpeglib.h						\
    libjpeg/jpegint.h						\
    libjpeg/jmorecfg.h						\
    libjpeg/jmemsys.h						\
    libjpeg/jinclude.h						\
    libjpeg/jerror.h						\
    libjpeg/jdhuff.h						\
    libjpeg/jdct.h						\
    libjpeg/jconfig.h						\
    libjpeg/jchuff.h						\
    libjpeg/jsimddct.h						\
    libjpeg/jsimd.h						\
    libjpeg/jpegcomp.h						\
    poppler/include/symbian/config.h				\
    poppler/include/symbian/poppler-config.h			\
    poppler/fofi/FoFiType1C.h					\
    poppler/fofi/FoFiType1.h					\
    poppler/fofi/FoFiTrueType.h					\
    poppler/fofi/FoFiEncodings.h				\
    poppler/fofi/FoFiBase.h					\
    poppler/goo/gtypes_p.h					\
    poppler/goo/gtypes.h					\
    poppler/goo/gstrtod.h					\
    poppler/goo/GooTimer.h					\
    poppler/goo/GooString.h					\
    poppler/goo/GooMutex.h					\
    poppler/goo/GooList.h					\
    poppler/goo/GooLikely.h					\
    poppler/goo/GooHash.h					\
    poppler/goo/gmem.h						\
    poppler/goo/gfile.h						\
    poppler/goo/FixedPoint.h					\
    poppler/splash/SplashXPathScanner.h				\
    poppler/splash/SplashXPath.h				\
    poppler/splash/SplashTypes.h				\
    poppler/splash/SplashT1FontFile.h				\
    poppler/splash/SplashT1FontEngine.h				\
    poppler/splash/SplashT1Font.h				\
    poppler/splash/SplashState.h				\
    poppler/splash/SplashScreen.h				\
    poppler/splash/SplashPattern.h				\
    poppler/splash/SplashPath.h					\
    poppler/splash/SplashMath.h					\
    poppler/splash/SplashGlyphBitmap.h				\
    poppler/splash/SplashFTFontFile.h				\
    poppler/splash/SplashFTFontEngine.h				\
    poppler/splash/SplashFTFont.h				\
    poppler/splash/SplashFontFileID.h				\
    poppler/splash/SplashFontFile.h				\
    poppler/splash/SplashFontEngine.h				\
    poppler/splash/SplashFont.h					\
    poppler/splash/SplashErrorCodes.h				\
    poppler/splash/SplashClip.h					\
    poppler/splash/SplashBitmap.h				\
    poppler/splash/Splash.h					\
    poppler/poppler/XRef.h					\
    poppler/poppler/XpdfPluginAPI.h				\
    poppler/poppler/ViewerPreferences.h				\
    poppler/poppler/UTF8.h					\
    poppler/poppler/UnicodeTypeTable.h				\
    poppler/poppler/UnicodeMapTables.h				\
    poppler/poppler/UnicodeMap.h				\
    poppler/poppler/UnicodeDecompTables.h			\
    poppler/poppler/UnicodeCompTables.h				\
    poppler/poppler/UnicodeCClassTables.h			\
    poppler/poppler/TextOutputDev.h				\
    poppler/poppler/Stream-CCITT.h				\
    poppler/poppler/Stream.h					\
    poppler/poppler/StdinPDFDocBuilder.h			\
    poppler/poppler/StdinCachedFile.h				\
    poppler/poppler/SplashOutputDev.h				\
    poppler/poppler/Sound.h					\
    poppler/poppler/SecurityHandler.h				\
    poppler/poppler/Rendition.h					\
    poppler/poppler/PSTokenizer.h				\
    poppler/poppler/ProfileData.h				\
    poppler/poppler/PreScanOutputDev.h				\
    poppler/poppler/PopplerCache.h				\
    poppler/poppler/PDFDocFactory.h				\
    poppler/poppler/PDFDocEncoding.h				\
    poppler/poppler/PDFDocBuilder.h				\
    poppler/poppler/PDFDoc.h					\
    poppler/poppler/Parser.h					\
    poppler/poppler/PageTransition.h				\
    poppler/poppler/PageLabelInfo.h				\
    poppler/poppler/Page.h					\
    poppler/poppler/OutputDev.h					\
    poppler/poppler/Outline.h					\
    poppler/poppler/OptionalContent.h				\
    poppler/poppler/Object.h					\
    poppler/poppler/NameToUnicodeTable.h			\
    poppler/poppler/NameToCharCode.h				\
    poppler/poppler/Movie.h					\
    poppler/poppler/LocalPDFDocBuilder.h			\
    poppler/poppler/Link.h					\
    poppler/poppler/Linearization.h				\
    poppler/poppler/Lexer.h					\
    poppler/poppler/JPXStream.h					\
    poppler/poppler/JBIG2Stream.h				\
    poppler/poppler/JArithmeticDecoder.h			\
    poppler/poppler/Hints.h					\
    poppler/poppler/GlobalParams.h				\
    poppler/poppler/GfxState_helpers.h				\
    poppler/poppler/GfxState.h					\
    poppler/poppler/GfxFont.h					\
    poppler/poppler/Gfx.h					\
    poppler/poppler/Function.h					\
    poppler/poppler/Form.h					\
    poppler/poppler/FontInfo.h					\
    poppler/poppler/FontEncodingTables.h			\
    poppler/poppler/FlateStream.h				\
    poppler/poppler/FileSpec.h					\
    poppler/poppler/ErrorCodes.h				\
    poppler/poppler/Error.h					\
    poppler/poppler/Dict.h					\
    poppler/poppler/Decrypt.h					\
    poppler/poppler/DCTStream.h					\
    poppler/poppler/DateInfo.h					\
    poppler/poppler/CurlPDFDocBuilder.h				\
    poppler/poppler/CurlCachedFile.h				\
    poppler/poppler/CompactFontTables.h				\
    poppler/poppler/CMap.h					\
    poppler/poppler/CharTypes.h					\
    poppler/poppler/CharCodeToUnicode.h				\
    poppler/poppler/Catalog.h					\
    poppler/poppler/CachedFile.h				\
    poppler/poppler/BuiltinFontTables.h				\
    poppler/poppler/BuiltinFont.h				\
    poppler/poppler/Array.h					\
    poppler/poppler/Annot.h					\
    libdjvu/ZPCodec.h						\
    libdjvu/XMLTags.h						\
    libdjvu/XMLParser.h						\
    libdjvu/UnicodeByteStream.h					\
    libdjvu/Template.h						\
    libdjvu/MMX.h						\
    libdjvu/MMRDecoder.h					\
    libdjvu/miniexp.h						\
    libdjvu/JPEGDecoder.h					\
    libdjvu/JB2Image.h						\
    libdjvu/IW44Image.h						\
    libdjvu/IFFByteStream.h					\
    libdjvu/GURL.h						\
    libdjvu/GThreads.h						\
    libdjvu/GString.h						\
    libdjvu/GSmartPointer.h					\
    libdjvu/GScaler.h						\
    libdjvu/GRect.h						\
    libdjvu/GPixmap.h						\
    libdjvu/GOS.h						\
    libdjvu/GMapAreas.h						\
    libdjvu/GIFFManager.h					\
    libdjvu/GException.h					\
    libdjvu/GContainer.h					\
    libdjvu/GBitmap.h						\
    libdjvu/DjVuToPS.h						\
    libdjvu/DjVuText.h						\
    libdjvu/DjVuPort.h						\
    libdjvu/DjVuPalette.h					\
    libdjvu/DjVuNavDir.h					\
    libdjvu/DjVuMessageLite.h					\
    libdjvu/DjVuMessage.h					\
    libdjvu/DjVuInfo.h						\
    libdjvu/DjVuImage.h						\
    libdjvu/DjVuGlobal.h					\
    libdjvu/DjVuFileCache.h					\
    libdjvu/DjVuFile.h						\
    libdjvu/DjVuErrorList.h					\
    libdjvu/DjVuDumpHelper.h					\
    libdjvu/DjVuDocument.h					\
    libdjvu/DjVuDocEditor.h					\
    libdjvu/DjVuAnno.h						\
    libdjvu/DjVmNav.h						\
    libdjvu/DjVmDoc.h						\
    libdjvu/DjVmDir0.h						\
    libdjvu/DjVmDir.h						\
    libdjvu/debug.h						\
    libdjvu/ddjvuapi.h						\
    libdjvu/DataPool.h						\
    libdjvu/ByteStream.h					\
    libdjvu/BSByteStream.h					\
    libdjvu/atomic.h						\
    libdjvu/Arrays.h						\
    zlib/zutil.h						\
    zlib/zlib.h							\
    zlib/zconf.h						\
    zlib/trees.h						\
    zlib/inftrees.h						\
    zlib/inflate.h						\
    zlib/inffixed.h						\
    zlib/inffast.h						\
    zlib/gzguts.h						\
    zlib/deflate.h						\
    zlib/crc32.h						\
    libtiff/symbian/tif_config.h				\
    libtiff/symbian/tiffconf.h					\
    libtiff/uvcode.h						\
    libtiff/tiffvers.h						\
    libtiff/tiffiop.h						\
    libtiff/tiffio.hxx						\
    libtiff/tiffio.h						\
    libtiff/tiff.h						\
    libtiff/tif_predict.h					\
    libtiff/tif_fax3.h						\
    libtiff/tif_dir.h						\
    libtiff/t4.h

FORMS += \
    nsrtouchdialog.ui		\
    nsrpreferences.ui		\
    nsrmainwindow.ui		\
    nsraboutdialog.ui		\
    nsrdocumentmenu.ui		\
    nsrnewsdialog.ui

QMAKE_CXXFLAGS.GCCE -= -fvisibility-inlines-hidden
QMAKE_CXXFLAGS.GCCE += -fpermissive

LIBS += -lfbscli
LIBS += -lavkon
LIBS += -leikcore
LIBS += -lRemConCoreApi
LIBS += -lRemConInterfaceBase
LIBS += -lefsrv
LIBS += -leuser
LIBS += -leiksrv
LIBS += -ltouchfeedback
LIBS += -lcone
LIBS += -leikcdlg
LIBS += -laknnotify
LIBS += -leikcoctl
LIBS += -leikctl
LIBS += -lform
LIBS += -luiklaf
LIBS += -leikdlg
LIBS += -lapparc
LIBS += -lapgrfx
LIBS += -lws32
LIBS += -lbafl
LIBS += -lglxcommoncommandhandlers

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc
