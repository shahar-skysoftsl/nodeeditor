#pragma once

#include <QtGui/QPainter>

#include "AbstractNodePainter.hpp"
#include "Definitions.hpp"

namespace QtNodes {

class AbstractQGraphicsScene;
class GraphModel;
class NodeGeometry;
class AbstractNodeGraphicsObject;
class NodeState;

/// @ Lightweight class incapsulating paint code.
class NODE_EDITOR_PUBLIC DefaultNodePainter : public AbstractNodePainter
{
public:
    void paint(QPainter *painter, AbstractNodeGraphicsObject &ngo) const override;

    void drawNodeRect(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;

    void drawConnectionPoints(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;

    void drawFilledConnectionPoints(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;

    void drawNodeCaption(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;

    void drawEntryLabels(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;

    void drawResizeRect(QPainter *painter, AbstractNodeGraphicsObject &ngo) const;
};
} // namespace QtNodes
