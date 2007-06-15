/*
 * This file is part of Krita
 *
 * Copyright (c) 2007 Cyrille Berger <cberger@cberger.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "GrayscalizerConfigurationWidget.h"

#include <qlayout.h>
#include <qradiobutton.h>
#include <klistview.h>
#include <knuminput.h>
#include <klocale.h>

#include "GrayscalizerConfigurationBaseWidget.h"

GrayscalizerConfigurationWidget::GrayscalizerConfigurationWidget(QWidget * parent, const char * name) : KisFilterConfigWidget ( parent, name )
{
    QGridLayout *widgetLayout = new QGridLayout(this, 1, 1);
    m_widget = new GrayscalizerConfigurationBaseWidget(this);
    widgetLayout -> addWidget(m_widget,0,0);
    m_widget->listFilters->setSorting(-1);
    connect(m_widget->listFilters, SIGNAL(currentChanged(QListViewItem*)), SLOT(filterChanged(QListViewItem * )));
    
    connect(m_widget->coefRed, SIGNAL(valueChanged(double)), SIGNAL(sigPleaseUpdatePreview()));
    connect(m_widget->coefGreen, SIGNAL(valueChanged(double)), SIGNAL(sigPleaseUpdatePreview()));
    connect(m_widget->coefBlue, SIGNAL(valueChanged(double)), SIGNAL(sigPleaseUpdatePreview()));
}

void GrayscalizerConfigurationWidget::filterChanged(QListViewItem * item)
{
    kdDebug() << item->text(0) << endl;
    if(item->text(0) == i18n("Red"))
    {
        m_widget->coefRed->setValue(1.0);
        m_widget->coefGreen->setValue(0.0);
        m_widget->coefBlue->setValue(0.0);
    } else if(item->text(0) == i18n("Green"))
    {
        m_widget->coefRed->setValue(0.0);
        m_widget->coefGreen->setValue(1.0);
        m_widget->coefBlue->setValue(0.0);
    } else if(item->text(0) == i18n("Yellow"))
    {
        m_widget->coefRed->setValue(1.0);
        m_widget->coefGreen->setValue(1.0);
        m_widget->coefBlue->setValue(0.0);
    } else if(item->text(0) == i18n("Orange"))
    {
        m_widget->coefRed->setValue(1.0);
        m_widget->coefGreen->setValue(0.5);
        m_widget->coefBlue->setValue(0.0);
    } else if(item->text(0) == i18n("Neutral"))
    {
        m_widget->coefRed->setValue(1.0);
        m_widget->coefGreen->setValue(1.0);
        m_widget->coefBlue->setValue(1.0);
    }
}

GrayscalizerConfigurationWidget::~GrayscalizerConfigurationWidget()
{
}

void GrayscalizerConfigurationWidget::setConfiguration(KisFilterConfiguration* config)
{
    QVariant value;
    if (config->getProperty("redCoef", value))
    {
        widget()->coefRed->setValue(value.toInt(0));
    }
    if (config->getProperty("greenCoef", value))
    {
        widget()->coefGreen->setValue(value.toInt(0));
    }
    if (config->getProperty("blueCoef", value))
    {
        widget()->coefBlue->setValue(value.toInt(0));
    }
}

#include "GrayscalizerConfigurationWidget.moc"
