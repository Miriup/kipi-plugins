/* ============================================================
 * File  : calwizard.cpp
 * Authors: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 *          Tom Albers <tomalbers@kde.nl>
 *          Orgad Shaneh <orgads@gmail.com>
 * Date  : 2008-11-13
 * Description: main dialog
 *
 * Copyright 2003 by Renchi Raju
 * Copyright 2006 by Tom Albers
 * Copyright 2008 by Orgad Shaneh
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

// Qt includes.

#include <QDate>
#include <QPrinter>
#include <QStringList>
#include <QPrintDialog>

// KDE includes.

#include <KMenu>
#include <KDebug>
#include <KGlobal>
#include <KLocale>
#include <KHelpMenu>
#include <KToolInvocation>
#include <KCalendarSystem>

// LibKIPI includes.

#include <kpaboutdata.h>
#include <libkipi/interface.h>

// Local includes.

#include "caltemplate.h"
#include "calselect.h"
#include "calsettings.h"
#include "calpainter.h"
#include "calwizard.h"
#include "calformatter.h"

namespace KIPICalendarPlugin
{

CalWizard::CalWizard( KIPI::Interface* interface, QWidget *parent )
         : KAssistantDialog(parent),
           interface_( interface )
{
    setMaximumSize( 800, 600 );
    cSettings_ = CalSettings::instance();

    // ---------------------------------------------------------------

    wTemplate_ = new CalTemplate( this );
    addPage(wTemplate_, i18n( "Create Template for Calendar" ));

    // ---------------------------------------------------------------

    wEvents_ = new QWidget( this );
    calEventsUI.setupUi( wEvents_ );
    addPage(wEvents_, i18n( "Choose events to show on the Calendar" ));

    // ---------------------------------------------------------------

    wSelect_ = new CalSelect( interface, this );
    addPage(wSelect_, i18n( "Select Year & Images" ));

    // ---------------------------------------------------------------

    wPrintLabel_ = new QLabel( this );
    wPrintLabel_->setIndent( 20 );
    wPrintLabel_->setWordWrap( true );

    wPrintPage_ = addPage( wPrintLabel_, i18n( "Print" ) );

    // ---------------------------------------------------------------

    wFinish_ = new QWidget( this );
    calProgressUI.setupUi( wFinish_ );
    wFinishPage_ = addPage(wFinish_, i18n( "Printing" ));

    // ---------------------------------------------------------------

    // About data and help button.

    m_about = new KIPIPlugins::KPAboutData(ki18n("Calendar"),
                                           0,
                                           KAboutData::License_GPL,
                                           ki18n("A Kipi plugin to create a calendar"),
                                           ki18n("(c) 2003-2004, Renchi Raju, (c) 2006 Tom Albers, (c) 2008 Orgad Shaneh"));

    m_about->addAuthor(ki18n("Orgad Shaneh"), ki18n("Author and maintainer"),
                       "orgads@gmail.com");

    m_about->addAuthor(ki18n("Tom Albers"), ki18n("Former author and maintainer"),
                       "tomalbers@kde.nl");

    m_about->addAuthor(ki18n("Renchi Raju"), ki18n("Former author and maintainer"),
                       "renchi@pooh.tam.uiuc.edu");

    disconnect(this, SIGNAL(helpClicked()),
               this, SLOT(slotHelp()));

    KPushButton *helpButton = button( Help );
    KHelpMenu* helpMenu = new KHelpMenu(this, m_about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Plugin Handbook"), this);
    connect(handbook, SIGNAL(triggered()),
            this, SLOT(slotHelp()));
    connect(this, SIGNAL(helpClicked()),
               this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    helpButton->setDelayedMenu( helpMenu->menu() );

    // ------------------------------------------

    printer_  = 0;
    formatter_ = 0;

    connect(this, SIGNAL(currentPageChanged(KPageWidgetItem *, KPageWidgetItem *)),
            SLOT(slotPageSelected(KPageWidgetItem *, KPageWidgetItem *)));

    setCaption(i18n("Create Calendar"));
}

CalWizard::~CalWizard()
{
    if (painter_) delete painter_;

    if (printer_) delete printer_;

    delete m_about;

    if ( formatter_ ) delete formatter_;
}

void CalWizard::slotHelp()
{
    KToolInvocation::invokeHelp("calendar", "kipi-plugins");
}

void CalWizard::slotPageSelected(KPageWidgetItem *current, KPageWidgetItem *before)
{
    Q_UNUSED(before);

    if (current == wPrintPage_) {

        months_.clear();
        KUrl image;
        QString month;
        QStringList printList;
        QDate d;
        KGlobal::locale()->calendar()->setYMD(d, cSettings_->year(), 1, 1);
        for (int i=1; i<=KGlobal::locale()->calendar()->monthsInYear(d); i++) {
            month = KGlobal::locale()->calendar()->monthName(i, cSettings_->year(), KCalendarSystem::LongName);
            image = cSettings_->image(i);
            if (!image.isEmpty()) {
                months_.insert(i, image);
                printList.append(month);
            }
        }

        if (months_.empty()) {
            wPrintLabel_->setText(i18n("No valid images selected for months<br>"
                    "Click Back to select images"));
            setValid(wFinishPage_, false);
        }
        else {
            QString year = QString::number(cSettings_->year());

            QString extra;
            if ((KGlobal::locale()->calendar()->month(QDate::currentDate()) >= 6 &&
                 KGlobal::locale()->calendar()->year(QDate::currentDate()) == cSettings_->year()) ||
                 KGlobal::locale()->calendar()->year(QDate::currentDate()) > cSettings_->year())
                extra = "<br><br><b>"+i18n("Please note that you are making a "
                        "calendar for<br>the current year or a year in the "
                        "past.")+"</b>";

            wPrintLabel_->setText(i18n("Click Next to start Printing<br><br>"
                    "Following months will be printed for year %1:<br>").arg(year)
                    + printList.join(" - ") + extra);
            wPrintLabel_->setTextFormat(Qt::RichText);

            setValid(wFinishPage_, true);
        }
    }

    else if (current == wFinishPage_) {

        calProgressUI.finishLabel->clear();
        calProgressUI.currentProgress->reset();
        calProgressUI.totalProgress->reset();

        enableButton(KDialog::User3, false); // disable 'Back' button
        enableButton(KDialog::User1, false); // disable 'Finish' button

        // Set printer settings ---------------------------------------

        if (!printer_)
            printer_ = new QPrinter( QPrinter::HighResolution );

        // TODO: Let user choose resolutions

        CalParams& params = cSettings_->params;

        // Orientation
        switch (params.imgPos) {
        case(CalParams::Top): {
            printer_->setOrientation(QPrinter::Portrait);
            break;
        }
        default:
            printer_->setOrientation(QPrinter::Landscape);
        }

        kDebug(51000) << "printing...";
        // PageSize
        printer_->setPageSize(params.pageSize);
        QPrintDialog printDialog( printer_, this );

        if ( printDialog.exec() == QDialog::Accepted ) {
            print();
        }
        else {
            calProgressUI.finishLabel->setText(i18n( "Printing Cancelled" ));
            enableButton(KDialog::User3, false); // enable 'Back' button
        }

    }
}

void CalWizard::print()
{
    calProgressUI.totalProgress->setMaximum( months_.count() );
    calProgressUI.totalProgress->setValue( 0 );

    if ( formatter_ )
        delete formatter_;
    formatter_ = new CalFormatter( cSettings_->year(),
                                   calEventsUI.ohUrlRequester->url(),
                                   calEventsUI.fhUrlRequester->url(),
                                   this );

    if (painter_) delete painter_;
    painter_ = new CalPainter( printer_, formatter_ );

    calProgressUI.totalProgress->setMaximum(months_.count());
    calProgressUI.totalProgress->setValue( 0 );

    currPage_ = 0;

    connect(painter_, SIGNAL(signalTotal(int)),
            calProgressUI.currentProgress, SLOT(setMaximum(int)));
    connect(painter_, SIGNAL(signalProgress(int)),
            calProgressUI.currentProgress, SLOT(setValue(int)));

    connect(painter_, SIGNAL(signalFinished()), SLOT(paintNextPage()));
    paintNextPage();
}

void CalWizard::paintNextPage()
{
    const int year = cSettings_->year();

    calProgressUI.totalProgress->setValue( currPage_ );

    if (currPage_ >= months_.count())
    {
        printComplete();
        return;
    }
    int month = months_.keys().at(currPage_);

    calProgressUI.finishLabel->setText(i18n( "Printing Calendar Page for %1 of %2" )
            .arg(KGlobal::locale()->calendar()->monthName(month, year, KCalendarSystem::LongName))
            .arg(year));

    if (currPage_)
        printer_->newPage();
    currPage_++;

    int angle = interface_->info( months_.value(month) ).angle();

    painter_->setYearMonth(year, month);
    painter_->setImage(months_.value(month), angle);

    painter_->paint( false );
}

void CalWizard::printComplete()
{
    enableButton(KDialog::User3, true); // enable 'Back' button
    enableButton(KDialog::User1, true); // enable 'Finish' button
    calProgressUI.finishLabel->setText(i18n( "Printing Complete" ));
}

}  // nameSpace KIPICalendarPlugin