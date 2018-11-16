
#ifndef DRW_POINT_H
#define DRW_POINT_H

#include "drw_entity.h"
//! Class to handle point entity
/*!
*  Class to handle point entity
*  @author Rallaz
*/
class DRW_Point : public DRW_Entity {
	SETENTFRIENDS
public:
	DRW_Point() {
		eType = DRW::POINT;
		basePoint.z = extPoint.x = extPoint.y = 0;
		extPoint.z = 1;
		thickness = 0;
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	double thickness;         /*!< thickness, code 39 */
	DRW_Coord basePoint;      /*!<  base point, code 10, 20 & 30 */
	DRW_Coord extPoint;       /*!<  Dir extrusion normal vector, code 210, 220 & 230 */
							  // TNick: we're not handling code 50 - Angle of the X axis for
							  // the UCS in effect when the point was drawn
};

#endif
