
#include "drw_line.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Line::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 11:
		secPoint.x = reader->getDouble();
		break;
	case 21:
		secPoint.y = reader->getDouble();
		break;
	case 31:
		secPoint.z = reader->getDouble();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

bool DRW_Line::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing line *********************************************\n");

	if (version < DRW::AC1015) {//14-
		basePoint.x = buf->getBitDouble();
		basePoint.y = buf->getBitDouble();
		basePoint.z = buf->getBitDouble();
		secPoint.x = buf->getBitDouble();
		secPoint.y = buf->getBitDouble();
		secPoint.z = buf->getBitDouble();
	}
	if (version > DRW::AC1014) {//2000+
		bool zIsZero = buf->getBit(); //B
		basePoint.x = buf->getRawDouble();//RD
		secPoint.x = buf->getDefaultDouble(basePoint.x);//DD
		basePoint.y = buf->getRawDouble();//RD
		secPoint.y = buf->getDefaultDouble(basePoint.y);//DD
		if (!zIsZero) {
			basePoint.z = buf->getRawDouble();//RD
			secPoint.z = buf->getDefaultDouble(basePoint.z);//DD
		}
	}
	DRW_DBG("start point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	DRW_DBG("\nend point: "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z);
	thickness = buf->getThickness(version > DRW::AC1014);//BD
	DRW_DBG("\nthickness: "); DRW_DBG(thickness);
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG(", Extrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z); DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
