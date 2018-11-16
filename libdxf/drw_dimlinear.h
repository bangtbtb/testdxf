#ifndef DRW_DIMLINEAR_H
#define DRW_DIMLINEAR_H

#include "drw_dimaligned.h"


//! Class to handle  linear or rotated dimension entity
/*!
*  Class to handle linear or rotated dimension entity
*  @author Rallaz
*/
class DRW_DimLinear : public DRW_DimAligned {
public:
	DRW_DimLinear() {
		eType = DRW::DIMLINEAR;
	}
	DRW_DimLinear(const DRW_Dimension& d) : DRW_DimAligned(d) {
		eType = DRW::DIMLINEAR;
	}

	double getAngle() const { return getAn50(); }          /*!< Angle of rotated, horizontal, or vertical dimensions, code 50 */
	void setAngle(const double d) { setAn50(d); }
	double getOblique() const { return getOb52(); }      /*!< oblique angle, code 52 */
	void setOblique(const double d) { setOb52(d); }
};



#endif
