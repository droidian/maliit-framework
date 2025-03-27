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

#include <wayland-client.h>

#include <QDebug>
#include <QGuiApplication>
#include <QRegion>
#include <QWindow>

#include "waylandplatform.h"
#include "windowdata.h"

using namespace LayerShellQt;

namespace Maliit
{

class WaylandPlatformPrivate
{
public:
    WaylandPlatformPrivate() = default;
    ~WaylandPlatformPrivate() = default;
};

WaylandPlatform::WaylandPlatform()
    : d_ptr(new WaylandPlatformPrivate)
{}

void WaylandPlatform::setupInputPanel(QWindow* window,
                                      Maliit::Position position)
{
    if (qgetenv("QT_WAYLAND_SHELL_INTEGRATION") == QByteArray("layer-shell")) {
        m_lsWindow.reset(Window::get(window));
        m_lsWindow.get()->setLayer(Window::Layer::LayerOverlay);
        m_lsWindow.get()->setKeyboardInteractivity(Window::KeyboardInteractivity::KeyboardInteractivityNone);
        Window::Anchors anchors = Window::Anchor::AnchorBottom;
        switch (position) {
            case PositionLeftBottom: {
                anchors |= Window::Anchor::AnchorLeft;
                break;
            }
            case PositionRightBottom: {
                anchors |= Window::Anchor::AnchorRight;
                break;
            }
            default: {
                break;
            }
        }
        m_lsWindow.get()->setAnchors(anchors);
    }
}

void WaylandPlatform::setInputRegion(QWindow* window,
                                     const QRegion& region)
{
    if (not window) {
        return;
    }

    window->setMask(region);

    if(!region.boundingRect().isNull())
        m_lsWindow.get()->setExclusiveZone(region.boundingRect().height());
}

WaylandPlatform::~WaylandPlatform()
{

}

} // namespace Maliit
