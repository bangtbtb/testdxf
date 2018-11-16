#ifndef DRW_TRACE_H
#define DRW_TRACE_H

#include "drw_line.h"

//! Class to handle trace entity
/*!
*  Class to handle trace entity
*  @author Rallaz
*/
class DRW_Trace : public DRW_Line {
	SETENTFRIENDS
public:
	DRW_Trace() {
		eType = DRW::TRACE;
		thirdPoint.z = 0;
		fourPoint.z = 0;
	}

	virtual void applyExtrusion();

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	DRW_Coord thirdPoint;        /*!< third point, code 12, 22 & 32 */
	DRW_Coord fourPoint;        /*!< four point, code 13, 23 & 33 */
};


#endif
