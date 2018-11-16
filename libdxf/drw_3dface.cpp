#include <cstdlib>
#include "drw_3dface.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_3Dface::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 70:
		invisibleflag = reader->getInt32();
		break;
	default:
		DRW_Trace::parseCode(code, reader);
		break;
	}
}

bool DRW_3Dface::parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(v, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing 3Dface *********************************************\n");

	if (v < DRW::AC1015) {// R13 & R14
		basePoint.x = buf->getBitDouble();
		basePoint.y = buf->getBitDouble();
		basePoint.z = buf->getBitDouble();
		secPoint.x = buf->getBitDouble();
		secPoint.y = buf->getBitDouble();
		secPoint.z = buf->getBitDouble();
		thirdPoint.x = buf->getBitDouble();
		thirdPoint.y = buf->getBitDouble();
		thirdPoint.z = buf->getBitDouble();
		fourPoint.x = buf->getBitDouble();
		fourPoint.y = buf->getBitDouble();
		fourPoint.z = buf->getBitDouble();
		invisibleflag = buf->getBitShort();
	}
	else { // 2000+
		bool has_no_flag = buf->getBit();
		bool z_is_zero = buf->getBit();
		basePoint.x = buf->getRawDouble();
		basePoint.y = buf->getRawDouble();
		basePoint.z = z_is_zero ? 0.0 : buf->getRawDouble();
		secPoint.x = buf->getDefaultDouble(basePoint.x);
		secPoint.y = buf->getDefaultDouble(basePoint.y);
		secPoint.z = buf->getDefaultDouble(basePoint.z);
		thirdPoint.x = buf->getDefaultDouble(secPoint.x);
		thirdPoint.y = buf->getDefaultDouble(secPoint.y);
		thirdPoint.z = buf->getDefaultDouble(secPoint.z);
		fourPoint.x = buf->getDefaultDouble(thirdPoint.x);
		fourPoint.y = buf->getDefaultDouble(thirdPoint.y);
		fourPoint.z = buf->getDefaultDouble(thirdPoint.z);
		invisibleflag = has_no_flag ? (int)NoEdge : buf->getBitShort();
	}
	drw_assert(invisibleflag >= NoEdge);
	drw_assert(invisibleflag <= AllEdges);

	DRW_DBG(" - base "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z); DRW_DBG("\n");
	DRW_DBG(" - sec "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z); DRW_DBG("\n");
	DRW_DBG(" - third "); DRW_DBGPT(thirdPoint.x, thirdPoint.y, thirdPoint.z); DRW_DBG("\n");
	DRW_DBG(" - fourth "); DRW_DBGPT(fourPoint.x, fourPoint.y, fourPoint.z); DRW_DBG("\n");
	DRW_DBG(" - Invisibility mask: "); DRW_DBG(invisibleflag); DRW_DBG("\n");

	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(v, buf);
	if (!ret)
		return ret;
	return buf->isGood();
}
