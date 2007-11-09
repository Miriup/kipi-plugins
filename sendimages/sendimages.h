/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-25
 * Description : a kipi plugin to e-mailing images
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef SENDIMAGES_H
#define SENDIMAGES_H

// Qt includes.

#include <qobject.h>

// Local includes.

#include "actions.h"
#include "emailsettingscontainer.h"

namespace KIPISendimagesPlugin
{
class SendImagesPriv;

class SendImages : public QObject
{

Q_OBJECT

public:

    SendImages(const EmailSettingsContainer& settings, QObject *parent=0);
    ~SendImages();

    void sendImages();
	
private slots:
   
    void slotStarting(const KUrl&, int);
    void slotFinished(const KUrl&, int);
    void slotFailed(const KUrl&, int, const QString&);
    void slotComplete(int action);
    void slotCancel();

private:

    SendImagesPriv *d;
};

}  // NameSpace KIPISendimagesPlugin

#endif  // SENDIMAGES_H
