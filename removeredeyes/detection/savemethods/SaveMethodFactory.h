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

#ifndef SAVEMETHODFACTORY_H
#define SAVEMETHODFACTORY_H

namespace KIPIRemoveRedEyesPlugin
{

class SaveMethodAbstract;

class SaveMethodFactory
{

public:

    static SaveMethodAbstract* factory(int type);

private:

    SaveMethodFactory() {};
    virtual ~SaveMethodFactory() {};

};

}

#endif /* SAVEMETHODFACTORY_H */