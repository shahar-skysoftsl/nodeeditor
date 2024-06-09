#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H


#include <QGraphicsView>

#include "AbstractNodeGeometry.hpp"
#include "AbstractQGraphicsScene.hpp"
#include "AbstractNodeGraphicsObject.hpp"
#include "AbstractConnectionGraphicsObject.hpp"
#include "ConnectionIdUtils.hpp"
//#include "DefaultHorizontalNodeGeometry.hpp"
//#include "DefaultVerticalNodeGeometry.hpp"

namespace QtNodes {

template<class NodeGraphicsObjectType, class ConnectionGraphicsObjectType>
class NODE_EDITOR_EXPORT GraphicsScene : public AbstractQGraphicsScene
{

    static_assert(std::is_base_of<AbstractNodeGraphicsObject, NodeGraphicsObjectType>::value, "NodeGraphicsObjectType must be derived from AbstractNodeGraphicsObject");
    static_assert(std::is_base_of<AbstractConnectionGraphicsObject, ConnectionGraphicsObjectType>::value, "ConnectionGraphicsObjectType must be derived from AbstractNodeGraphicsObject");

public:

    GraphicsScene(AbstractGraphModel &graphModel, QObject *parent = nullptr):
       AbstractQGraphicsScene(graphModel, parent) {
    }

    GraphicsScene() = delete;
    ~GraphicsScene() = default;

    virtual QMenu *createSceneMenu(QPointF const scenePos) { return nullptr; }

    AbstractNodeGraphicsObject *nodeGraphicsObject(NodeId nodeId) {
        NodeGraphicsObjectType *ngo = nullptr;

        auto it = _nodeGraphicsObjects.find(nodeId);
        if (it != _nodeGraphicsObjects.end()) {
            ngo = it->second.get();
        }

        return ngo;
     }
    AbstractConnectionGraphicsObject *connectionGraphicsObject(ConnectionId connectionId) {
        ConnectionGraphicsObjectType *cgo = nullptr;
            auto it = _connectionGraphicsObjects.find(connectionId);
            if (it != _connectionGraphicsObjects.end()) {
                cgo = it->second.get();
            }

            return cgo;
    }

    const std::unique_ptr<AbstractConnectionGraphicsObject> &makeDraftConnection(const ConnectionId incompleteConnectionId) {
        _draftConnection = std::make_unique<ConnectionGraphicsObjectType>(*this, incompleteConnectionId);

        _draftConnection->grabMouse();

        return _draftConnection;
    }

    virtual void resetDraftConnection() { _draftConnection.reset(); }

    /// Slot called when the `connectionId` is created in the AbstractGraphModel.
    virtual void onConnectionCreated(ConnectionId const connectionId) {
        _connectionGraphicsObjects[connectionId]
            = std::make_unique<ConnectionGraphicsObjectType>(*this, connectionId);

        updateAttachedNodes(connectionId, PortType::Out);
        updateAttachedNodes(connectionId, PortType::In);

        Q_EMIT modified(this);
    }
    /// Slot called when the `connectionId` is erased form the AbstractGraphModel.
    virtual void onConnectionDeleted(ConnectionId const connectionId) {
        _connectionGraphicsObjects.erase(connectionId);
        /*auto it = _connectionGraphicsObjects.find(connectionId);
        if (it != _connectionGraphicsObjects.end()) {
            _connectionGraphicsObjects.erase(it);
        }*/

        // TODO: do we need it?
        /*if (_draftConnection && _draftConnection->connectionId() == connectionId) {
            _draftConnection.reset();
        }*/

        /*updateAttachedNodes(connectionId, PortType::Out);
        updateAttachedNodes(connectionId, PortType::In);

        Q_EMIT modified(this);*/
    }

    virtual void onConnectionsDeleted(QList<ConnectionId> const connections)
    {
        for (auto &connectionId : connections) {
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
        }

        Q_EMIT modified(this);
    }

