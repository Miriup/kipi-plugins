/* ============================================================
 * File  : galleryconfig.cpp
 * Author: Colin Guthrie <kde@colin.guthr.ie>
 * Date  : 2006-09-04
 * Copyright 2006 by Colin Guthrie <kde@colin.guthr.ie>
 *
 *
 * Modified by : Andrea Diamantini <adjam7@gmail.com>
 * Date        : 2008-07-11
 * Copyright 2008 by Andrea Diamantini <adjam7@gmail.com>
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

// Include files for Qt

#include <QListWidget>
#include <QProgressDialog>
#include <QFrame>

#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

// Include files for KDE

#include <KLocale>
#include <khtml_part.h>
#include <khtmlview.h>
#include <KDebug>

// KIPI include files

#include <libkipi/version.h>
#include <libkipi/interface.h>
//#include <libkipi/imagedialog.h>

// Local includes.

#include "galleryconfig.h"
#include "galleries.h"

namespace KIPIGalleryExportPlugin
{

GalleryEdit::GalleryEdit(QWidget* pParent,
                         Gallery* pGallery,
                         QString title)
    : KDialog(pParent, Qt::Dialog), //KDialog(pParent, 0, true, title, Ok|Cancel, Ok, false),
      mpGallery(pGallery)
{
// TODO: system this
//  setButtonGuiItem( Ok, KStandardGuiItem::save() );

  QFrame *page = new QFrame (this);
  QHBoxLayout *tll = new QHBoxLayout(page);
  page->setMinimumSize (500, 200);
  setMainWidget(page);

  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setSpacing (KDialog::spacingHint());
  tll->addItem(vbox);

  mpHeaderLabel = new QLabel(page);
  mpHeaderLabel->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,
                                            QSizePolicy::Fixed));
  mpHeaderLabel->setText(title);
  vbox->addWidget(mpHeaderLabel);

  QFrame* hline = new QFrame(page);//, "hline");
  hline->setFrameShape(QFrame::HLine);
  hline->setFrameShadow(QFrame::Sunken);
  hline->setFrameShape(QFrame::HLine);
  vbox->addWidget(hline);

  QGridLayout* centerLayout = new QGridLayout(); //0, 1, 1, 5, 5);

  mpNameEdit = new QLineEdit( this );
  centerLayout->addWidget(mpNameEdit, 0, 1);

  mpUrlEdit = new QLineEdit( this );
  centerLayout->addWidget(mpUrlEdit, 1, 1);

  mpUsernameEdit = new QLineEdit( this );
  centerLayout->addWidget(mpUsernameEdit, 2, 1);

  mpPasswordEdit = new QLineEdit( this );
  mpPasswordEdit->setEchoMode(QLineEdit::Password);
  centerLayout->addWidget(mpPasswordEdit, 3, 1);

  QLabel* name_label = new QLabel(this);
  name_label->setText(i18n( "Name:" ));
  centerLayout->addWidget(name_label, 0, 0);

  QLabel* urlLabel = new QLabel(this);
  urlLabel->setText(i18n( "URL:" ));
  centerLayout->addWidget(urlLabel, 1, 0);

  QLabel* nameLabel = new QLabel(this);
  nameLabel->setText(i18n( "Username:" ));
  centerLayout->addWidget(nameLabel, 2, 0);

  QLabel* passwdLabel = new QLabel(this);
  passwdLabel->setText(i18n( "Password:" ));
  centerLayout->addWidget(passwdLabel, 3, 0);

  //---------------------------------------------
  mpGalleryVersion = new QCheckBox( i18n("Use &Gallery 2"), this);
  mpGalleryVersion->setChecked( 2 == pGallery->version() );
  centerLayout->addWidget( mpGalleryVersion, 4, 1 );
  //---------------------------------------------

  vbox->addLayout( centerLayout );

  resize( QSize(300, 150).expandedTo(minimumSizeHint()) );

  // TODO: system this
  // clearWState( WState_Polished );

  mpNameEdit->setText(pGallery->name());
  mpUrlEdit->setText(pGallery->url());
  mpUsernameEdit->setText(pGallery->username());
  mpPasswordEdit->setText(pGallery->password());
}

GalleryEdit::~GalleryEdit()
{

}

void GalleryEdit::slotOk(void)
{
  if (mpNameEdit->isModified())
    mpGallery->setName(mpNameEdit->text());
  if (mpUrlEdit->isModified())
    mpGallery->setUrl(mpUrlEdit->text());
  if (mpUsernameEdit->isModified())
    mpGallery->setUsername(mpUsernameEdit->text());
  if (mpPasswordEdit->isModified())
    mpGallery->setPassword(mpPasswordEdit->text());
  if (mpGalleryVersion->isChecked())
    mpGallery->setVersion(2);
  else
    mpGallery->setVersion(1);
  accept();
}

}

#include "galleryconfig.moc"

