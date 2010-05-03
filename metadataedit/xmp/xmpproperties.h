/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2007-10-24
 * Description : XMP workflow status properties settings page.
 *
 * Copyright (C) 2007-2010 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef XMP_PROPERTIES_H
#define XMP_PROPERTIES_H

// Qt includes

#include <QWidget>
#include <QByteArray>

namespace KIPIMetadataEditPlugin
{

class XMPPropertiesPriv;

class XMPProperties : public QWidget
{
    Q_OBJECT

public:

    XMPProperties(QWidget* parent);
    ~XMPProperties();

    void applyMetadata(QByteArray& xmpData);
    void readMetadata(QByteArray& xmpData);

Q_SIGNALS:

    void signalModified();

private:

    XMPPropertiesPriv* const d;
};

}  // namespace KIPIMetadataEditPlugin

#endif // XMP_PROPERTIES_H 