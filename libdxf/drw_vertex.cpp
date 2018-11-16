#include <cstdlib>
#include "drw_vertex.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Vertex::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 70:
		flags = reader->getInt32();
		break;
	case 40:
		stawidth = reader->getDouble();
		break;
	case 41:
		endwidth = reader->getDouble();
		break;
	case 42:
		bulge = reader->getDouble();
		break;
	case 50:
		tgdir = reader->getDouble();
		break;
	case 71:
		vindex1 = reader->getInt32();
		break;
	case 72:
		vindex2 = reader->getInt32();
		break;
	case 73:
		vindex3 = reader->getInt32();
		break;
	case 74:
		vindex4 = reader->getInt32();
		break;
	case 91:
		identifier = reader->getInt32();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

//0x0A vertex 2D
//0x0B vertex 3D
//0x0C MESH
//0x0D PFACE
//0x0E PFACE FACE
bool DRW_Vertex::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs, double el) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing pline Vertex *********************************************\n");

	if (oType == 0x0A) { //pline 2D, needed example
		flags = buf->getRawChar8(); //RLZ: EC  unknown type
		DRW_DBG("flags value: "); DRW_DBG(flags);
		basePoint = buf->get3BitDouble();
		basePoint.z = el;
		DRW_DBG("basePoint: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
		stawidth = buf->getBitDouble();
		if (stawidth < 0)
			endwidth = stawidth = fabs(stawidth);
		else
			endwidth = buf->getBitDouble();
		bulge = buf->getBitDouble();
		if (version > DRW::AC1021) //2010+
			DRW_DBG("Vertex ID: "); DRW_DBG(buf->getBitLong());
		tgdir = buf->getBitDouble();
	}
	else if (oType == 0x0B || oType == 0x0C || oType == 0x0D) { //PFACE
		flags = buf->getRawChar8(); //RLZ: EC  unknown type
		DRW_DBG("flags value: "); DRW_DBG(flags);
		basePoint = buf->get3BitDouble();
		DRW_DBG("basePoint: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	}
	else if (oType == 0x0E) { //PFACE FACE
		vindex1 = buf->getBitShort();
		vindex2 = buf->getBitShort();
		vindex3 = buf->getBitShort();
		vindex4 = buf->getBitShort();
	}

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
