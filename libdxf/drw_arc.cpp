#include <cstdlib>
#include "drw_arc.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Arc::applyExtrusion() {
	DRW_Circle::applyExtrusion();

	if (haveExtrusion) {
		// If the extrusion vector has a z value less than 0, the angles for the arc
		// have to be mirrored since DXF files use the right hand rule.
		// Note that the following code only handles the special case where there is a 2D
		// drawing with the z axis heading into the paper (or rather screen). An arbitrary
		// extrusion axis (with x and y values greater than 1/64) may still have issues.
		if (fabs(extPoint.x) < 0.015625 && fabs(extPoint.y) < 0.015625 && extPoint.z < 0.0) {
			staangle = M_PI - staangle;
			endangle = M_PI - endangle;

			double temp = staangle;
			staangle = endangle;
			endangle = temp;
		}
	}
}

void DRW_Arc::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 50:
		staangle = reader->getDouble() / ARAD;
		break;
	case 51:
		endangle = reader->getDouble() / ARAD;
		break;
	default:
		DRW_Circle::parseCode(code, reader);
		break;
	}
}

bool DRW_Arc::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing circle arc *********************************************\n");

	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	DRW_DBG("center point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);

	radious = buf->getBitDouble();
	DRW_DBG("\nradius: "); DRW_DBG(radious);
	thickness = buf->getThickness(version > DRW::AC1014);
	DRW_DBG(" thickness: "); DRW_DBG(thickness);
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG("\nextrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);
	staangle = buf->getBitDouble();
	DRW_DBG("\nstart angle: "); DRW_DBG(staangle);
	endangle = buf->getBitDouble();
	DRW_DBG(" end angle: "); DRW_DBG(endangle); DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	return buf->isGood();
}
