#ifndef DRW_LINE_H
#define DRW_LINE_H

#include "drw_point.h"



//! Class to handle line entity
/*!
*  Class to handle line entity
*  @author Rallaz
*/
class DRW_Line : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Line() {
		eType = DRW::LINE;
		secPoint.z = 0;
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	DRW_Coord secPoint;        /*!< second point, code 11, 21 & 31 */
};


#endif
