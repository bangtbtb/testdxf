#ifndef DRW_DIMANGULAR_H
#define DRW_DIMANGULAR_H

#include "drw_dimension.h"
//! Class to handle angular dimension entity
/*!
*  Class to handle angular dimension entity
*  @author Rallaz
*/
class DRW_DimAngular : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimAngular() {
		eType = DRW::DIMANGULAR;
	}
	DRW_DimAngular(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMANGULAR;
	}

	DRW_Coord getFirstLine1() const { return getPt3(); }       /*!< Definition point line 1-1, code 13, 23 & 33 */
	void setFirstLine1(const DRW_Coord p) { setPt3(p); }
	DRW_Coord getFirstLine2() const { return getPt4(); }       /*!< Definition point line 1-2, code 14, 24 & 34 */
	void setFirstLine2(const DRW_Coord p) { setPt4(p); }
	DRW_Coord getSecondLine1() const { return getPt5(); }      /*!< Definition point line 2-1, code 15, 25 & 35 */
	void setSecondLine1(const DRW_Coord p) { setPt5(p); }
	DRW_Coord getSecondLine2() const { return getDefPoint(); } /*!< Definition point line 2-2, code 10, 20 & 30 */
	void setSecondLine2(const DRW_Coord p) { setDefPoint(p); }
	DRW_Coord getDimPoint() const { return getPt6(); }         /*!< Dimension definition point, code 16, 26 & 36 */
	void setDimPoint(const DRW_Coord p) { setPt6(p); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};


#endif
