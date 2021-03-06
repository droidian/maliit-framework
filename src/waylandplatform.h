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

#ifndef MALIIT_WAYLAND_PLATFORM_H
#define MALIIT_WAYLAND_PLATFORM_H

#include <QScopedPointer>

#include "abstractplatform.h"

namespace Maliit
{

class WaylandPlatformPrivate;

class WaylandPlatform : public AbstractPlatform
{
    Q_DECLARE_PRIVATE(WaylandPlatform)

public:
    WaylandPlatform();
    ~WaylandPlatform();

    virtual void setupInputPanel(QWindow* window,
                                 Maliit::Position position);
    virtual void setInputRegion(QWindow* window,
                                const QRegion& region);

private:
    QScopedPointer<WaylandPlatformPrivate> d_ptr;
};

} // namespace Maliit

#endif // MALIIT_WAYLAND_PLATFORM_H
