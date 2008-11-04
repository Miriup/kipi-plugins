/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-11-01
 * Description : a widget that holds all classifier settings
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

#ifndef CLASSIFIERSETTINGSBOX_H
#define CLASSIFIERSETTINGSBOX_H

// Qt includes.

#include <QGroupBox>

class QWidget;

namespace KIPIRemoveRedEyesPlugin
{

class ClassifierSettingsBoxPriv;

class ClassifierSettingsBox : public QGroupBox
{
    Q_OBJECT

public:

    ClassifierSettingsBox(QWidget* parent = 0);
    virtual ~ClassifierSettingsBox();

    bool useStandardClassifier() const;
    void setUseStandardClassifier(bool);

    int neighborGroups() const;
    void setNeighborGroups(int);

    double scalingFactor() const;
    void setScalingFactor(double);

    QString classifierUrl() const;
    void setClassifierUrl(QString&);

public slots:

    void slotStandardClassifier();

private:

    ClassifierSettingsBoxPriv* const d;
};

}

#endif /* CLASSIFIERSETTINGSBOX_H */