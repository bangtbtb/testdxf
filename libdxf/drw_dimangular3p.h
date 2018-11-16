#ifndef DRW_DIMANGULAR3P_H
#define DRW_DIMANGULAR3P_H

#include "drw_dimension.h"

//! Class to handle angular 3p dimension entity
/*!
*  Class to handle angular 3p dimension entity
*  @author Rallaz
*/
class DRW_DimAngular3p : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimAngular3p() {
		eType = DRW::DIMANGULAR3P;
	}
	DRW_DimAngular3p(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMANGULAR3P;
	}

	DRW_Coord getFirstLine() const { return getPt3(); }       /*!< Definition point line 1, code 13, 23 & 33 */
	void setFirstLine(const DRW_Coord p) { setPt3(p); }
	DRW_Coord getSecondLine() const { return getPt4(); }       /*!< Definition point line 2, code 14, 24 & 34 */
	void setSecondLine(const DRW_Coord p) { setPt4(p); }
	DRW_Coord getVertexPoint() const { return getPt5(); }      /*!< Vertex point, code 15, 25 & 35 */
	void SetVertexPoint(const DRW_Coord p) { setPt5(p); }
	DRW_Coord getDimPoint() const { return getDefPoint(); }    /*!< Dimension definition point, code 10, 20 & 30 */
	void setDimPoint(const DRW_Coord p) { setDefPoint(p); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};


#endif
