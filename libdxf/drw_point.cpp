
#include "drw_point.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Point::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 10:
		basePoint.x = reader->getDouble();
		break;
	case 20:
		basePoint.y = reader->getDouble();
		break;
	case 30:
		basePoint.z = reader->getDouble();
		break;
	case 39:
		thickness = reader->getDouble();
		break;
	case 210:
		haveExtrusion = true;
		extPoint.x = reader->getDouble();
		break;
	case 220:
		extPoint.y = reader->getDouble();
		break;
	case 230:
		extPoint.z = reader->getDouble();
		break;
	default:
		DRW_Entity::parseCode(code, reader);
		break;
	}
}

bool DRW_Point::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing point *********************************************\n");

	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	DRW_DBG("point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	thickness = buf->getThickness(version > DRW::AC1014);//BD
	DRW_DBG("\nthickness: "); DRW_DBG(thickness);
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG(", Extrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);

	double x_axis = buf->getBitDouble();//BD
	DRW_DBG("\n  x_axis: "); DRW_DBG(x_axis); DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */

	return buf->isGood();
}
