#include <cstdlib>
#include "drw_dimradial.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


bool DRW_DimRadial::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
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
	DRW_DBG("\n***************************** parsing dim radial *********************************************\n");
	DRW_Coord pt = buf->get3BitDouble();
	setDefPoint(pt); //code 10
	DRW_DBG("defPoint: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	pt = buf->get3BitDouble();
	setPt5(pt); //center pt  code 15
	DRW_DBG("\ncenter point: "); DRW_DBGPT(pt.x, pt.y, pt.z);
	setRa40(buf->getBitDouble()); //leader length code 40
	DRW_DBG("\nleader length: "); DRW_DBG(getRa40());
	type |= 4;
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
