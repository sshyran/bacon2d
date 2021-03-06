/**
 * The MIT License (MIT)
 *
 * Copyright (C) 2014 by Bacon2D
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

import QtQuick 2.2
import QtQuick.Window 2.0
import Bacon2D 1.0

Window {
    width: 512
    height: 512
    visible: true

    Game {
        id: game
        anchors.fill: parent

        currentScene: scene

        Scene {
            id: scene

            focus: true
            width: parent.width
            height: parent.height

            ImageLayer {
                id: layer
                anchors.fill: parent
                source: "sky.png"

                behavior: ScrollBehavior {
                    verticalStep: -15
                }
            }

            Sprite {
                id: spriteItem

                x: scene.width / 2 - spriteItem.width / 2
                y: scene.height / 2 - spriteItem.height / 2

                animation: "falling"

                animations: SpriteAnimation {
                    name: "falling"
                    source: "astronaut.png"
                    frames: 3
                    duration: 450
                    loops: Animation.Infinite
                }

                NumberAnimation on rotation {
                    from: 0
                    to: 360
                    running: game.gameState === Bacon2D.Running
                    loops: Animation.Infinite
                    duration: 1800
                }
            }
        }
    }
}
