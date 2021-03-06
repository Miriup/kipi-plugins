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

#include "gpstracklistwidget.h"
#include "gpstracklistwidget.moc"

// KDE includes

#include <kdebug.h>
#include <khtmlview.h>
#include <kurl.h>

// local includes

#include "pluginsversion.h"

namespace KIPIGPSSyncPlugin
{

class GPSTrackListWidgetPrivate
{

public:

    GPSTrackListWidgetPrivate()
    {
        gpsTrackListUrl = QString("http://digikam3rdparty.free.fr/gpslocator/tracklistedit.php");
    }

    QString      gpsTrackListUrl;
    QString      zoomLevel;
    QString      mapType;
    QString      extraOptions;

    GPSTrackList gpsTrackList;
};

GPSTrackListWidget::GPSTrackListWidget(QWidget* parent)
                  : KHTMLPart(parent), d(new GPSTrackListWidgetPrivate)
{
    setJScriptEnabled(true);
    setDNDEnabled(false);
    setEditable(false);

    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view()->setMinimumSize(480, 360);
    view()->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
}

GPSTrackListWidget::~GPSTrackListWidget()
{
    delete d;
}

void GPSTrackListWidget::setExtraOptions(const QString& extraOptions)
{
    d->extraOptions = extraOptions;
}

void GPSTrackListWidget::setTrackList(const GPSTrackList& gpsTrackList)
{
    d->gpsTrackList = gpsTrackList;
}

void GPSTrackListWidget::setMapType(const QString& mapType)
{
    d->mapType = mapType;
}

QString GPSTrackListWidget::mapType() const
{
    return d->mapType;
}

void GPSTrackListWidget::setZoomLevel(int zoomLevel)
{
    d->zoomLevel = QString::number(zoomLevel);
}

int GPSTrackListWidget::zoomLevel() const
{
    return d->zoomLevel.toInt();
}

void GPSTrackListWidget::extractGPSPositionfromStatusbar(const QString& txt)
{
    QString status = txt;
    status.remove(0, 5);
    status.truncate(status.length()-1);
    QString idTxt  = status.section(',', 0, 0);
    QString latTxt = status.section(',', 1, 1);
    QString lngTxt = status.section(',', 2, 2);
    QString altTxt = status.section(',', 3, 3);
    int id         = idTxt.toInt();
    if (latTxt.isEmpty() || lngTxt.isEmpty())
    {
        // Special case if only marker have been selected but not moved.
        emit signalMarkerSelectedFromMap(id);
    }
    else
    {
        latTxt.remove(0, 5);
        lngTxt.remove(0, 5);
        altTxt.remove(0, 5);
        double lat = latTxt.toDouble();
        double lng = lngTxt.toDouble();
        double alt = altTxt.toDouble();
        emit signalNewGPSLocationFromMap(id, lat, lng, alt);
        kDebug() << id << "::" << lat << "::" << lng << "::" << alt ;
    }
}

void GPSTrackListWidget::khtmlMouseMoveEvent(khtml::MouseMoveEvent *e)
{
    QString status = jsStatusBarText();

    // If a new point to the map is dragged around the map, the status bar
    // string is like : "(mkr:1, lat:25.5894748, lon:47.6897455478)"
    if (status.startsWith(QString("(mkr:")))
        extractGPSPositionfromStatusbar(status);

    KHTMLPart::khtmlMouseMoveEvent(e);
}

void GPSTrackListWidget::khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *e)
{
    QString status = jsStatusBarText();

    // If a new point to the map have been moved around the map, the Status
    // string is like : "(mkr:1, lat:25.5894748, lon:47.6897455478, alt:211)"
    if (status.startsWith(QString("(mkr:")))
        extractGPSPositionfromStatusbar(status);

    // If a new map zoom level have been selected, the Status
    // string is like : "newZoomLevel:5"
    if (status.startsWith(QString("newZoomLevel:")))
    {
        status.remove(0, 13);
        d->zoomLevel = status;
    }

    // If a new map type have been selected, the Status
    // string is like : "newMapType:G_SATELLITE_MAP"
    if (status.startsWith(QString("newMapType:")))
    {
        status.remove(0, 11);
        d->mapType = status;
    }

    KHTMLPart::khtmlMouseReleaseEvent(e);
}

void GPSTrackListWidget::slotResized()
{
    KUrl url = d->gpsTrackListUrl;
    url.addQueryItem("width", QString::number(view()->width()));
    url.addQueryItem("height", QString::number(view()->height()));
    url.addQueryItem("zoom", d->zoomLevel);
    url.addQueryItem("maptype", d->mapType);
    url.addQueryItem("pluginversion", QString(kipiplugins_version));
    url.addQueryItem("extraoptions", d->extraOptions);
    url.addQueryItem("maplang",
                    i18nc(
                        "Language code for the embedded Google Maps. "
                        "Please take a look at "
                        "http://spreadsheets.google.com/pub?key=p9pdwsai2hDMsLkXsoM05KQ&gid=1 "
                        "for supported languages. If your language is not on the list, pick 'en'."
                        , "en"));

    int count = d->gpsTrackList.count();
    url.addQueryItem("items", QString::number(count));
    for( GPSTrackList::iterator it = d->gpsTrackList.begin() ;
         it != d->gpsTrackList.end() ; ++it)
    {
        int id = it->id();
        url.addQueryItem(QString("&lat%1").arg(id), QString::number(it->gpsData().latitude()));
        url.addQueryItem(QString("&lng%1").arg(id), QString::number(it->gpsData().longitude()));
        url.addQueryItem(QString("&alt%1").arg(id), QString::number(it->gpsData().altitude()));
        url.addQueryItem(QString("&title%1").arg(id), QString("%1").arg(id));
    }

    openUrl(url);
    kDebug(AREA_CODE_LOADING) << url;
}

}  // namespace KIPIGPSSyncPlugin
