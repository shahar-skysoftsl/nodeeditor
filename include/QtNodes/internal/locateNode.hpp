#pragma once

#include <QtCore/QPointF>
#include <QtGui/QTransform>

class QGraphicsScene;

namespace QtNodes {

class AbstractNodeGraphicsObject;

AbstractNodeGraphicsObject *locateNodeAt(QPointF scenePoint,
                                 QGraphicsScene &scene,
                                 QTransform const &viewTransform);

} // namespace QtNodes
