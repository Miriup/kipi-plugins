// vim: set tabstop=4 shiftwidth=4 noexpandtab:
/*
A KIPI plugin to generate HTML image galleries
Copyright 2008 Aurelien Gateau <aurelien dot gateau at free.fr>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/
#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H

// Qt
#include <QSize>
#include <QString>

// KIPI
#include <libkipi/imageinfo.h>

// Local
#include "xmlutils.h"

namespace KExiv2Iface {
class KExiv2;
}

namespace KIPIHTMLExport {

/**
 * This structure stores all the necessary information to produce an XML
 * description of an image
 */
class ImageElement {
public:
	ImageElement(const KIPI::ImageInfo& info)
	: mValid(false)
	, mTitle(info.title())
	, mDescription(info.description())
	, mAngle(info.angle())
	, mTime(info.time())
	{}

	ImageElement() : mValid(false) {}

	bool mValid;
	QString mTitle;
	QString mDescription;
	int mAngle;
	QDateTime mTime;

	QString mPath;

	QString mThumbnailFileName;
	QSize mThumbnailSize;
	QString mFullFileName;
	QSize mFullSize;
	QString mOriginalFileName;
	QSize mOriginalSize;

        // Exif Metadata
        QString mExifImageMake;
        QString mExifImageModel;
        QString mExifImageOrientation;
        QString mExifImageXResolution;
        QString mExifImageYResolution;
        QString mExifImageResolutionUnit;
        QString mExifImageDateTime;
        QString mExifImageYCbCrPositioning;
        QString mExifPhotoExposureTime;
        QString mExifPhotoFNumber;
        QString mExifPhotoExposureProgram;
        QString mExifPhotoISOSpeedRatings;
        QString mExifPhotoShutterSpeedValue;
        QString mExifPhotoApertureValue;
        QString mExifPhotoFocalLength;

	void appendToXML(XMLWriter& xmlWriter, bool copyOriginalImage) const {
		if (!mValid) {
			return;
		}
		XMLElement imageX(xmlWriter, "image");
		xmlWriter.writeElement("title", mTitle);
		xmlWriter.writeElement("description", mDescription);
		xmlWriter.writeElement("date", mTime.toString("yyyy-MM-ddThh:mm:ss"));
                appendImageElementToXML(xmlWriter, "full", mFullFileName, mFullSize);
                appendImageElementToXML(xmlWriter, "thumbnail", mThumbnailFileName, mThumbnailSize);
                if (copyOriginalImage) {
                        appendImageElementToXML(xmlWriter, "original", mOriginalFileName, mOriginalSize);
                }
                //Exif
                // TODO put all exif tags in a sub level
                XMLElement imageExif(xmlWriter, "exif");
                xmlWriter.writeElement("exifimagemake", mExifImageMake);
                xmlWriter.writeElement("exifimagemodel", mExifImageModel);
                xmlWriter.writeElement("exifimageorientation", mExifImageOrientation);
                xmlWriter.writeElement("exifimagexresolution", mExifImageXResolution);
                xmlWriter.writeElement("exifimageyresolution", mExifImageYResolution);
                xmlWriter.writeElement("exifimageresolutionunit", mExifImageResolutionUnit);
                xmlWriter.writeElement("exifimagedatetime", mExifImageDateTime);
                xmlWriter.writeElement("exifimageycbcrpositioning", mExifImageYCbCrPositioning);
                xmlWriter.writeElement("exifphotoexposuretime", mExifPhotoExposureTime);
                xmlWriter.writeElement("exifphotofnumber", mExifPhotoFNumber);
                xmlWriter.writeElement("exifphotoexposureprogram", mExifPhotoExposureProgram);
                xmlWriter.writeElement("exifphotoisospeedratings", mExifPhotoISOSpeedRatings);
                xmlWriter.writeElement("exifphotoshutterspeedvalue", mExifPhotoShutterSpeedValue);
                xmlWriter.writeElement("exifphotoaperturevalue", mExifPhotoApertureValue);
                xmlWriter.writeElement("exifphotofocallength", mExifPhotoFocalLength);


	}

	void appendImageElementToXML(XMLWriter& xmlWriter, const QString& elementName, const QString& fileName, const QSize& size) const {
		XMLAttributeList attrList;
		attrList.append("fileName", fileName);
		attrList.append("width", size.width());
		attrList.append("height", size.height());
		XMLElement elem(xmlWriter, elementName, &attrList);
	}
};

} // namespace

#endif /* IMAGEELEMENT_H */
