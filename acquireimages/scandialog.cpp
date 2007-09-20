/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2007-09-09
 * Description : scanner dialog
 *
 * Copyright (C) 2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

// Qt includes.

#include <QDateTime>
#include <QString>
#include <QProgressBar>
#include <QGridLayout>
#include <QPushButton>

// KDE includes.

#include <ktoolinvocation.h>
#include <kapplication.h>
#include <kimageio.h>
#include <kurl.h>
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kpushbutton.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <khelpmenu.h>
#include <kmenu.h>

// LibKSane includes.

#include <libksane/sane_widget.h>

// LibKipi includes.

#include <libkipi/interface.h>

// Local includes.

#include "kpaboutdata.h"
#include "kpwriteimage.h"
#include "pluginsversion.h"
#include "scandialog.h"
#include "scandialog.moc"

namespace KIPIAcquireImagesPlugin
{

class ScanDialogPriv
{
public:

    ScanDialogPriv()
    {
        saneWidget = 0;
        progress   = 0;
        interface  = 0;
        cancel     = 0;
        about      = 0;
    }

    QProgressBar             *progress;

    QPushButton              *cancel;

    KIPI::Interface          *interface;

    KIPIPlugins::KPAboutData *about;

    KSaneIface::SaneWidget   *saneWidget;
};

ScanDialog::ScanDialog(KIPI::Interface* interface, QWidget *parent)
          : KDialog(parent)
{
    d = new ScanDialogPriv;
    d->interface = interface;

    setButtons(None);
    setCaption(i18n("Scan Image"));
    setModal(true);

    QWidget *main     = new QWidget(this);
    QGridLayout *grid = new QGridLayout(main);

    // -- About data and help button ----------------------------------------

    d->about = new KIPIPlugins::KPAboutData(ki18n("Acquire images"),
                   QByteArray(),
                   KAboutData::License_GPL,
                   ki18n("A Kipi plugin to acquire images using a flat scanner"),
                   ki18n("(c) 2003-2007, Gilles Caulier"));

    d->about->addAuthor(ki18n("Gilles Caulier"), 
                        ki18n("Author"),
                        "caulier dot gilles at gmail dot com");

    d->about->addAuthor(ki18n("Angelo Naselli"), 
                        ki18n("Developper"),
                        "anaselli at linux dot it");

    d->about->addAuthor(ki18n("Kare Sars"), 
                        ki18n("Developper"),
                        "kare dot sars at kolumbus dot fi");

    KPushButton *help   = new KPushButton(KStandardGuiItem::help(), main);
    KHelpMenu* helpMenu = new KHelpMenu(this, d->about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Plugin Handbook"), this);
    connect(handbook, SIGNAL(triggered(bool)),
            this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    help->setDelayedMenu( helpMenu->menu() );

    // ------------------------------------------------------------------------

    d->saneWidget = new KSaneIface::SaneWidget(main);
    QString dev   = d->saneWidget->selectDevice(0);
    if (!d->saneWidget->openDevice(dev)) 
    {
        // could not open a scanner
        KMessageBox::sorry(0, i18n("No scanner device has been found."));
        slotClose();
    }

    KPushButton *close = new KPushButton(KStandardGuiItem::close(), main);
    d->progress        = new QProgressBar(main);
    d->progress->setMaximumHeight(fontMetrics().height()+4);
    d->progress->hide();
    d->cancel          = new QPushButton(main);
    d->cancel->setIcon(SmallIcon("dialog-cancel"));
    d->cancel->setMaximumHeight(fontMetrics().height()+4);
    d->cancel->hide();

    grid->addWidget(d->saneWidget, 0, 0, 1, 6);
    grid->addWidget(help, 1, 0, 1, 1);
    grid->addWidget(d->progress, 1, 2, 1, 1);
    grid->addWidget(d->cancel, 1, 3, 1, 1);
    grid->addWidget(close, 1, 5, 1, 1);
    grid->setColumnStretch(1, 50);
    grid->setColumnStretch(2, 100);
    grid->setColumnStretch(4, 10);
    grid->setRowStretch(0, 10);
    grid->setMargin(0);
    grid->setSpacing(spacingHint());

    setMainWidget(main);
    readSettings();

    // ------------------------------------------------------------------------

    connect(close, SIGNAL(clicked()), 
            this, SLOT(slotClose()));

    connect(d->saneWidget, SIGNAL(scanStart()), 
            this, SLOT(slotScanStart()));

    connect(d->saneWidget, SIGNAL(scanFaild()), 
            this, SLOT(slotScanFailed()));

    connect(d->saneWidget, SIGNAL(scanDone()), 
            this, SLOT(slotScanEnd()));

    connect(d->saneWidget, SIGNAL(imageReady()), 
            this, SLOT(slotSaveImage()));

    connect(d->cancel, SIGNAL(clicked()), 
            d->saneWidget, SLOT(scanCancel()));

    connect(d->saneWidget, SIGNAL(scanProgress(int)), 
            d->progress, SLOT(setValue(int)));
}

ScanDialog::~ScanDialog()
{
    delete d->about;
    delete d;
}

void ScanDialog::readSettings()
{
    KConfig config("kipirc");
    KConfigGroup group = config.group(QString("Scan Tool Dialog"));
    restoreDialogSize(group);
}

void ScanDialog::saveSettings()
{
    KConfig config("kipirc");
    KConfigGroup group = config.group(QString("Scan Tool Dialog"));
    saveDialogSize(group);
    config.sync();
}

void ScanDialog::slotClose()
{
    saveSettings();
    done(Cancel);
}

void ScanDialog::closeEvent(QCloseEvent *e)
{
    d->saneWidget->scanCancel();
    saveSettings();
    e->accept();
}

void ScanDialog::slotHelp()
{
    KToolInvocation::invokeHelp("acquireimages", "kipi-plugins");
}

void ScanDialog::slotScanStart()
{
    d->progress->show();
    d->cancel->show();
}

void ScanDialog::slotScanEnd()
{
    d->progress->hide();
    d->cancel->hide();
}

void ScanDialog::slotScanFailed()
{
    d->progress->hide();
    d->cancel->hide();
    KMessageBox::error(0, i18n("Scanning failed!"));
}

void ScanDialog::slotSaveImage()
{
    QStringList writableMimetypes = KImageIO::mimeTypes(KImageIO::Writing);
    // Put first class citizens at first place
    writableMimetypes.removeAll("image/jpeg");
    writableMimetypes.removeAll("image/tiff");
    writableMimetypes.removeAll("image/png");
    writableMimetypes.insert(0, "image/png");
    writableMimetypes.insert(1, "image/jpeg");
    writableMimetypes.insert(2, "image/tiff");

    kDebug() << "slotSaveImage: Offered mimetypes: " << writableMimetypes << endl;

    QString defaultMimeType("image/png");
    QString defaultFileName("image.png");
    QString format("PNG");

    KFileDialog imageFileSaveDialog(d->interface->currentAlbum().uploadPath(), QString(), 0);

    imageFileSaveDialog.setModal(false);
    imageFileSaveDialog.setOperationMode(KFileDialog::Saving);
    imageFileSaveDialog.setMode(KFile::File);
    imageFileSaveDialog.setSelection(defaultFileName);
    imageFileSaveDialog.setCaption(i18n("New Image File Name"));
    imageFileSaveDialog.setMimeFilter(writableMimetypes, defaultMimeType);

    // Start dialog and check if canceled.
    if ( imageFileSaveDialog.exec() != KFileDialog::Accepted )
       return;

    KUrl newURL = imageFileSaveDialog.selectedUrl();
    QFileInfo fi(newURL.path());

    // Check if target image format have been selected from Combo List of dialog.

    QStringList mimes = KImageIO::typeForMime(imageFileSaveDialog.currentMimeFilter());
    if (!mimes.isEmpty())
    {
        format = mimes.first().toUpper();
    }
    else
    {
        // Else, check if target image format have been add to target image file name using extension.

        format = fi.suffix().toUpper();

        // Check if format from file name extension is include on file mime type list.

        QStringList imgExtList = KImageIO::types(KImageIO::Writing);
        imgExtList << "TIF";
        imgExtList << "TIFF";
        imgExtList << "JPG";
        imgExtList << "JPE";

        if ( !imgExtList.contains( format ) )
        {
            KMessageBox::error(0, i18n("Target image file format \"%1\" unsupported.", format));
            kWarning() << "target image file format " << format << " unsupported!" << endl;
            return;
        }
    }

    if (!newURL.isValid())
    {
        KMessageBox::error(0, i18n("Failed to save file\n\"%1\" to\n\"%2\".",
                              newURL.fileName(),
                              newURL.path().section('/', -2, -2)));
        kWarning() << "target URL is not valid !" << endl;
        return;
    }

    // Check for overwrite ----------------------------------------------------------

    if ( fi.exists() )
    {
        int result = KMessageBox::warningYesNo(0, i18n("A file named \"%1\" already "
                                                       "exists. Are you sure you want "
                                                       "to overwrite it?",
                                               newURL.fileName()),
                                               i18n("Overwrite File?"),
                                               KStandardGuiItem::overwrite(),
                                               KStandardGuiItem::cancel());

        if (result != KMessageBox::Yes)
            return;
    }

    // Perform saving ---------------------------------------------------------------

    kapp->setOverrideCursor( Qt::WaitCursor );
    saveSettings();

    // TODO : support 16 bits color depth image.
    QImage img   = d->saneWidget->getFinalImage()->copy();
    QImage prev  = img.scaled(800, 600, Qt::KeepAspectRatio);
    QImage thumb = img.scaled(160, 120, Qt::KeepAspectRatio);
    QByteArray data((const char*)img.bits(), img.numBytes());
    QByteArray prof = KIPIPlugins::KPWriteImage::getICCProfilFromFile(KDcrawIface::RawDecodingSettings::SRGB);
    KExiv2Iface::KExiv2 meta;    
    meta.setImageProgramId(QString("Kipi-plugins"), QString(kipiplugins_version));
    meta.setImageDimensions(img.size());
    meta.setImagePreview(prev);
    meta.setExifThumbnail(thumb);
    meta.setExifTagString("Exif.Image.DocumentName", QString("Scanned Image")); // not i18n
    meta.setExifTagString("Exif.Image.Make", d->saneWidget->make());
    meta.setExifTagString("Exif.Image.Model", d->saneWidget->model());
    meta.setImageDateTime(QDateTime::currentDateTime());
    meta.setImageOrientation(KExiv2Iface::KExiv2::ORIENTATION_NORMAL);
    meta.setImageColorWorkSpace(KExiv2Iface::KExiv2::WORKSPACE_SRGB);

    KIPIPlugins::KPWriteImage wImageIface;
    wImageIface.setImageData(data, img.width(), img.height(), false, true, prof, meta);

    if (format == QString("JPEG"))
    {
        wImageIface.write2JPEG(newURL.path());
    }
    else if (format == QString("PNG"))
    {
        wImageIface.write2PNG(newURL.path());
    }
    else if (format == QString("TIFF"))
    {
        wImageIface.write2TIFF(newURL.path());
    }
    else
    {
        img.save(newURL.path(), format.toAscii().data());
    }    
    
    d->interface->refreshImages( KUrl::List(newURL) );
    kapp->restoreOverrideCursor();
}

}  // namespace KIPIAcquireImagesPlugin
