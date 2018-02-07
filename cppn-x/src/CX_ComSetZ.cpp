/*
 * CE_CommandSetPos.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: Joost Huizinga
 */

//Local includes
#include "CX_ComSetZ.hpp"

CommandSetZ::CommandSetZ(QList<QGraphicsItem*> items, QList<qreal> z_values, QString text) {
    dbg::trace trace("commandsetz", DBG_HERE);
    dbg::out(dbg::info, "commandsetz") << "Items passed: " << items.size() << std::endl;
	init(items, z_values, text);
}

CommandSetZ::CommandSetZ(QList<QGraphicsItem*> items, qreal z_value, QString text) {
    dbg::trace trace("commandsetz", DBG_HERE);
    dbg::out(dbg::info, "commandsetz") << "Items passed: " << items.size() << std::endl;
    QList<qreal> z_values;
    z_values.reserve(items.size());
    for(int i=0; i<items.size(); ++i){
    	z_values.append(z_value);
    }
	init(items, z_values, text);
}

//CommandSetZ::CommandSetPos(QList<Node*> nodes) {
//    dbg::trace trace("commandsetz", DBG_HERE);
//    dbg::out(dbg::info, "commandsetz") << "Nodes passed: " << nodes.size() << std::endl;
//    QList<MovableObject*> objects;
//    foreach(MovableObject* object, nodes){
//        dbg::out(dbg::info, "commandsetz") << "Attempting to append object: " << object << std::endl;
//        if(object){
//            objects.append(object);
//        }
//    }
//    init(objects);
//}
//
//CommandSetZ::CommandSetPos(QList<MovableObject*> objects){
//    dbg::trace trace("commandsetz", DBG_HERE);
//    dbg::out(dbg::info, "commandsetz") << "Objects passed: " << objects.size() << std::endl;
//    init(objects);
//}

CommandSetZ::~CommandSetZ() {
    dbg::trace trace("commandsetz", DBG_HERE);
}

void CommandSetZ::init(QList<QGraphicsItem*>& items, QList<qreal> z_values, QString text){
    dbg::trace trace("commandsetz", DBG_HERE);
    for(int i=0; i<items.size(); ++i){
    	_nodeZTriples.append(triple_t(items[i], items[i]->zValue(), z_values[i]));
    }
    setText(text);
}

void CommandSetZ::undo(){
    dbg::trace trace("commandsetz", DBG_HERE);
	foreach(triple_t triple, _nodeZTriples){
		triple.first->setZValue(triple.second);
	}
}

void CommandSetZ::redo(){
    dbg::trace trace("commandsetz", DBG_HERE);
	foreach(triple_t triple, _nodeZTriples){
		triple.first->setZValue(triple.third);
	}
}

//bool CommandSetZ::mergeWith(const QUndoCommand *other){
//    dbg::trace trace("commandsetz", DBG_HERE);
//    return false;
//}
