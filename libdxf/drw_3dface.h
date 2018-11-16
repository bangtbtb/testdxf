#ifndef DRW_3DFACE_H
#define DRW_3DFACE_H

#include "drw_trace.h"


//! Class to handle 3dface entity
/*!
*  Class to handle 3dface entity
*  @author Rallaz
*/
class DRW_3Dface : public DRW_Trace {
	SETENTFRIENDS
public:
	enum InvisibleEdgeFlags {
		NoEdge = 0x00,
		FirstEdge = 0x01,
		SecodEdge = 0x02,
		ThirdEdge = 0x04,
		FourthEdge = 0x08,
		AllEdges = 0x0F
	};

	DRW_3Dface() {
		eType = DRW::E3DFACE;
		invisibleflag = 0;
	}

	virtual void applyExtrusion() {}

	//! first corner in WCS
	const DRW_Coord & firstCorner() { return basePoint; }
	//! second corner in WCS
	const DRW_Coord & secondCorner() { return secPoint; }
	//! third corner in WCS
	const DRW_Coord & thirdCorner() { return thirdPoint; }
	//! fourth corner in WCS
	const DRW_Coord & fourthCorner() { return fourPoint; }
	//! edge visibility flags
	InvisibleEdgeFlags edgeFlags() { return (InvisibleEdgeFlags)invisibleflag; }

protected:
	//! interpret code in dxf reading process or dispatch to inherited class
	void parseCode(int code, dxfReader *reader);
	//! interpret dwg data (was already determined to be part of this object)
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	int invisibleflag;       /*!< invisible edge flag, code 70 */

};


#endif
