/* * This file is part of Maliit framework *
 *
 * Copyright (C) 2013 Openismus GmbH
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef MALIIT_ABSTRACT_PLATFORM_H
#define MALIIT_ABSTRACT_PLATFORM_H

#include <maliit/namespace.h>

#include <QWindow>

#include <memory>

QT_BEGIN_NAMESPACE
class QRegion;
QT_END_NAMESPACE

namespace Maliit
{

class AbstractPlatform
{
public:
    virtual ~AbstractPlatform();
    virtual void setupInputPanel(QWindow* window,
                                 Maliit::Position position) = 0;
    virtual void setInputRegion(QWindow* window,
                                const QRegion& region) = 0;
    virtual void setApplicationWindow(QWindow *window, WId appWindowId);
};

std::unique_ptr<AbstractPlatform> createPlatform();

} // namespace Maliit

#endif // MALIIT_ABSTRACT_PLATFORM_H
