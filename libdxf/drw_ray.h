#ifndef DRW_RAY_H
#define DRW_RAY_H

#include "drw_line.h"


//! Class to handle ray entity
/*!
*  Class to handle ray entity
*  @author Rallaz
*/
class DRW_Ray : public DRW_Line {
	SETENTFRIENDS
public:
	DRW_Ray() {
		eType = DRW::RAY;
	}
protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};

#endif
