#include <cstdlib>
#include "drw_dimaligned.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"

#include "intern/drw_dbg.h"


bool DRW_DimAligned::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
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
	if (oType == 0x15)
		DRW_DBG("\n***************************** parsing dim linear *********************************************\n");
	else
		DRW_DBG("\n***************************** parsing dim aligned *********************************************\n");
	DRW_Coord pt = buf->get3BitDouble();
	setPt3(pt); //def1
	DRW_DBG("def1: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	pt = buf->get3BitDouble();
	setPt4(pt);
	DRW_DBG("\ndef2: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	pt = buf->get3BitDouble();
	setDefPoint(pt);
	DRW_DBG("\ndefPoint: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	setOb52(buf->getBitDouble());
	if (oType == 0x15)
		setAn50(buf->getBitDouble() * ARAD);
	else
		type |= 1;
	DRW_DBG("\n  type (70) final: "); DRW_DBG(type); DRW_DBG("\n");

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
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
