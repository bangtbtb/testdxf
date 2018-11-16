#ifndef DRW_DIMDIAMETRIC_H
#define DRW_DIMDIAMETRIC_H

#include "drw_dimension.h"
//! Class to handle radial dimension entity
/*!
*  Class to handle aligned, linear or rotated dimension entity
*  @author Rallaz
*/
class DRW_DimDiametric : public DRW_Dimension {
	SETENTFRIENDS
public:
	DRW_DimDiametric() {
		eType = DRW::DIMDIAMETRIC;
	}
	DRW_DimDiametric(const DRW_Dimension& d) : DRW_Dimension(d) {
		eType = DRW::DIMDIAMETRIC;
	}

	DRW_Coord getDiameter1Point() const { return getPt5(); }      /*!< First definition point for diameter, code 15, 25 & 35 */
	void setDiameter1Point(const DRW_Coord p) { setPt5(p); }
	DRW_Coord getDiameter2Point() const { return getDefPoint(); } /*!< Oposite point for diameter, code 10, 20 & 30 */
	void setDiameter2Point(const DRW_Coord p) { setDefPoint(p); }
	double getLeaderLength() const { return getRa40(); }          /*!< Leader length, code 40 */
	void setLeaderLength(const double d) { setRa40(d); }

protected:
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);
};



#endif
