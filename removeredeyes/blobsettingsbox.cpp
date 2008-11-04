/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-11-01
 * Description : a widget that holds all blob extraction settings
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

#include "blobsettingsbox.h"
#include "blobsettingsbox.moc"

// Qt includes.

#include <QGridLayout>

// KDE includes.

#include <klocale.h>
#include <knuminput.h>

namespace KIPIRemoveRedEyesPlugin
{

class BlobSettingsBoxPriv
{
public:

    BlobSettingsBoxPriv()
    {
        minBlobSizeNumInput         = 0;
        minRoundnessNumInput        = 0;
    }

    KIntNumInput*       minBlobSizeNumInput;
    KDoubleNumInput*    minRoundnessNumInput;
};

BlobSettingsBox::BlobSettingsBox(QWidget* parent)
               : QGroupBox(parent),
                 d(new BlobSettingsBoxPriv)
{
    setTitle(i18n("Blob Extraction Settings"));

    d->minBlobSizeNumInput = new KIntNumInput;
    d->minBlobSizeNumInput->setLabel(i18n("Minimum blob size:"));
    d->minBlobSizeNumInput->setRange(1, 100, 1);
    d->minBlobSizeNumInput->setSliderEnabled(true);

    d->minRoundnessNumInput = new KDoubleNumInput;
    d->minRoundnessNumInput->setLabel(i18n("Minimum roundness:"));
    d->minRoundnessNumInput->setRange(1.0, 5.0, 0.01);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(d->minBlobSizeNumInput,  0, 0, 1,-1);
    mainLayout->addWidget(d->minRoundnessNumInput, 1, 0, 1,-1);
    mainLayout->setRowStretch(2, 10);
    setLayout(mainLayout);
}

BlobSettingsBox::~BlobSettingsBox()
{
    delete d;
}

int BlobSettingsBox::minBlobSize() const
{
    return d->minBlobSizeNumInput->value();
}

void BlobSettingsBox::setMinBlobSize(int value)
{
    d->minBlobSizeNumInput->setValue(value);
}

double BlobSettingsBox::minRoundness() const
{
    return d->minRoundnessNumInput->value();
}

void BlobSettingsBox::setMinRoundness(double value)
{
    d->minRoundnessNumInput->setValue(value);
}

} // namespace KIPIRemoveRedEyesPlugin