/**
 * Copyright (C) 2012 by INdT
 * Copyright (C) 2014 Bacon2D Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @author Rodrigo Goncalves de Oliveira <rodrigo.goncalves@openbossa.org>
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

#include "sprite.h"
#include "scene.h"
#include "spritesheet.h"
#include "spriteanimation.h"
#include "animationchangeevent.h"
#include "animationtransition.h"

void Sprite::append_animation(QQmlListProperty<SpriteAnimation> *list, SpriteAnimation *animation)
{
    Sprite *spriteItem = qobject_cast<Sprite *>(list->object);

    if (!spriteItem)
        return;

    spriteItem->m_states.insert(animation->name(), animation);
    animation->spriteSheet()->setParentItem(spriteItem);
}

/*!
  \qmltype Sprite
  \inqmlmodule Bacon2D
  \inherits Item
  \brief A Sprite, providing state based
   management of multiple SpriteAnimation animations.
 */
Sprite::Sprite(QQuickItem *parent)
    : QQuickItem(parent)
    , m_entity(0)
    , m_stateMachine(0)
    , m_stateGroup(0)
    , m_verticalMirror(false)
    , m_horizontalMirror(false)
{
}

QQmlListProperty<SpriteAnimation> Sprite::animations() const
{
    return QQmlListProperty<SpriteAnimation>(const_cast<Sprite *>(this), 0, &Sprite::append_animation, 0, 0, 0);
}

/*!
 * \qmlproperty string Sprite::animation
 * \brief The current SpriteAnimation state of the Sprite as a string
 */
QString Sprite::animation() const
{
    return m_animation;
}

void Sprite::setAnimation(const QString &animation, const bool &force)
{
    if (!m_states.contains(animation)) {
        qWarning() << "SpriteAnimation:" << animation << "invalid";
        return;
    }

    if (force || (m_animation != animation)) {
        if (m_animation != QString() && m_states.contains(m_animation)) {
            SpriteAnimation *animationItem = m_states[m_animation];
            animationItem->setRunning(false);
            animationItem->setVisible(false);
        }
        m_animation = animation;

        if (!m_stateMachine)
            initializeMachine();

        if (m_stateMachine && m_stateMachine->isRunning())
            m_stateMachine->postEvent(new AnimationChangeEvent(m_animation));

        emit animationChanged();
    }
}

void Sprite::initializeMachine()
{
    m_stateMachine= new QStateMachine;
    m_stateGroup = new QState(QState::ParallelStates);

    SpriteAnimation *animation;
    foreach (animation, m_states.values()) {
        AnimationTransition *transition = new AnimationTransition(animation);
        animation->setParent(m_stateGroup);
        animation->addTransition(transition);

        if (width() == 0 || height() == 0) {
            setWidth(animation->spriteSheet()->width());
            setHeight(animation->spriteSheet()->height());
        }
    }

    m_stateMachine->addState(m_stateGroup);
    m_stateMachine->setInitialState(m_stateGroup);

    connect(m_stateMachine, SIGNAL(started()), this, SLOT(initializeAnimation()));

    m_stateMachine->start();
}

void Sprite::initializeAnimation()
{
    if (m_animation != QString())
        setAnimation(m_animation, true);
}

/*!
 * \qmlproperty bool Sprite::verticalMirror
 * \brief Shows the Sprite mirrored vertically (upside down)
 */
bool Sprite::verticalMirror() const
{
    return m_verticalMirror;
}

void Sprite::setVerticalMirror(const bool &verticalMirror)
{
    if (m_verticalMirror == verticalMirror)
        return;

    m_verticalMirror = verticalMirror;

    foreach (SpriteAnimation *animation, m_states.values())
        animation->setVerticalMirror(m_verticalMirror);

    emit verticalMirrorChanged();
}

/*!
 * \qmlproperty bool Sprite::horizontalMirror
 * \brief Shows the Sprite mirrored horizontally
 */
bool Sprite::horizontalMirror() const
{
    return m_horizontalMirror;
}

void Sprite::setHorizontalMirror(const bool &horizontalMirror)
{
    if (m_horizontalMirror == horizontalMirror)
        return;

    m_horizontalMirror = horizontalMirror;

    foreach (SpriteAnimation *animation, m_states.values())
        animation->setHorizontalMirror(m_horizontalMirror);

    emit horizontalMirrorChanged();
}

Entity *Sprite::entity() const
{
    return m_entity;
}

void Sprite::setEntity(Entity *entity)
{
    if (m_entity == entity)
        return;

    m_entity = entity;

    emit entityChanged();
}
