/* * This file is part of meego-im-framework *
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * Contact: Nokia Corporation (directui@nokia.com)
 *
 * If you have questions regarding the use of this file, please contact
 * Nokia at directui@nokia.com.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */

#ifndef MINPUTCONTEXTGLIBDBUSCONNECTION_H
#define MINPUTCONTEXTGLIBDBUSCONNECTION_H

#include "minputcontextconnection.h"
#include "mattributeextensionid.h"

#include <QWidget>
#include <QByteArray>
#include <QMap>
#include <QSet>
#include <QEvent>
#include <QString>
#include <QVariant>
#include <QWidget>

class QPoint;
class QRegion;
struct MDBusGlibICConnection;
struct MIMSDBusActivater;
struct DBusServer;

//! \internal
/*! \brief Peer-to-peer DBus input context connection based on glib dbus bindings
 */
class MInputContextGlibDBusConnection : public MInputContextConnection
{
    Q_OBJECT

public:
    MInputContextGlibDBusConnection();
    virtual ~MInputContextGlibDBusConnection();

    void handleNewDBusConnectionReady(MDBusGlibICConnection *connectionObj);
    void handleDBusDisconnection(MDBusGlibICConnection *connectionObj);

    //! \reimp
    virtual void sendPreeditString(const QString &string,
                                   const QList<MInputMethod::PreeditTextFormat> &preeditFormats,
                                   int replacementStart = 0, int replacementLength = 0,
                                   int cursorPos = -1);
    virtual void sendCommitString(const QString &string, int replaceStart = 0,
                                  int replaceLength = 0, int cursorPos = -1);
    virtual void sendKeyEvent(const QKeyEvent &keyEvent,
                              MInputMethod::EventRequestType requestType);
    virtual void notifyImInitiatedHiding();

    virtual int contentType(bool &valid);
    virtual bool correctionEnabled(bool &valid);
    virtual bool predictionEnabled(bool &valid);
    virtual bool autoCapitalizationEnabled(bool &valid);
    virtual void setGlobalCorrectionEnabled(bool);
    virtual bool surroundingText(QString &text, int &cursorPosition);
    virtual bool hasSelection(bool &valid);
    virtual int inputMethodMode(bool &valid);
    virtual QRect preeditRectangle(bool &valid);
    virtual QRect cursorRectangle(bool &valid);
    virtual bool hiddenText(bool &valid);
    virtual void setRedirectKeys(bool enabled);
    virtual void setDetectableAutoRepeat(bool enabled);
    virtual void copy();
    virtual void paste();
    virtual void setSelection(int start, int length);
    virtual int anchorPosition(bool &valid);
    virtual QString selection(bool &valid);
    virtual void setLanguage(const QString &language);
    virtual void addTarget(MAbstractInputMethod *target);
    virtual int preeditClickPos(bool &valid) const;
    //! \reimp_end

public Q_SLOTS:
    // communication w/ minputcontext

    //! \reimp
    virtual void updateInputMethodArea(const QRegion &region);
    //! \reimp_end

public:
    //! sets the input to go to calling connection
    void activateContext(MDBusGlibICConnection *connection);

    //! ipc method provided to the application, shows input method
    void showInputMethod(unsigned int clientId);

    //! ipc method provided to the application, hides input method
    void hideInputMethod(unsigned int clientId);

    //! ipc method provided to the application, signals mouse click on preedit
    void mouseClickedOnPreedit(unsigned int clientId,
                               const QPoint &pos, const QRect &preeditRect);

    //! ipc method provided to the application, sets preedit
    void setPreedit(unsigned int clientId, const QString &text, int cursorPos);

    void updateWidgetInformation(unsigned int clientId,
                                 const QMap<QString, QVariant> &stateInformation,
                                 bool focusChanged);

    //! ipc method provided to the application, resets the input method
    void reset(unsigned int clientId);

    /*!
     * \brief Target application is changing orientation
     */
    void receivedAppOrientationAboutToChange(unsigned int clientId, int angle);

    /*!
     * \brief Target application changed orientation (already finished)
     */
    void receivedAppOrientationChanged(unsigned int clientId, int angle);

    /*! \brief Set copy/paste state for appropriate UI elements in the input method server
     *  \param copyAvailable bool TRUE if text is selected
     *  \param pasteAvailable bool TRUE if clipboard content is not empty
     */
    void setCopyPasteState(unsigned int clientId,
                                   bool copyAvailable, bool pasteAvailable);

    /*!
     * \brief Process a key event redirected from hardware keyboard to input method plugin(s).
     *
     * This is called only if one has enabled redirection by calling \a setRedirectKeys.
     */
    void processKeyEvent(unsigned int clientId, QEvent::Type keyType, Qt::Key keyCode,
                         Qt::KeyboardModifiers modifiers, const QString &text, bool autoRepeat,
                         int count, quint32 nativeScanCode, quint32 nativeModifiers, unsigned long time);

    /*!
     * \brief Register an input method attribute extension which is defined in \a fileName with the
     * unique identifier \a id.
     *
     *  The \a id should be unique, and the \a fileName is the absolute file name of the
     *  attribute extension.
     */
    void registerAttributeExtension(unsigned int clientId, int id, const QString &fileName);

    /*!
     * \brief Unregister an input method attribute extension which unique identifier is \a id.
     */
    void unregisterAttributeExtension(unsigned int clientId, int id);

    /*!
     * \brief Sets the \a attribute for the \a target in the extended attribute which has unique \a id to \a value.
     */
    void setExtendedAttribute(unsigned int clientId, int id, const QString &target,
                              const QString &targetItem, const QString &attribute, const QVariant &value);

Q_SIGNALS:
    void appOrientationAboutToChange(int angle);
    void appOrientationChanged(int angle);

private:
    //! Updates the transient hint on the framework side to point to the
    //! current application's window id.
    void updateTransientHint();

    //! Helper method for setLanguage(QString) to use it for other than active connection.
    void setLanguage(MDBusGlibICConnection *targetIcConnection,
                     const QString &language);

    /*!
     * \brief get the X window id of the active app window
     */
    WId winId(bool &valid);

    unsigned int mActiveClientId; // 0 means no active client
    MDBusGlibICConnection *activeContext;
    QMap<QString, QVariant> widgetState;
    bool globalCorrectionEnabled;
    bool redirectionEnabled;
    bool detectableAutoRepeat;
    int lastOrientation;
    MAttributeExtensionId attributeExtensionId; //current attribute extension id
    QSet<MAttributeExtensionId> attributeExtensionIds; //all attribute extension ids
    QByteArray socketAddress;

    DBusServer *server;

    QString preedit;

    //! Cached values to be sent from server to new input contexts.
    QString lastLanguage;

    Q_DISABLE_COPY(MInputContextGlibDBusConnection)
};
//! \internal_end


#endif
