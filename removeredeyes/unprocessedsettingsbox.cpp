/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-11-28
 * Description : a widget that holds unprocessed image settings
 *
 * Copyright (C) 2008 by Andi Clemens <andi dot clemens at gmx dot net>
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

#include "unprocessedsettingsbox.h"
#include "unprocessedsettingsbox.moc"

// Qt includes.

#include <QButtonGroup>
#include <QGridLayout>
#include <QRadioButton>

// KDE includes.

#include <klocale.h>

// Local includes.

#include "removeredeyeswindow.h"

namespace KIPIRemoveRedEyesPlugin
{

class UnprocessedSettingsBoxPriv
{
public:

    UnprocessedSettingsBoxPriv()
    {
        unprocessedGroup = 0;
    }

    QButtonGroup*       unprocessedGroup;
};

UnprocessedSettingsBox::UnprocessedSettingsBox(QWidget* parent)
                  : QGroupBox(parent),
                    d(new UnprocessedSettingsBoxPriv)
{
    setTitle(i18n("Unprocessed Images Settings"));
    d->unprocessedGroup = new QButtonGroup(this);
    d->unprocessedGroup->setExclusive(true);

    QRadioButton* askMode = new QRadioButton(i18n("Ask the user"));
    askMode->setToolTip(i18n("If checked, the user will be asked to keep or "
                             "remove unprocessed images."));
    d->unprocessedGroup->addButton(askMode, Ask);

    QRadioButton* keepMode = new QRadioButton(i18n("Keep"));
    keepMode->setToolTip(i18n("If checked, unprocessed images will be kept after test-run."));
    d->unprocessedGroup->addButton(keepMode, Keep);

    QRadioButton* removeMode = new QRadioButton(i18n("Remove"));
    removeMode->setToolTip(i18n("If checked, unprocessed images will be removed from "
                                "the images list after test-run."));
    d->unprocessedGroup->addButton(removeMode, Remove);

    QGridLayout* unprocessedGroupLayout = new QGridLayout;
    unprocessedGroupLayout->addWidget(askMode,       0, 0, 1, 1);
    unprocessedGroupLayout->addWidget(keepMode,      1, 0, 1, 1);
    unprocessedGroupLayout->addWidget(removeMode,    2, 0, 1, 1);
    setLayout(unprocessedGroupLayout);

    setHandleMode(Ask);

    connect(d->unprocessedGroup, SIGNAL(buttonClicked(int)),
            this, SIGNAL(settingsChanged()));
}

UnprocessedSettingsBox::~UnprocessedSettingsBox()
{
    delete d;
}

int UnprocessedSettingsBox::handleMode() const
{
    return d->unprocessedGroup->checkedId();
}

void UnprocessedSettingsBox::setHandleMode(int mode)
{
    d->unprocessedGroup->button(mode)->setChecked(true);
}

} // namespace KIPIRemoveRedEyesPlugin