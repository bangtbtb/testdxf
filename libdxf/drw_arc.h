#ifndef DRW_ARC_H
#define DRW_ARC_H


#include "drw_circle.h"

//! Class to handle arc entity
/*!
*  Class to handle arc entity
*  @author Rallaz
*/
class DRW_Arc : public DRW_Circle {
	SETENTFRIENDS
public:
	DRW_Arc() {
		eType = DRW::ARC;
		isccw = 1;
	}

	virtual void applyExtrusion();

	//! center point in OCS
	const DRW_Coord & center() { return basePoint; }
	//! the radius of the circle
	double radius() { return radious; }
	//! start angle in radians
	double startAngle() { return staangle; }
	//! end angle in radians
	double endAngle() { return endangle; }
	//! thickness
	double thick() { return thickness; }
	//! extrusion
	const DRW_Coord & extrusion() { return extPoint; }

protected:
	//! interpret code in dxf reading process or dispatch to inherited class
	void parseCode(int code, dxfReader *reader);
	//! interpret dwg data (was already determined to be part of this object)
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	double staangle;            /*!< start angle, code 50 in radians*/
	double endangle;            /*!< end angle, code 51 in radians */
	int isccw;                  /*!< is counter clockwise arc?, only used in hatch, code 73 */
};

#endif
