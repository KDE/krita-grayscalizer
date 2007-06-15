/*
 * This file is part of the KDE project
 *
 *  Copyright (c) 2007 Cyrille Berger <cberger@cberger.net>
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

#include "Grayscalizer.h"

#include <stdlib.h>
#include <vector>

#include <qradiobutton.h>
#include <klistview.h>
#include <knuminput.h>

#include <klocale.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <kdebug.h>
#include <kgenericfactory.h>

#include <kis_iterators_pixel.h>
#include <kis_progress_display_interface.h>
#include <kis_filter_registry.h>
#include <kis_global.h>
#include <kis_transaction.h>
#include <kis_types.h>
#include <kis_selection.h>

#include <kis_convolution_painter.h>

#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpainter.h>

#include "GrayscalizerConfigurationWidget.h"
#include "GrayscalizerConfigurationBaseWidget.h"


typedef KGenericFactory<KritaGrayscalizer> KritaGrayscalizerFactory;
K_EXPORT_COMPONENT_FACTORY( kritaGrayscalizer, KritaGrayscalizerFactory( "krita" ) )

KritaGrayscalizer::KritaGrayscalizer(QObject *parent, const char *name, const QStringList &)
: KParts::Plugin(parent, name)
{
    setInstance(KritaGrayscalizerFactory::instance());

    kdDebug(41006) << "Grayscalizer filter plugin. Class: "
    << className()
    << ", Parent: "
    << parent -> className()
    << "\n";

    if(parent->inherits("KisFilterRegistry"))
    {
        KisFilterRegistry * manager = dynamic_cast<KisFilterRegistry *>(parent);
        manager->add(new KisGrayscalizerFilter());
    }
}

KritaGrayscalizer::~KritaGrayscalizer()
{
}

KisGrayscalizerFilter::KisGrayscalizerFilter() 
    : KisFilter(id(), "Grayscalizer", i18n("&Grayscalizer..."))
{
}

std::list<KisFilterConfiguration*> KisGrayscalizerFilter::listOfExamplesConfiguration(KisPaintDeviceSP )
{
    std::list<KisFilterConfiguration*> list;
    list.insert(list.begin(), configuration());
    return list;
}

KisFilterConfiguration* KisGrayscalizerFilter::configuration()
{
    KisFilterConfiguration* config = new KisFilterConfiguration(id().id(),1);
    config->setProperty("sigma", 3.0);
    return config;
};

KisFilterConfigWidget * KisGrayscalizerFilter::createConfigurationWidget(QWidget* parent, KisPaintDeviceSP /*dev*/)
{
    GrayscalizerConfigurationWidget* w = new GrayscalizerConfigurationWidget(parent, "");
    Q_CHECK_PTR(w);
    return w;
}

KisFilterConfiguration* KisGrayscalizerFilter::configuration(QWidget* nwidget)
{
    GrayscalizerConfigurationWidget* widget = (GrayscalizerConfigurationWidget*) nwidget;
    if( widget == 0 )
    {
        return configuration();
    } else {
        GrayscalizerConfigurationBaseWidget* baseWidget = widget->widget();
        KisFilterConfiguration* config = new KisFilterConfiguration(id().id(),1);
        config->setProperty("redCoef", baseWidget->coefRed->value() );
        config->setProperty("greenCoef", baseWidget->coefGreen->value() );
        config->setProperty("blueCoef", baseWidget->coefBlue->value() );
        return config;
    }
}

void KisGrayscalizerFilter::process(KisPaintDeviceSP src, KisPaintDeviceSP dst, 
                                   KisFilterConfiguration* config, const QRect& rect ) 
{
    Q_ASSERT(src != 0);
    Q_ASSERT(dst != 0);

    double redCoef = config->getDouble("redCoef", 1.0);
    double greenCoef = config->getDouble("greenCoef", 1.0);
    double blueCoef = config->getDouble("blueCoef", 1.0);
    double totalCoef = redCoef + greenCoef + blueCoef;
    
    KisHLineIteratorPixel dstIt = dst->createHLineIterator(rect.x(), rect.y(), rect.width(), true );
    KisHLineIteratorPixel srcIt = src->createHLineIterator(rect.x(), rect.y(), rect.width(), false);

    int pixelsProcessed = 0;
    setProgressTotalSteps(rect.width() * rect.height());

    KisColorSpace * cs = src->colorSpace();
    QColor c;
    Q_INT32 psize = cs->pixelSize();

    for(int y = 0; y < rect.height(); y++)
    {
        while( ! srcIt.isDone() )
        {
            if(srcIt.isSelected())
            {
                cs->toQColor( srcIt.oldRawData(), &c, (KisProfile*)0 );
                int gray = (int)( (c.red() * redCoef + c.green() * greenCoef + c.blue() * blueCoef) / totalCoef );
                c.setRgb(gray, gray, gray);
                cs->fromQColor(c, dstIt.rawData(), (KisProfile*)0);
            }
            setProgress(++pixelsProcessed);
            ++srcIt;
            ++dstIt;
        }
        dstIt.nextRow();
        srcIt.nextRow();
    }
    
    setProgressDone(); // Must be called even if you don't really support progression
}
