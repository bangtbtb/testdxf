
#include <cstdlib>
#include "drw_ray.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"




bool DRW_Ray::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing ray/xline *********************************************\n");
	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	secPoint.x = buf->getBitDouble();
	secPoint.y = buf->getBitDouble();
	secPoint.z = buf->getBitDouble();
	DRW_DBG("start point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	DRW_DBG("\nvector: "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z);
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}

