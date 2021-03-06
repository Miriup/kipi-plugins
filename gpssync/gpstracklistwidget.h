/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-02-08
 * Description : a widget to display a GPS track list over web map locator.
 *
 * Copyright (C) 2008-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef GPSTRACKLISTWIDGET_H
#define GPSTRACKLISTWIDGET_H

// Qt includes

#include <QString>

// KDE includes

#include <khtml_part.h>

// Local includes

#include "gpstracklistcontainer.h"

namespace KIPIGPSSyncPlugin
{

class GPSTrackListWidgetPrivate;

class GPSTrackListWidget : public KHTMLPart
{
    Q_OBJECT

public:

    GPSTrackListWidget(QWidget* parent);
    ~GPSTrackListWidget();

    void    setExtraOptions(const QString& extraOptions);
    void    setTrackList(const GPSTrackList& trackList);

    void    setZoomLevel(int zoomLevel);
    int     zoomLevel() const;

    void    setMapType(const QString& mapType);
    QString mapType() const;

public Q_SLOTS:

    void slotResized();

Q_SIGNALS:

    void signalNewGPSLocationFromMap(int, double, double, double);
    void signalMarkerSelectedFromMap(int);

protected:

    void khtmlMouseMoveEvent(khtml::MouseMoveEvent*);
    void khtmlMouseReleaseEvent(khtml::MouseReleaseEvent*);

private:

    void extractGPSPositionfromStatusbar(const QString& txt);

private:

    GPSTrackListWidgetPrivate* const d;
};

}  // namespace KIPIGPSSyncPlugin

#endif /* GPSTRACKLISTWIDGET_H */
