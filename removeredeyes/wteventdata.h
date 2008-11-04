/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-06-08
 * Description : a kipi plugin to automatically detect
 *               and remove red eyes from images
 *
 * Copyright 2008 by Andi Clemens <andi dot clemens at gmx dot net>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef WTEVENTDATA_H
#define WTEVENTDATA_H

class KUrl;

namespace KIPIRemoveRedEyesPlugin
{

class WTEventData
{

public:

    WTEventData(int current, const KUrl& url, int eyes = 0);
    ~WTEventData();

    int         current() const;
    int         eyes() const;
    const KUrl& url() const;

private:

    int         m_current;
    int         m_eyes;
    const KUrl& m_url;
};

} // namespace KIPIRemoveRedEyesPlugin

#endif