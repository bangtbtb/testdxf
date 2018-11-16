#ifndef DRW_SOLID_H
#define DRW_SOLID_H

#include "drw_trace.h"

//! Class to handle solid entity
/*!
*  Class to handle solid entity
*  @author Rallaz
*/
class DRW_Solid : public DRW_Trace {
	SETENTFRIENDS
public:
	DRW_Solid() {
		eType = DRW::SOLID;
	}

protected:
	//! interpret code in dxf reading process or dispatch to inherited class
	//void parseCode(int code, dxfReader *reader);
	//! interpret dwg data (was already determined to be part of this object)
	//virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	//! first corner (2D)
	const DRW_Coord & firstCorner() { return basePoint; }
	//! second corner (2D)
	const DRW_Coord & secondCorner() { return secPoint; }
	//! third corner (2D)
	const DRW_Coord & thirdCorner() { return thirdPoint; }
	//! fourth corner (2D)
	const DRW_Coord & fourthCorner() { return thirdPoint; }
	//! thickness
	double thick() { return thickness; }
	//! elevation
	double elevation() { return basePoint.z; }
	//! extrusion
	const DRW_Coord & extrusion() { return extPoint; }

};


#endif
