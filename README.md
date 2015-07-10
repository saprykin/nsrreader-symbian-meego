## What is NSR Reader?

NSR Reader is a powerful reader for PDF, DjVu, TIFF (S^3, Anna, Belle) and TXT files. This repository contains source
code for Symbian S60v5, S3, Anna and Belle, and also for MeeGo and Maemo platforms. Though the Maemo port is compilable,
it wasn't tested on a real hardware.

## Features
<ul>
<li>Supports PDF, DjVu, TIFF and plain TXT files</li>
<li>Saves last opened document and its position</li>
<li>Text reflow mode</li>
<li>Landscape and portrait orientations handling</li>
<li>Supports password-protected PDFs</li>
<li>Pinch-to-zoom gestures</li>
<li>Fullscreen and rotation</li>
<li>Kinetic scroll and UI animation</li>
<li>Can be used as default reader</li>
<li>Fits to screen width</li>
</ul>

## Constraints
Not all platforms are consistent in their APIs and features, so depending on running platfom some
features in the app may be unavailable or limited.

<b>Symbian</b>

Due to limited memory capabilities of the devices with Symbian, TIFF is supported only from S^3 and above.
You must use unsigned version of the app if you want it to be a default reader.

<b>MeeGo/Maemo</b>

There is no support for orientation handling because of X11 implementation, so the app is capable to
work only in landscape mode.

## Building
To build NSR Reader you should use the latest Qt SDK 1.2.1 with Symbian/MeeGo/Maemo support. It is not available
on Nokia or Microsoft website, so you need to search through the Internet for a copy.

To build MIME-type recognizers for Symbian you need to use native Symbian SDKs and Carbide IDE provided by Nokia.
Try to search the Internet for them.

## License
Software is distributed under the terms of GNU GPLv2 license, but you can use GNU GPLv3 if you want it. Please
note that source code contains several third-party libraries with their own license terms.
