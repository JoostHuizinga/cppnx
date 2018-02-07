/*
 * CE_CommandSetZ.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Joost Huizinga
 */

#ifndef CX_COMMANDSETZ_H_
#define CX_COMMANDSETZ_H_

#include <QGraphicsItem>

#include "CE_Util.h"
#include "CE_Defines.h"
#include "CX_ComBase.h"
#include "CX_Debug.hpp"

class CommandSetZ: public ComBase {
public:
	CommandSetZ(QList<QGraphicsItem*> items, QList<qreal> z_values, QString text);
	CommandSetZ(QList<QGraphicsItem*> items, qreal z_value, QString text);
	virtual ~CommandSetZ();

	void init(QList<QGraphicsItem*>& items, QList<qreal> z_values, QString text);
	void undo();
	void redo();

	int id() const{return SET_Z_ID;}
//	bool mergeWith(const QUndoCommand *other);

private:
	typedef util::Triple<QGraphicsItem*, qreal, qreal> triple_t;
	QList<triple_t> _nodeZTriples;
};

#endif /* CX_COMMANDSETZ_H_ */
