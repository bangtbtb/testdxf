#include <cstdlib>
#include "drw_insert.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Insert::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 2:
		name = reader->getUtf8String();
		break;
	case 41:
		xscale = reader->getDouble();
		break;
	case 42:
		yscale = reader->getDouble();
		break;
	case 43:
		zscale = reader->getDouble();
		break;
	case 50:
		angle = reader->getDouble();
		angle = angle / ARAD; //convert to radian
		break;
	case 70:
		colcount = reader->getInt32();
		break;
	case 71:
		rowcount = reader->getInt32();
		break;
	case 44:
		colspace = reader->getDouble();
		break;
	case 45:
		rowspace = reader->getDouble();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

bool DRW_Insert::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dint32 objCount = 0;
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n************************** parsing insert/minsert *****************************************\n");
	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	DRW_DBG("insertion point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z); DRW_DBG("\n");
	if (version < DRW::AC1015) {//14-
		xscale = buf->getBitDouble();
		yscale = buf->getBitDouble();
		zscale = buf->getBitDouble();
	}
	else {
		duint8 dataFlags = buf->get2Bits();
		if (dataFlags == 3) {
			//none default value 1,1,1
		}
		else if (dataFlags == 1) { //x default value 1, y & z can be x value
			yscale = buf->getDefaultDouble(xscale);
			zscale = buf->getDefaultDouble(xscale);
		}
		else if (dataFlags == 2) {
			xscale = buf->getRawDouble();
			yscale = zscale = xscale;
		}
		else { //dataFlags == 0
			xscale = buf->getRawDouble();
			yscale = buf->getDefaultDouble(xscale);
			zscale = buf->getDefaultDouble(xscale);
		}
	}
	angle = buf->getBitDouble();
	DRW_DBG("scale : "); DRW_DBGPT(xscale, yscale, zscale); DRW_DBG(", angle: "); DRW_DBG(angle);
	extPoint = buf->getExtrusion(false); //3BD R14 style
	DRW_DBG("\nextrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);

	bool hasAttrib = buf->getBit();
	DRW_DBG("   has Attrib: "); DRW_DBG(hasAttrib);

	if (hasAttrib && version > DRW::AC1015) {//2004+
		objCount = buf->getBitLong();
		DRW_UNUSED(objCount);
		DRW_DBG("   objCount: "); DRW_DBG(objCount); DRW_DBG("\n");
	}
	if (oType == 8) {//entity are minsert
		colcount = buf->getBitShort();
		rowcount = buf->getBitShort();
		colspace = buf->getBitDouble();
		rowspace = buf->getBitDouble();
	}
	DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	blockRecH = buf->getHandle(); /* H 2 BLOCK HEADER (hard pointer) */
	DRW_DBG("BLOCK HEADER Handle: "); DRW_DBGHL(blockRecH.code, blockRecH.size, blockRecH.ref); DRW_DBG("\n");
	DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	/*attribs follows*/
	if (hasAttrib) {
		if (version < DRW::AC1018) {//2000-
			dwgHandle attH = buf->getHandle(); /* H 2 BLOCK HEADER (hard pointer) */
			DRW_DBG("first attrib Handle: "); DRW_DBGHL(attH.code, attH.size, attH.ref); DRW_DBG("\n");
			attH = buf->getHandle(); /* H 2 BLOCK HEADER (hard pointer) */
			DRW_DBG("second attrib Handle: "); DRW_DBGHL(attH.code, attH.size, attH.ref); DRW_DBG("\n");
		}
		else {
			for (duint8 i = 0; i < objCount; ++i) {
				dwgHandle attH = buf->getHandle(); /* H 2 BLOCK HEADER (hard pointer) */
				DRW_DBG("attrib Handle #"); DRW_DBG(i); DRW_DBG(": "); DRW_DBGHL(attH.code, attH.size, attH.ref); DRW_DBG("\n");
			}
		}
		seqendH = buf->getHandle(); /* H 2 BLOCK HEADER (hard pointer) */
		DRW_DBG("seqendH Handle: "); DRW_DBGHL(seqendH.code, seqendH.size, seqendH.ref); DRW_DBG("\n");
	}
	DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
