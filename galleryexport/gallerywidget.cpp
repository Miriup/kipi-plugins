/* ============================================================
 * File  : gallerywidget.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-12-01
 * Description : 
 * 
 * Copyright 2004 by Renchi Raju

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

#include <klocale.h>
#include <khtml_part.h>
#include <khtmlview.h>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qheader.h>
#include <qlistview.h>
#include <qbuttongroup.h>
#include <qhgroupbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "gallerywidget.h"

namespace KIPIGalleryExportPlugin
{

GalleryWidget::GalleryWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "GalleryWidget" );
    QVBoxLayout*  GalleryWidgetLayout
        = new QVBoxLayout( this, 5, 5, "GalleryWidgetLayout"); 

    // ------------------------------------------------------------------------
    
    QLabel*       headerLabel;
    QFrame*       headerLine;

    headerLabel = new QLabel( this, "headerLabel" );
    GalleryWidgetLayout->addWidget( headerLabel );
    headerLine = new QFrame( this, "headerLine" );
    headerLine->setFrameShape( QFrame::HLine );
    headerLine->setFrameShadow( QFrame::Sunken );
    GalleryWidgetLayout->addWidget( headerLine );

    // ------------------------------------------------------------------------

    QHBoxLayout*  hbox = new QHBoxLayout( 0, 0, 5, "hbox"); 

    m_albumView = new QListView( this, "m_albumView" );
    m_albumView->addColumn( i18n( "Albums" ) );
    m_albumView->setResizeMode( QListView::AllColumns );
    hbox->addWidget( m_albumView );

    // ------------------------------------------------------------------------

    m_photoView = new KHTMLPart( this, "m_photoView" );
    hbox->addWidget( m_photoView->view() );

    // ------------------------------------------------------------------------
    
    QVBoxLayout*  rightButtonGroupLayout;
    QSpacerItem*  spacer;
    QButtonGroup* rightButtonGroup;

    rightButtonGroup = new QButtonGroup( this, "rightButtonGroup" );
    rightButtonGroupLayout = new QVBoxLayout( rightButtonGroup );
    rightButtonGroupLayout->setSpacing( 5 );
    rightButtonGroupLayout->setMargin( 5 );

    m_newAlbumBtn = new QPushButton( rightButtonGroup, "m_newAlbumBtn" );
    m_newAlbumBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightButtonGroupLayout->addWidget( m_newAlbumBtn, 0, Qt::AlignHCenter );

    m_addPhotoBtn = new QPushButton( rightButtonGroup, "m_addPhotoBtn" );
    m_addPhotoBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightButtonGroupLayout->addWidget( m_addPhotoBtn, 0, Qt::AlignHCenter );

    // ------------------------------------------------------------------------
    
    QHGroupBox* scaleBox = new QHGroupBox(i18n("Maximum Photo Size"),
                                          rightButtonGroup);
    m_widthSpinBox  = new QSpinBox(0, 5000, 10, scaleBox);
    QLabel* filler  = new QLabel("x", scaleBox);
    m_heightSpinBox = new QSpinBox(0, 5000, 10, scaleBox);
    m_widthSpinBox->setValue(600);
    m_heightSpinBox->setValue(600);
    filler->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_widthSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_heightSpinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    rightButtonGroupLayout->addWidget(scaleBox);

    // ------------------------------------------------------------------------
    
    spacer = new QSpacerItem( 20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding );
    rightButtonGroupLayout->addItem( spacer );

    hbox->addWidget( rightButtonGroup );
    GalleryWidgetLayout->addLayout( hbox );

    // ------------------------------------------------------------------------
    
    headerLabel->setText( i18n( "<h2>Gallery Export</h2>" ) );
    m_albumView->header()->setLabel( 0, i18n( "Albums" ) );
    m_newAlbumBtn->setText( i18n( "&New Album" ) );
    m_addPhotoBtn->setText( i18n( "&Add Photos" ) );

    // ------------------------------------------------------------------------

    resize( QSize(600, 400).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

GalleryWidget::~GalleryWidget()
{
}

}
