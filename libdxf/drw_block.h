#ifndef DRW_BLOCK_H
#define DRW_BLOCK_H

#include "drw_point.h"

//! Class to handle block entries
/*!
*  Class to handle block entries
*  @author Rallaz
*/
class DRW_Block : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Block() {
		eType = DRW::BLOCK;
		layer = "0";
		flags = 0;
		name = "*U0";
		isEnd = false;
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	UTF8STRING name;             /*!< block name, code 2 */
	int flags;                   /*!< block type, code 70 */
private:
	bool isEnd; //for dwg parsing
};

#endif
