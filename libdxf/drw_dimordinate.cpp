#include <cstdlib>
#include "drw_dimordinate.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


bool DRW_DimOrdinate::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	ret = DRW_Dimension::parseDwg(version, buf, sBuf);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing dim ordinate *********************************************\n");
	DRW_Coord pt = buf->get3BitDouble();
	setDefPoint(pt);
	DRW_DBG("defPoint: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	pt = buf->get3BitDouble();
	setPt3(pt); //def1
	DRW_DBG("\ndef1: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	pt = buf->get3BitDouble();
	setPt4(pt);
	DRW_DBG("\ndef2: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	duint8 type2 = buf->getRawChar8();//RLZ: correct this
	DRW_DBG("type2 (70) read: "); DRW_DBG(type2);
	type = (type2 & 1) ? type | 0x80 : type & 0xBF; //set bit 6
	DRW_DBG(" type (70) set: "); DRW_DBG(type);
	type |= 6;
	DRW_DBG("\n  type (70) final: "); DRW_DBG(type);

	ret = DRW_Entity::parseDwgEntHandle(version, buf); DRW_DBG("\n");
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	if (!ret)
		return ret;
	dimStyleH = buf->getHandle();
	DRW_DBG("dim style Handle: "); DRW_DBGHL(dimStyleH.code, dimStyleH.size, dimStyleH.ref); DRW_DBG("\n");
	blockH = buf->getHandle(); /* H 7 STYLE (hard pointer) */
	DRW_DBG("anon block Handle: "); DRW_DBGHL(blockH.code, blockH.size, blockH.ref); DRW_DBG("\n");
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	//    RS crc;   //RS */
	return buf->isGood();
}
