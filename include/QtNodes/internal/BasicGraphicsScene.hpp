#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QMenu>


#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "Export.hpp"


#include "AbstractQGraphicsScene.hpp"
#include "NodeGraphicsObject.hpp"
#include "ConnectionGraphicsObject.hpp"

#include "GraphicsScene.h"

class QUndoStack;

namespace QtNodes {

//class AbstractGraphModel;
//class AbstractNodePainter;
//class AbstractConnectionPainter;
//class NodeStyle;


/// An instance of QGraphicsScene, holds connections and nodes.
class NODE_EDITOR_PUBLIC BasicGraphicsScene : public GraphicsScene<NodeGraphicsObject, ConnectionGraphicsObject>
{
    Q_OBJECT
public:
    BasicGraphicsScene(AbstractGraphModel &graphModel, QObject *parent = nullptr)
        : GraphicsScene(graphModel, parent)
    {
        qDebug() << "BasicGraphicsScene";
    };

    // Scenes without models are not supported
    BasicGraphicsScene() = delete;

    ~BasicGraphicsScene() = default;
};

} // namespace QtNodes
