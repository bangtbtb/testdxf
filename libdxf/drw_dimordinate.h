#ifndef DRW_DIMORDINATE_H
#define DRW_DIMORDINATE_H

#include "drw_dimension.h"

//! Class to handle ordinate dimension entity
/*!
*  Class to handle ordinate dimension entity
*  @author Rallaz
*/
class DRW_DimOrdinate : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimOrdinate() {
		eType = DRW::DIMORDINATE;
	}
	DRW_DimOrdinate(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMORDINATE;
	}

	DRW_Coord getOriginPoint() const { return getDefPoint(); }   /*!< Origin definition point, code 10, 20 & 30 */
	void setOriginPoint(const DRW_Coord p) { setDefPoint(p); }
	DRW_Coord getFirstLine() const { return getPt3(); }          /*!< Feature location point, code 13, 23 & 33 */
	void setFirstLine(const DRW_Coord p) { setPt3(p); }
	DRW_Coord getSecondLine() const { return getPt4(); }         /*!< Leader end point, code 14, 24 & 34 */
	void setSecondLine(const DRW_Coord p) { setPt4(p); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};


#endif
