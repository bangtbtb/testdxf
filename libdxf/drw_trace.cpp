#include <cstdlib>
#include "drw_trace.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Trace::applyExtrusion() {
	if (haveExtrusion) {
		calculateAxis(extPoint);
		extrudePoint(extPoint, &basePoint);
		extrudePoint(extPoint, &secPoint);
		extrudePoint(extPoint, &thirdPoint);
		extrudePoint(extPoint, &fourPoint);
	}
}

void DRW_Trace::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 12:
		thirdPoint.x = reader->getDouble();
		break;
	case 22:
		thirdPoint.y = reader->getDouble();
		break;
	case 32:
		thirdPoint.z = reader->getDouble();
		break;
	case 13:
		fourPoint.x = reader->getDouble();
		break;
	case 23:
		fourPoint.y = reader->getDouble();
		break;
	case 33:
		fourPoint.z = reader->getDouble();
		break;
	default:
		DRW_Line::parseCode(code, reader);
		break;
	}
}

bool DRW_Trace::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing Trace *********************************************\n");

	thickness = buf->getThickness(version > DRW::AC1014);
	basePoint.z = buf->getBitDouble();
	basePoint.x = buf->getRawDouble();
	basePoint.y = buf->getRawDouble();
	secPoint.x = buf->getRawDouble();
	secPoint.y = buf->getRawDouble();
	secPoint.z = basePoint.z;
	thirdPoint.x = buf->getRawDouble();
	thirdPoint.y = buf->getRawDouble();
	thirdPoint.z = basePoint.z;
	fourPoint.x = buf->getRawDouble();
	fourPoint.y = buf->getRawDouble();
	fourPoint.z = basePoint.z;
	extPoint = buf->getExtrusion(version > DRW::AC1014);

	DRW_DBG(" - base "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	DRW_DBG("\n - sec "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z);
	DRW_DBG("\n - third "); DRW_DBGPT(thirdPoint.x, thirdPoint.y, thirdPoint.z);
	DRW_DBG("\n - fourth "); DRW_DBGPT(fourPoint.x, fourPoint.y, fourPoint.z);
	DRW_DBG("\n - extrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);
	DRW_DBG("\n - thickness: "); DRW_DBG(thickness); DRW_DBG("\n");

	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;

	/* CRC X --- */
	return buf->isGood();
}
