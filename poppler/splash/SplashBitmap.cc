//========================================================================
//
// SplashBitmap.cc
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2006, 2009, 2010 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2007 Ilmari Heikkinen <ilmari.heikkinen@gmail.com>
// Copyright (C) 2009 Shen Liang <shenzhuxi@gmail.com>
// Copyright (C) 2009 Stefan Thomas <thomas@eload24.com>
// Copyright (C) 2010 Adrian Johnson <ajohnson@redneon.com>
// Copyright (C) 2010 Harry Roberts <harry.roberts@midnight-labs.org>
// Copyright (C) 2010 Christian Feuersänger <cfeuersaenger@googlemail.com>
// Copyright (C) 2010 William Bader <williambader@hotmail.com>
// Copyright (C) 2011 Thomas Freitag <Thomas.Freitag@alfa.de>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "goo/gmem.h"
#include "SplashErrorCodes.h"
#include "SplashBitmap.h"
#include "poppler/Error.h"

//------------------------------------------------------------------------
// SplashBitmap
//------------------------------------------------------------------------

SplashBitmap::SplashBitmap(int widthA, int heightA, int rowPadA,
			   SplashColorMode modeA, GBool alphaA,
			   GBool topDown) {
  width = widthA;
  height = heightA;
  mode = modeA;
  rowPad = rowPadA;
  switch (mode) {
  case splashModeMono1:
    if (width > 0) {
      rowSize = (width + 7) >> 3;
    } else {
      rowSize = -1;
    }
    break;
  case splashModeMono8:
    if (width > 0) {
      rowSize = width;
    } else {
      rowSize = -1;
    }
    break;
  case splashModeRGB8:
  case splashModeBGR8:
    if (width > 0 && width <= INT_MAX / 3) {
      rowSize = width * 3;
    } else {
      rowSize = -1;
    }
    break;
  case splashModeXBGR8:
    if (width > 0 && width <= INT_MAX / 4) {
      rowSize = width * 4;
    } else {
      rowSize = -1;
    }
    break;
#if SPLASH_CMYK
  case splashModeCMYK8:
    if (width > 0 && width <= INT_MAX / 4) {
      rowSize = width * 4;
    } else {
      rowSize = -1;
    }
    break;
#endif
  }
  if (rowSize > 0) {
    rowSize += rowPad - 1;
    rowSize -= rowSize % rowPad;
  }
  data = (SplashColorPtr)gmallocn(rowSize, height);
  if (!topDown) {
    data += (height - 1) * rowSize;
    rowSize = -rowSize;
  }
  if (alphaA) {
    alpha = (Guchar *)gmallocn(width, height);
  } else {
    alpha = NULL;
  }
}


SplashBitmap::~SplashBitmap() {
  if (rowSize < 0) {
    gfree(data + (height - 1) * rowSize);
  } else {
    gfree(data);
  }
  gfree(alpha);
}

void SplashBitmap::getPixel(int x, int y, SplashColorPtr pixel) {
  SplashColorPtr p;

  if (y < 0 || y >= height || x < 0 || x >= width) {
    return;
  }
  switch (mode) {
  case splashModeMono1:
    p = &data[y * rowSize + (x >> 3)];
    pixel[0] = (p[0] & (0x80 >> (x & 7))) ? 0xff : 0x00;
    break;
  case splashModeMono8:
    p = &data[y * rowSize + x];
    pixel[0] = p[0];
    break;
  case splashModeRGB8:
    p = &data[y * rowSize + 3 * x];
    pixel[0] = p[0];
    pixel[1] = p[1];
    pixel[2] = p[2];
    break;
  case splashModeXBGR8:
    p = &data[y * rowSize + 4 * x];
    pixel[0] = p[2];
    pixel[1] = p[1];
    pixel[2] = p[0];
    pixel[3] = p[3];
    break;
  case splashModeBGR8:
    p = &data[y * rowSize + 3 * x];
    pixel[0] = p[2];
    pixel[1] = p[1];
    pixel[2] = p[0];
    break;
#if SPLASH_CMYK
  case splashModeCMYK8:
    p = &data[y * rowSize + 4 * x];
    pixel[0] = p[0];
    pixel[1] = p[1];
    pixel[2] = p[2];
    pixel[3] = p[3];
    break;
#endif
  }
}

Guchar SplashBitmap::getAlpha(int x, int y) {
  return alpha[y * width + x];
}
