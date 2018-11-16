
#ifndef DRW_XLINE_H
#define DRW_XLINE_H

#include "drw_ray.h"

//! Class to handle xline entity
/*!
*  Class to handle xline entity
*  @author Rallaz
*/
class DRW_Xline : public DRW_Ray {
public:
	DRW_Xline() {
		eType = DRW::XLINE;
	}
};

#endif
