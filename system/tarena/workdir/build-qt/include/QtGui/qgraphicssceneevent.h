/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGRAPHICSSCENEEVENT_H
#define QGRAPHICSSCENEEVENT_H

#include <QtCore/qcoreevent.h>
#include <QtCore/qpoint.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

#if !defined(QT_NO_GRAPHICSVIEW) || (QT_EDITION & QT_MODULE_GRAPHICSVIEW) != QT_MODULE_GRAPHICSVIEW

class QMimeData;
class QPointF;
class QSizeF;
class QWidget;

class QGraphicsSceneEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneEvent : public QEvent
{
public:
    QGraphicsSceneEvent(Type type);
    ~QGraphicsSceneEvent();

    QWidget *widget() const;
    void setWidget(QWidget *widget);

protected:
    QGraphicsSceneEvent(QGraphicsSceneEventPrivate &dd, Type type = None);
    QGraphicsSceneEventPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QGraphicsSceneEvent)
};

class QGraphicsSceneMouseEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneMouseEvent : public QGraphicsSceneEvent
{
public:
    QGraphicsSceneMouseEvent(Type type = None);
    ~QGraphicsSceneMouseEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    QPointF buttonDownPos(Qt::MouseButton button) const;
    void setButtonDownPos(Qt::MouseButton button, const QPointF &pos);

    QPointF buttonDownScenePos(Qt::MouseButton button) const;
    void setButtonDownScenePos(Qt::MouseButton button, const QPointF &pos);

    QPoint buttonDownScreenPos(Qt::MouseButton button) const;
    void setButtonDownScreenPos(Qt::MouseButton button, const QPoint &pos);

    QPointF lastPos() const;
    void setLastPos(const QPointF &pos);

    QPointF lastScenePos() const;
    void setLastScenePos(const QPointF &pos);

    QPoint lastScreenPos() const;
    void setLastScreenPos(const QPoint &pos);

    Qt::MouseButtons buttons() const;
    void setButtons(Qt::MouseButtons buttons);

    Qt::MouseButton button() const;
    void setButton(Qt::MouseButton button);

    Qt::KeyboardModifiers modifiers() const;
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneMouseEvent)
};

class QGraphicsSceneWheelEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneWheelEvent : public QGraphicsSceneEvent
{
public:
    QGraphicsSceneWheelEvent(Type type = None);
    ~QGraphicsSceneWheelEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    Qt::MouseButtons buttons() const;
    void setButtons(Qt::MouseButtons buttons);

    Qt::KeyboardModifiers modifiers() const;
    void setModifiers(Qt::KeyboardModifiers modifiers);

    int delta() const;
    void setDelta(int delta);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneWheelEvent)
};

class QGraphicsSceneContextMenuEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneContextMenuEvent : public QGraphicsSceneEvent
{
public:
    enum Reason { Mouse, Keyboard, Other };

    QGraphicsSceneContextMenuEvent(Type type = None);
    ~QGraphicsSceneContextMenuEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    Qt::KeyboardModifiers modifiers() const;
    void setModifiers(Qt::KeyboardModifiers modifiers);

    Reason reason() const;
    void setReason(Reason reason);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneContextMenuEvent)
};

class QGraphicsSceneHoverEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneHoverEvent : public QGraphicsSceneEvent
{
public:
    QGraphicsSceneHoverEvent(Type type = None);
    ~QGraphicsSceneHoverEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    QPointF lastPos() const;
    void setLastPos(const QPointF &pos);

    QPointF lastScenePos() const;
    void setLastScenePos(const QPointF &pos);

    QPoint lastScreenPos() const;
    void setLastScreenPos(const QPoint &pos);

    Qt::KeyboardModifiers modifiers() const;
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneHoverEvent)
};

class QGraphicsSceneHelpEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneHelpEvent : public QGraphicsSceneEvent
{
public:
    QGraphicsSceneHelpEvent(Type type = None);
    ~QGraphicsSceneHelpEvent();

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneHelpEvent)
};

class QGraphicsSceneDragDropEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneDragDropEvent : public QGraphicsSceneEvent
{
public:
    QGraphicsSceneDragDropEvent(Type type = None);
    ~QGraphicsSceneDragDropEvent();

    QPointF pos() const;
    void setPos(const QPointF &pos);

    QPointF scenePos() const;
    void setScenePos(const QPointF &pos);

    QPoint screenPos() const;
    void setScreenPos(const QPoint &pos);

    Qt::MouseButtons buttons() const;
    void setButtons(Qt::MouseButtons buttons);

    Qt::KeyboardModifiers modifiers() const;
    void setModifiers(Qt::KeyboardModifiers modifiers);

    Qt::DropActions possibleActions() const;
    void setPossibleActions(Qt::DropActions actions);

    Qt::DropAction proposedAction() const;
    void setProposedAction(Qt::DropAction action);
    void acceptProposedAction();

    Qt::DropAction dropAction() const;
    void setDropAction(Qt::DropAction action);

    QWidget *source() const;
    void setSource(QWidget *source);

    const QMimeData *mimeData() const;
    void setMimeData(const QMimeData *data);

private:
    Q_DECLARE_PRIVATE(QGraphicsSceneDragDropEvent)
};

class QGraphicsSceneResizeEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneResizeEvent : public QGraphicsSceneEvent
{
    Q_DECLARE_PRIVATE(QGraphicsSceneResizeEvent)
public:
    QGraphicsSceneResizeEvent();
    ~QGraphicsSceneResizeEvent();

    QSizeF oldSize() const;
    void setOldSize(const QSizeF &size);

    QSizeF newSize() const;
    void setNewSize(const QSizeF &size);
};

class QGraphicsSceneMoveEventPrivate;
class Q_GUI_EXPORT QGraphicsSceneMoveEvent : public QGraphicsSceneEvent
{
    Q_DECLARE_PRIVATE(QGraphicsSceneMoveEvent)
public:
    QGraphicsSceneMoveEvent();
    ~QGraphicsSceneMoveEvent();

    QPointF oldPos() const;
    void setOldPos(const QPointF &pos);

    QPointF newPos() const;
    void setNewPos(const QPointF &pos);
};

#endif // QT_NO_GRAPHICSVIEW

QT_END_NAMESPACE

QT_END_HEADER

#endif
