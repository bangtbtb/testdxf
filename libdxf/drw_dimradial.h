#ifndef DRW_DIMRADIAL_H
#define DRW_DIMRADIAL_H

#include "drw_dimension.h"

//! Class to handle radial dimension entity
/*!
*  Class to handle aligned, linear or rotated dimension entity
*  @author Rallaz
*/
class DRW_DimRadial : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimRadial() {
		eType = DRW::DIMRADIAL;
	}
	DRW_DimRadial(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMRADIAL;
	}

	DRW_Coord getCenterPoint() const { return getDefPoint(); }   /*!< center point, code 10, 20 & 30 */
	void setCenterPoint(const DRW_Coord p) { setDefPoint(p); }
	DRW_Coord getDiameterPoint() const { return getPt5(); }      /*!< Definition point for radius, code 15, 25 & 35 */
	void setDiameterPoint(const DRW_Coord p) { setPt5(p); }
	double getLeaderLength() const { return getRa40(); }         /*!< Leader length, code 40 */
	void setLeaderLength(const double d) { setRa40(d); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};


#endif
