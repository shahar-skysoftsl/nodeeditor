#pragma once

#include <QPainter>

#include "Export.hpp"

class QPainter;

namespace QtNodes {

class AbstractConnectionGraphicsObject;
class NodeDataModel;

/// Class enables custom painting.
class NODE_EDITOR_PUBLIC AbstractConnectionPainter
{
public:
    virtual ~AbstractConnectionPainter() = default;

    /**
   * Reimplement this function in order to have a custom painting.
   *
   * Useful functions:
   * `NodeGraphicsObject::nodeScene()->nodeGeometry()`
   * `NodeGraphicsObject::graphModel()`
   */
    virtual void paint(QPainter *painter, AbstractConnectionGraphicsObject &cgo) const = 0;

    virtual QPainterPath getPainterStroke(AbstractConnectionGraphicsObject const &cgo) = 0;
};
} // namespace QtNodes
