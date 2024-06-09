#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

#include <QGraphicsScene>
#include <QUndoStack>

#include "AbstractGraphModel.hpp"
#include "AbstractPainter.hpp"
#include "AbstractNodeGeometry.hpp"
#include "AbstractGraphicsObject.hpp"
#include "AbstractConnectionGraphicsObject.hpp"
#include "AbstractNodeGraphicsObject.hpp"
#include "AbstractNodePainter.hpp"
#include "AbstractConnectionPainter.hpp"

//using namespace std;

namespace QtNodes {

class AbstractGraphModel;
class AbstractPainter;

class NODE_EDITOR_PUBLIC AbstractQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    AbstractQGraphicsScene(AbstractGraphModel &graphModel, QObject *parent = nullptr);

    AbstractQGraphicsScene() = delete;
    ~AbstractQGraphicsScene() = default;

    /// @returns associated AbstractGraphModel.
    AbstractGraphModel const &graphModel() const;

    AbstractGraphModel &graphModel();

    AbstractNodeGeometry &nodeGeometry();

    AbstractNodePainter &nodePainter();
    AbstractConnectionPainter &connectionPainter();

    virtual AbstractNodeGraphicsObject *nodeGraphicsObject(NodeId nodeId) = 0;
    virtual AbstractConnectionGraphicsObject *connectionGraphicsObject(ConnectionId connectionId) = 0;

    virtual std::unique_ptr<AbstractConnectionGraphicsObject> const &makeDraftConnection(
        ConnectionId const newConnectionId)
        = 0;

    virtual void resetDraftConnection() = 0;

    void setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter);
    void setConnectionPainter(std::unique_ptr<AbstractConnectionPainter> newPainter);

    QUndoStack &undoStack();

    /// Deletes all the nodes. Connections are removed automatically.
    void clearScene();

    virtual Qt::Orientation orientation() const;
    virtual void setOrientation(Qt::Orientation const orientation);

    virtual void setNodeGeometry(std::unique_ptr <AbstractNodeGeometry> nodeGeometry);

    virtual QMenu *createSceneMenu(QPointF const scenePos) = 0;

Q_SIGNALS:
    void modified(AbstractQGraphicsScene *);

    void nodeMoved(NodeId const nodeId, QPointF const &newLocation);

    void nodeClicked(NodeId const nodeId);

    void nodeSelected(NodeId const nodeId);

    void nodeDoubleClicked(NodeId const nodeId);

    void nodeHovered(NodeId const nodeId, QPoint const screenPos);

    void nodeHoverLeft(NodeId const nodeId);

    void connectionHovered(ConnectionId const connectionId, QPoint const screenPos);

    void connectionHoverLeft(ConnectionId const connectionId);

    /// Signal allows showing custom context menu upon clicking a node.
    void nodeContextMenu(NodeId const nodeId, QPointF const pos);

public Q_SLOTS:
    /// Slot called when the `connectionId` is erased form the AbstractGraphModel.
    virtual void onConnectionDeleted(ConnectionId const connectionId) = 0;
    virtual void onConnectionsDeleted(QList<ConnectionId> const connections) = 0;
    //
    /// Slot called when the `connectionId` is created in the AbstractGraphModel.
    virtual void onConnectionCreated(ConnectionId const connectionId) = 0;

    virtual void onNodeCreated(NodeId const nodeId) = 0;
    virtual void onNodeDeleted(NodeId const nodeId) = 0;
    virtual void onNodesDeleted(QList<NodeId> const nodes) = 0;
    virtual void onNodePositionUpdated(NodeId const nodeId) = 0;
    virtual void onNodeUpdated(NodeId const nodeId) = 0;
    virtual void onNodeClicked(NodeId const nodeId) = 0;

    virtual void onModelReset() = 0;

protected:

    virtual void traverseGraphAndPopulateGraphicsObjects() = 0;

    /// Redraws adjacent nodes for given `connectionId`
    virtual void updateAttachedNodes(ConnectionId const connectionId, PortType const portType) = 0;
    
    AbstractGraphModel &_graphModel;

    std::unique_ptr<AbstractNodeGeometry> _nodeGeometry;

    std::unique_ptr<AbstractNodePainter> _nodePainter;
    std::unique_ptr<AbstractConnectionPainter> _connectionPainter;

    bool _nodeDrag;

    QUndoStack *_undoStack;

    Qt::Orientation _orientation;
}; 
} // namespace QtNodes;