    virtual void onNodeCreated(NodeId const nodeId) {
        _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObjectType>(*this, nodeId);
        Q_EMIT modified(this);
    }

    virtual void onNodeDeleted(const NodeId nodeId) {
        _nodeGraphicsObjects.erase(nodeId);

        //Q_EMIT modified(this);

        /*auto it = _nodeGraphicsObjects.find(nodeId);
        if (it != _nodeGraphicsObjects.end()) {
            _nodeGraphicsObjects.erase(it);

            
        }*/
    }

    virtual void onNodesDeleted(QList<NodeId> const nodes) { 
        
        for (auto &nodeId : nodes) {
            auto it = _nodeGraphicsObjects.find(nodeId);
            if (it != _nodeGraphicsObjects.end()) {
                //deleteG.addToGroup(it);
                _nodeGraphicsObjects.erase(it);
            }
        }

        Q_EMIT modified(this);
    }


    virtual void onNodePositionUpdated(const NodeId nodeId) {
        auto node = nodeGraphicsObject(nodeId);
        if (node) {
            node->setPos(_graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>());
            node->update();
            _nodeDrag = true;
        }
    }
    virtual void onNodeUpdated(const NodeId nodeId) {
        auto node = nodeGraphicsObject(nodeId);

        if (node) {
            node->setGeometryChanged();

            _nodeGeometry->recomputeSize(nodeId);

            node->update();
            node->moveConnections();
        }
    }
    virtual void onNodeClicked(const NodeId nodeId) {
        if (_nodeDrag) {
            Q_EMIT nodeMoved(nodeId, _graphModel.nodeData(nodeId, NodeRole::Position).value<QPointF>());
            Q_EMIT modified(this);
        }
        _nodeDrag = false;
    }
    virtual void onModelReset() {
        _connectionGraphicsObjects.clear();
       _nodeGraphicsObjects.clear();

       clear();

       traverseGraphAndPopulateGraphicsObjects();
    }

protected:
    virtual void traverseGraphAndPopulateGraphicsObjects() {
        auto allNodeIds = _graphModel.allNodeIds();

        // First create all the nodes.
        for (NodeId const nodeId : allNodeIds) {
            _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObjectType>(*this, nodeId);
        }

        // Then for each node check output connections and insert them.
        for (NodeId const nodeId : allNodeIds) {
            auto nOutPorts = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

            for (PortIndex index = 0; index < nOutPorts; ++index) {
                auto const &outConnectionIds = _graphModel.connections(nodeId, PortType::Out, index);

                for (auto cid : outConnectionIds) {
                    _connectionGraphicsObjects[cid]
                        = std::make_unique<ConnectionGraphicsObjectType>(*this, cid);
                }
            }
        }
    }

    /// Redraws adjacent nodes for given `connectionId`
    virtual void updateAttachedNodes(ConnectionId const connectionId, PortType const portType) {
        auto node = nodeGraphicsObject(getNodeId(portType, connectionId));
        if (node) {
            node->update();
        }
    }

    using UniqueNodeGraphicsObject = std::unique_ptr<NodeGraphicsObjectType>;
    using UniqueConnectionGraphicsObject = std::unique_ptr<ConnectionGraphicsObjectType>;


    //QMap<NodeId, UniqueNodeGraphicsObject> _nodeGraphicsObjects2;
    std::unordered_map<NodeId, UniqueNodeGraphicsObject> _nodeGraphicsObjects;

    //QMap<NodeId, UniqueConnectionGraphicsObject> _connectionGraphicsObjects2;
    std::unordered_map<ConnectionId, UniqueConnectionGraphicsObject> _connectionGraphicsObjects;

    std::unique_ptr<AbstractConnectionGraphicsObject> _draftConnection;

    // AbstractQGraphicsScene interface
public:

};

};


#endif // GRAPHICSSCENE_H
