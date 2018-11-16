
#include <cstdlib>
#include "drw_circle.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Circle::applyExtrusion() {
	if (haveExtrusion) {
		//NOTE: Commenting these out causes the the arcs being tested to be located
		//on the other side of the y axis (all x dimensions are negated).
		calculateAxis(extPoint);
		extrudePoint(extPoint, &basePoint);
	}
}

void DRW_Circle::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 40:
		radious = reader->getDouble();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

bool DRW_Circle::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing circle *********************************************\n");

	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	DRW_DBG("center: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	radious = buf->getBitDouble();
	DRW_DBG("\nradius: "); DRW_DBG(radious);

	thickness = buf->getThickness(version > DRW::AC1014);
	DRW_DBG(" thickness: "); DRW_DBG(thickness);
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG("\nextrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z); DRW_DBG("\n");

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
