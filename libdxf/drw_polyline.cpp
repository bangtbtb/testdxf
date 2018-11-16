#include <cstdlib>
#include "drw_polyline.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Polyline::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 70:
		flags = reader->getInt32();
		break;
	case 40:
		defstawidth = reader->getDouble();
		break;
	case 41:
		defendwidth = reader->getDouble();
		break;
	case 71:
		vertexcount = reader->getInt32();
		break;
	case 72:
		facecount = reader->getInt32();
		break;
	case 73:
		smoothM = reader->getInt32();
		break;
	case 74:
		smoothN = reader->getInt32();
		break;
	case 75:
		curvetype = reader->getInt32();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

//0x0F polyline 2D bit 4(8) & 5(16) NOT set
//0x10 polyline 3D bit 4(8) set
//0x1D PFACE bit 5(16) set
bool DRW_Polyline::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing polyline *********************************************\n");

	dint32 ooCount = 0;
	if (oType == 0x0F) { //pline 2D
		flags = buf->getBitShort();
		DRW_DBG("flags value: "); DRW_DBG(flags);
		curvetype = buf->getBitShort();
		defstawidth = buf->getBitDouble();
		defendwidth = buf->getBitDouble();
		thickness = buf->getThickness(version > DRW::AC1014);
		basePoint = DRW_Coord(0, 0, buf->getBitDouble());
		extPoint = buf->getExtrusion(version > DRW::AC1014);
	}
	else if (oType == 0x10) { //pline 3D
		duint8 tmpFlag = buf->getRawChar8();
		DRW_DBG("flags 1 value: "); DRW_DBG(tmpFlag);
		if (tmpFlag & 1)
			curvetype = 5;
		else if (tmpFlag & 2)
			curvetype = 6;
		if (tmpFlag & 3) {
			curvetype = 8;
			flags |= 4;
		}
		tmpFlag = buf->getRawChar8();
		if (tmpFlag & 1)
			flags |= 1;
		flags |= 8; //indicate 3DPOL
		DRW_DBG("flags 2 value: "); DRW_DBG(tmpFlag);
	}
	else if (oType == 0x1D) { //PFACE
		flags = 64;
		vertexcount = buf->getBitShort();
		DRW_DBG("vertex count: "); DRW_DBG(vertexcount);
		facecount = buf->getBitShort();
		DRW_DBG("face count: "); DRW_DBG(facecount);
		DRW_DBG("flags value: "); DRW_DBG(flags);
	}
	if (version > DRW::AC1015) { //2004+
		ooCount = buf->getBitLong();
	}

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;

	if (version < DRW::AC1018) { //2000-
		dwgHandle objectH = buf->getOffsetHandle(handle);
		firstEH = objectH.ref;
		DRW_DBG(" first Vertex Handle: "); DRW_DBGHL(objectH.code, objectH.size, objectH.ref); DRW_DBG("\n");
		objectH = buf->getOffsetHandle(handle);
		lastEH = objectH.ref;
		DRW_DBG(" last Vertex Handle: "); DRW_DBGHL(objectH.code, objectH.size, objectH.ref); DRW_DBG("\n");
		DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	}
	else {
		for (dint32 i = 0; i < ooCount; ++i) {
			dwgHandle objectH = buf->getOffsetHandle(handle);
			hadlesList.push_back(objectH.ref);
			DRW_DBG(" Vertex Handle: "); DRW_DBGHL(objectH.code, objectH.size, objectH.ref); DRW_DBG("\n");
			DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		}
	}
	seqEndH = buf->getOffsetHandle(handle);
	DRW_DBG(" SEQEND Handle: "); DRW_DBGHL(seqEndH.code, seqEndH.size, seqEndH.ref); DRW_DBG("\n");
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	//    RS crc;   //RS */
	return buf->isGood();
}
