/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-12-01
 * Description : a kipi plugin to export images to SmugMug web service
 *
 * Copyright (C) 2008 by Luka Renko <lure at kubuntu dot org>
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

#ifndef SMUGITEM_H
#define SMUGITEM_H

// Qt includes.
#include <QString>

namespace KIPISmugExportPlugin
{

class SmugAlbum
{
public:
    SmugAlbum()
    {
        id = -1;
        categoryID = -1;
        subCategoryID = -1;
        isPublic = true;
    }

    int     id;

    QString key;
    QString title;

    int     categoryID;
    QString category;

    int     subCategoryID;
    QString subCategory;

    // below fields only used by createAlbum (and not by listAlbums)
    QString description;
    bool    isPublic;
    QString password;
    QString passwordHint;
};

class SmugCategory
{
public:
    SmugCategory()
    {
        id = -1;
    }

    int     id;
    QString name;
};

} // namespace KIPISmugExportPlugin

#endif /* SMUGITEM_H */