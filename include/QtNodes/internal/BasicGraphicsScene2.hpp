#pragma once


#include <QObject>

#include "AbstractQGraphicsScene.hpp"


namespace QtNodes {


template<class NodeGraphicsObject, class ConnectionGraphicsObject>
class NODE_EDITOR_PUBLIC AbstractGraphicsScene : public AbstractQGraphicsScene
{
public:

    AbstractGraphicsScene(AbstractGraphModel &graphModel, QObject *parent = nullptr);

    AbstractGraphicsScene() = delete;
    ~AbstractGraphicsScene() = default;

    virtual Qt::Orientation orientation() const;
    virtual void setOrientation(Qt::Orientation const orientation);

    virtual QMenu *createSceneMenu(QPointF const scenePos);

    /// Slot called when the `connectionId` is erased form the AbstractGraphModel.
    virtual void onConnectionDeleted(ConnectionId const connectionId);

    /// Slot called when the `connectionId` is created in the AbstractGraphModel.
    virtual void onConnectionCreated(ConnectionId const connectionId) {
        _connectionGraphicsObjects[connectionId]
            = std::make_unique<ConnectionGraphicsObject>(*this, connectionId);

        updateAttachedNodes(connectionId, PortType::Out);
        updateAttachedNodes(connectionId, PortType::In);

        Q_EMIT modified(this);
    }

    virtual void onNodeCreated(NodeId const nodeId) {
        _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObject>(*this, nodeId);
        Q_EMIT modified(this);
    }

private:
    virtual void traverseGraphAndPopulateGraphicsObjects() {
        auto allNodeIds = _graphModel.allNodeIds();

        // First create all the nodes.
        for (NodeId const nodeId : allNodeIds) {
            _nodeGraphicsObjects[nodeId] = std::make_unique<NodeGraphicsObject>(*this, nodeId);
        }

        // Then for each node check output connections and insert them.
        for (NodeId const nodeId : allNodeIds) {
            auto nOutPorts = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

            for (PortIndex index = 0; index < nOutPorts; ++index) {
                auto const &outConnectionIds = _graphModel.connections(nodeId, PortType::Out, index);

                for (auto cid : outConnectionIds) {
                    _connectionGraphicsObjects[cid]
                        = std::make_unique<ConnectionGraphicsObject>(*this, cid);
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

};
}
