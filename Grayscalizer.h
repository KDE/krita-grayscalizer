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

#ifndef _Grayscalizer_H_
#define _Grayscalizer_H_

// TODO: remove that
#define LCMS_HEADER <lcms.h>
// TODO: remove it !

#include <kparts/plugin.h>
#include <kis_filter.h>

class GrayscalizerFilterConfig;

class KritaGrayscalizer : public KParts::Plugin
{
public:
    KritaGrayscalizer(QObject *parent, const char *name, const QStringList &);
    virtual ~KritaGrayscalizer();
};

class KisGrayscalizerFilter : public KisFilter
{
public:
    KisGrayscalizerFilter();
public:
    virtual void process(KisPaintDeviceSP src, KisPaintDeviceSP dst, KisFilterConfiguration*, const QRect&);
    virtual ColorSpaceIndependence colorSpaceIndependence() { return TO_RGBA8; };
    static inline KisID id() { return KisID("Grayscalizer", i18n("Grayscalizer")); };
    virtual bool supportsPainting() { return false; }
    virtual bool supportsPreview() { return true; }
    virtual bool supportsIncrementalPainting() { return false; }
    virtual bool supportsAdjustmentLayers() { return true; }
    
    virtual std::list<KisFilterConfiguration*> listOfExamplesConfiguration(KisPaintDeviceSP );
public:
    virtual KisFilterConfigWidget * createConfigurationWidget(QWidget* parent, KisPaintDeviceSP dev);
    virtual KisFilterConfiguration* configuration(QWidget*);
    virtual KisFilterConfiguration* configuration();

};

#endif
