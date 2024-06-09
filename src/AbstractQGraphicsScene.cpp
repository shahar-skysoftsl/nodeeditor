#include "AbstractQGraphicsScene.hpp"
#include "DefaultConnectionPainter.hpp"
#include "DefaultNodePainter.hpp"

#include <DefaultHorizontalNodeGeometry.hpp>
#include <QUndoStack>

#include <QtWidgets/QGraphicsSceneMoveEvent>

#include "DefaultHorizontalNodeGeometry.hpp"
#include "DefaultVerticalNodeGeometry.hpp"

namespace QtNodes {

AbstractQGraphicsScene::AbstractQGraphicsScene(AbstractGraphModel &graphModel, QObject *parent)
    : QGraphicsScene(parent)
    , _graphModel(graphModel)
    , _nodeGeometry(std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel))
    , _nodePainter(std::make_unique<DefaultNodePainter>())
    , _connectionPainter(std::make_unique<DefaultConnectionPainter>())
    , _nodeDrag(false)
    , _undoStack(new QUndoStack(this))
    , _orientation(Qt::Horizontal) {


    connect(&_graphModel,
            &AbstractGraphModel::connectionCreated,
            this,
            &AbstractQGraphicsScene::onConnectionCreated);

    connect(&_graphModel,
            &AbstractGraphModel::connectionDeleted,
            this,
            &AbstractQGraphicsScene::onConnectionDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::connectionsDeleted,
            this,
            &AbstractQGraphicsScene::onConnectionsDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::nodeCreated,
            this,
            &AbstractQGraphicsScene::onNodeCreated);

    connect(&_graphModel,
            &AbstractGraphModel::nodeDeleted,
            this,
            &AbstractQGraphicsScene::onNodeDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::nodesDeleted,
            this,
            &AbstractQGraphicsScene::onNodesDeleted);

    connect(&_graphModel,
            &AbstractGraphModel::nodePositionUpdated,
            this,
            &AbstractQGraphicsScene::onNodePositionUpdated);

    connect(&_graphModel,
            &AbstractGraphModel::nodeUpdated,
            this,
            &AbstractQGraphicsScene::onNodeUpdated);

    connect(this, &AbstractQGraphicsScene::nodeClicked, this, &AbstractQGraphicsScene::onNodeClicked);
    connect(&_graphModel, &AbstractGraphModel::modelReset, this, &AbstractQGraphicsScene::onModelReset);
}

AbstractGraphModel const &AbstractQGraphicsScene::graphModel() const
{
    return _graphModel;
}

AbstractGraphModel &AbstractQGraphicsScene::graphModel()
{
    return _graphModel;
}

AbstractNodeGeometry &AbstractQGraphicsScene::nodeGeometry()
{
    return *_nodeGeometry;
}

AbstractNodePainter &AbstractQGraphicsScene::nodePainter()
{
    return *_nodePainter;
}
AbstractConnectionPainter &AbstractQGraphicsScene::connectionPainter()
{
    return *_connectionPainter;
}


//AbstractNodeGraphicsObject *AbstractQGraphicsScene::nodeGraphicsObject(NodeId nodeId)
// {
//    AbstractNodeGraphicsObject *ngo = nullptr;
//    auto it = _nodeGraphicsObjects.find(nodeId);
//    if (it != _nodeGraphicsObjects.end()) {
//        ngo = it->second.get();
//    }

//    return ngo;
// }
//
//AbstractConnectionGraphicsObject *AbstractQGraphicsScene::connectionGraphicsObject(
//    ConnectionId connectionId)
//{
//    AbstractConnectionGraphicsObject *cgo = nullptr;
//    auto it = _connectionGraphicsObjects.find(connectionId);
//    if (it != _connectionGraphicsObjects.end()) {
//        cgo = it->second.get();
//    }
//
//    return cgo;
//}

Qt::Orientation AbstractQGraphicsScene::orientation() const
{
    return _orientation;
}
void AbstractQGraphicsScene::setOrientation(Qt::Orientation const orientation)
{
    if (_orientation != orientation) {
        _orientation = orientation;

        switch (_orientation) {
        case Qt::Horizontal:
            _nodeGeometry = std::make_unique<DefaultHorizontalNodeGeometry>(_graphModel);
            break;

        case Qt::Vertical:
            _nodeGeometry = std::make_unique<DefaultVerticalNodeGeometry>(_graphModel);
            break;
        }

        onModelReset();
    }
}

void AbstractQGraphicsScene::setNodeGeometry(std::unique_ptr<AbstractNodeGeometry> nodeGeometry)
{
    _nodeGeometry = std::move(nodeGeometry);
}


void AbstractQGraphicsScene::setNodePainter(std::unique_ptr<AbstractNodePainter> newPainter)
{
    _nodePainter = std::move(newPainter);
}
void AbstractQGraphicsScene::setConnectionPainter(std::unique_ptr<AbstractConnectionPainter> newPainter)
{
    _connectionPainter = std::move(newPainter);
}

//void AbstractQGraphicsScene::resetDraftConnection()
//{
//    _draftConnection.reset();
//}

QUndoStack &AbstractQGraphicsScene::undoStack()
{
    return *_undoStack;
}

void AbstractQGraphicsScene::clearScene()
{
    auto const &allNodeIds = graphModel().allNodeIds();

    for (auto nodeId : allNodeIds) {
        graphModel().deleteNode(nodeId);
    }
}


/*

void AbstractQGraphicsScene::onConnectionDeleted(ConnectionId const connectionId)
{
    auto it = _connectionGraphicsObjects.find(connectionId);
    if (it != _connectionGraphicsObjects.end()) {
        _connectionGraphicsObjects.erase(it);
    }

    // TODO: do we need it?
    if (_draftConnection && _draftConnection->connectionId() == connectionId) {
        _draftConnection.reset();
    }

    updateAttachedNodes(connectionId, PortType::Out);
    updateAttachedNodes(connectionId, PortType::In);

    Q_EMIT modified(this);
}


void AbstractQGraphicsScene::onNodeDeleted(NodeId const nodeId)
{
    auto it = _nodeGraphicsObjects.find(nodeId);
    if (it != _nodeGraphicsObjects.end()) {
        _nodeGraphicsObjects.erase(it);

        Q_EMIT modified(this);
    }
}

void AbstractQGraphicsScene::onNodePositionUpdated(NodeId const nodeId)
{
    auto node = nodeGraphicsObject(nodeId);
    if (node) {
        node->setPos(_graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>());
        node->update();
        _nodeDrag = true;
    }
}

void AbstractQGraphicsScene::onNodeUpdated(NodeId const nodeId)
{
    auto node = nodeGraphicsObject(nodeId);

    if (node) {
        node->setGeometryChanged();

        _nodeGeometry->recomputeSize(nodeId);

        node->update();
        node->moveConnections();
    }
}

void AbstractQGraphicsScene::onNodeClicked(NodeId const nodeId)
{
    if (_nodeDrag) {
        Q_EMIT nodeMoved(nodeId, _graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>());
        Q_EMIT modified(this);
    }
    _nodeDrag = false;
}

void AbstractQGraphicsScene::onModelReset()
{
    _connectionGraphicsObjects.clear();
    _nodeGraphicsObjects.clear();

    clear();

    traverseGraphAndPopulateGraphicsObjects();
}

*/
} // namespace QtNodes
