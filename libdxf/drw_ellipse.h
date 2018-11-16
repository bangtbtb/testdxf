#ifndef DRW_ELLIPSE_H
#define DRW_ELLIPSE_H


#include "drw_line.h"
//! Class to handle ellipse entity
/*!
*  Class to handle ellipse and elliptic arc entity
*  Note: start/end parameter are in radians for ellipse entity but
*  for hatch boundary are in degrees
*  @author Rallaz
*/
class DRW_Ellipse : public DRW_Line {
	SETENTFRIENDS
public:
	DRW_Ellipse() {
		eType = DRW::ELLIPSE;
		isccw = 1;
	}

	void toPolyline(DRW_Polyline *pol, int parts = 128);
	virtual void applyExtrusion();

protected:
	//! interpret code in dxf reading process or dispatch to inherited class
	void parseCode(int code, dxfReader *reader);
	//! interpret dwg data (was already determined to be part of this object)
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

private:
	void correctAxis();

public:
	double ratio;        /*!< ratio, code 40 */
	double staparam;     /*!< start parameter, code 41, 0.0 for full ellipse*/
	double endparam;     /*!< end parameter, code 42, 2*PI for full ellipse */
	int isccw;           /*!< is counter clockwise arc?, only used in hatch, code 73 */
};

#endif
