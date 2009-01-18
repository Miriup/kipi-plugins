/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : Jan 18, 2009
 * Description : XXXXXXXXXXXXXXXXXXXXXXXx
 *
 * Copyright (C) 2009 by andi <xxxxxxxxxxxxx>
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

#ifndef SAVEMETHODABSTRACT_H
#define SAVEMETHODABSTRACT_H


// Qt includes.

#include <QString>

namespace KIPIRemoveRedEyesPlugin
{

class SaveMethodAbstract
{
public:

    SaveMethodAbstract() {};
    virtual ~SaveMethodAbstract() {};

    virtual QString savePath(const QString& path) = 0;
};

}

#endif /* SAVEMETHODABSTRACT_H */