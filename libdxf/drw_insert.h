#ifndef DRW_INSERT_H
#define DRW_INSERT_H

#include "drw_point.h"

//! Class to handle insert entries
/*!
*  Class to handle insert entries
*  @author Rallaz
*/
class DRW_Insert : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Insert() {
		eType = DRW::INSERT;
		xscale = 1;
		yscale = 1;
		zscale = 1;
		angle = 0;
		colcount = 1;
		rowcount = 1;
		colspace = 0;
		rowspace = 0;
	}

	virtual void applyExtrusion() { DRW_Point::applyExtrusion(); }

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	UTF8STRING name;         /*!< block name, code 2 */
	double xscale;           /*!< x scale factor, code 41 */
	double yscale;           /*!< y scale factor, code 42 */
	double zscale;           /*!< z scale factor, code 43 */
	double angle;            /*!< rotation angle in radians, code 50 */
	int colcount;            /*!< column count, code 70 */
	int rowcount;            /*!< row count, code 71 */
	double colspace;         /*!< column space, code 44 */
	double rowspace;         /*!< row space, code 45 */
public: //only for read dwg
	dwgHandle blockRecH;
	dwgHandle seqendH; //RLZ: on implement attrib remove this handle from obj list (see pline/vertex code)
};


#endif
