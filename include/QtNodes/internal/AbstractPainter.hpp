#pragma once

#pragma once

#include <QGraphicsObject>
#include <QPainter>

#include "Export.hpp"

class QPainter;

namespace QtNodes {

/// Class enables custom painting.
class NODE_EDITOR_PUBLIC AbstractPainter
{
public:
    virtual ~AbstractPainter() = default;

    /**
   * Reimplement this function in order to have a custom painting.
   */
    virtual void paint(QPainter *painter, QGraphicsObject &ngo) const = 0;

    virtual QPainterPath getPainterStroke(QGraphicsObject const &cgo) { return QPainterPath(); };
};
} // namespace QtNodes
