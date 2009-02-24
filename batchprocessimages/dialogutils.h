/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Copyright (C) 2009 by Aurélien Gâteau <agateau@kde.org>
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
#ifndef DIALOGUTILS_H
#define DIALOGUTILS_H

class KAboutData;
class KDialog;
namespace DialogUtils {

void setupHelpButton(KDialog*, KAboutData*);

} // namespace

#endif /* DIALOGUTILS_H */