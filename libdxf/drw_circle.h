
#ifndef DRW_CIRCLE_H
#define DRW_CIRCLE_H

#include "drw_point.h"

//! Class to handle circle entity
/*!
*  Class to handle circle entity
*  @author Rallaz
*/
class DRW_Circle : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Circle() {
		eType = DRW::CIRCLE;
	}

	virtual void applyExtrusion();

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	double radious;                 /*!< radius, code 40 */
};

#endif
