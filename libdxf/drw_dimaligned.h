#ifndef DRW_DIMALIGNED_H
#define DRW_DIMALIGNED_H

#include "drw_dimension.h"


//! Class to handle  aligned dimension entity
/*!
*  Class to handle aligned dimension entity
*  @author Rallaz
*/
class DRW_DimAligned : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimAligned() {
		eType = DRW::DIMALIGNED;
	}
	DRW_DimAligned(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMALIGNED;
	}

	DRW_Coord getClonepoint() const { return getPt2(); }      /*!< Insertion for clones (Baseline & Continue), 12, 22 & 32 */
	void setClonePoint(DRW_Coord c) { setPt2(c); }

	DRW_Coord getDimPoint() const { return getDefPoint(); }   /*!< dim line location point, code 10, 20 & 30 */
	void setDimPoint(const DRW_Coord p) { setDefPoint(p); }
	DRW_Coord getDef1Point() const { return getPt3(); }       /*!< Definition point 1, code 13, 23 & 33 */
	void setDef1Point(const DRW_Coord p) { setPt3(p); }
	DRW_Coord getDef2Point() const { return getPt4(); }       /*!< Definition point 2, code 14, 24 & 34 */
	void setDef2Point(const DRW_Coord p) { setPt4(p); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};


#endif
