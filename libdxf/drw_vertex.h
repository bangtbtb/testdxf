#ifndef DRW_VERTEX_H
#define DRW_VERTEX_H


#include "drw_point.h"

//! Class to handle vertex
/*!
*  Class to handle vertex  for polyline entity
*  @author Rallaz
*/
class DRW_Vertex : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Vertex() {
		eType = DRW::VERTEX;
		stawidth = endwidth = bulge = 0;
		vindex1 = vindex2 = vindex3 = vindex4 = 0;
		flags = identifier = 0;
	}
	DRW_Vertex(double sx, double sy, double sz, double b) {
		stawidth = endwidth = 0;
		vindex1 = vindex2 = vindex3 = vindex4 = 0;
		flags = identifier = 0;
		basePoint.x = sx;
		basePoint.y = sy;
		basePoint.z = sz;
		bulge = b;
	}

protected:
	void parseCode(int code, dxfReader *reader);
	bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0, double el = 0);
	virtual bool parseDwg(DRW::Version version, dwgBuffer* buf, duint32 bs = 0) {
		DRW_UNUSED(version); DRW_UNUSED(buf); DRW_UNUSED(bs); return true;
	}

public:
	double stawidth;          /*!< Start width, code 40 */
	double endwidth;          /*!< End width, code 41 */
	double bulge;             /*!< bulge, code 42 */

	int flags;                 /*!< vertex flag, code 70, default 0 */
	double tgdir;           /*!< curve fit tangent direction, code 50 */
	int vindex1;             /*!< polyface mesh vertex index, code 71, default 0 */
	int vindex2;             /*!< polyface mesh vertex index, code 72, default 0 */
	int vindex3;             /*!< polyface mesh vertex index, code 73, default 0 */
	int vindex4;             /*!< polyface mesh vertex index, code 74, default 0 */
	int identifier;           /*!< vertex identifier, code 91, default 0 */
};

#endif
