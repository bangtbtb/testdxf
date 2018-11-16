#include <cstdlib>
#include "drw_leader.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Leader::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 3:
		style = reader->getUtf8String();
		break;
	case 71:
		arrow = reader->getInt32();
		break;
	case 72:
		leadertype = reader->getInt32();
		break;
	case 73:
		flag = reader->getInt32();
		break;
	case 74:
		hookline = reader->getInt32();
		break;
	case 75:
		hookflag = reader->getInt32();
		break;
	case 76:
		vertnum = reader->getInt32();
		break;
	case 77:
		coloruse = reader->getInt32();
		break;
	case 40:
		textheight = reader->getDouble();
		break;
	case 41:
		textwidth = reader->getDouble();
		break;
	case 10: {
		vertexpoint = new DRW_Coord();
		vertexlist.push_back(vertexpoint);
		vertexpoint->x = reader->getDouble();
		break; }
	case 20:
		if (vertexpoint != NULL)
			vertexpoint->y = reader->getDouble();
		break;
	case 30:
		if (vertexpoint != NULL)
			vertexpoint->z = reader->getDouble();
		break;
	case 340:
		annotHandle = reader->getHandleString();
		break;
	case 210:
		extrusionPoint.x = reader->getDouble();
		break;
	case 220:
		extrusionPoint.y = reader->getDouble();
		break;
	case 230:
		extrusionPoint.z = reader->getDouble();
		break;
	case 211:
		horizdir.x = reader->getDouble();
		break;
	case 221:
		horizdir.y = reader->getDouble();
		break;
	case 231:
		horizdir.z = reader->getDouble();
		break;
	case 212:
		offsetblock.x = reader->getDouble();
		break;
	case 222:
		offsetblock.y = reader->getDouble();
		break;
	case 232:
		offsetblock.z = reader->getDouble();
		break;
	case 213:
		offsettext.x = reader->getDouble();
		break;
	case 223:
		offsettext.y = reader->getDouble();
		break;
	case 233:
		offsettext.z = reader->getDouble();
		break;
	default:
		DRW_Entity::parseCode(code, reader);
		break;
	}
}

bool DRW_Leader::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing leader *********************************************\n");
	DRW_DBG("unknown bit "); DRW_DBG(buf->getBit());
	DRW_DBG(" annot type "); DRW_DBG(buf->getBitShort());
	DRW_DBG(" Path type "); DRW_DBG(buf->getBitShort());
	dint32 nPt = buf->getBitLong();
	DRW_DBG(" Num pts "); DRW_DBG(nPt);

	// add vertexs
	for (int i = 0; i < nPt; i++) {
		DRW_Coord* vertex = new DRW_Coord(buf->get3BitDouble());
		vertexlist.push_back(vertex);
		DRW_DBG("\nvertex "); DRW_DBGPT(vertex->x, vertex->y, vertex->z);
	}
	DRW_Coord Endptproj = buf->get3BitDouble();
	DRW_DBG("\nEndptproj "); DRW_DBGPT(Endptproj.x, Endptproj.y, Endptproj.z);
	extrusionPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG("\nextrusionPoint "); DRW_DBGPT(extrusionPoint.x, extrusionPoint.y, extrusionPoint.z);
	if (version > DRW::AC1014) { //2000+
		DRW_DBG("\nFive unknown bits: "); DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit());
		DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit());
	}
	horizdir = buf->get3BitDouble();
	DRW_DBG("\nhorizdir "); DRW_DBGPT(horizdir.x, horizdir.y, horizdir.z);
	offsetblock = buf->get3BitDouble();
	DRW_DBG("\noffsetblock "); DRW_DBGPT(offsetblock.x, offsetblock.y, offsetblock.z);
	if (version > DRW::AC1012) { //R14+
		DRW_Coord unk = buf->get3BitDouble();
		DRW_DBG("\nunknown "); DRW_DBGPT(unk.x, unk.y, unk.z);
	}
	if (version < DRW::AC1015) { //R14 -
		DRW_DBG("\ndimgap "); DRW_DBG(buf->getBitDouble());
	}
	if (version < DRW::AC1024) { //2010-
		textheight = buf->getBitDouble();
		textwidth = buf->getBitDouble();
		DRW_DBG("\ntextheight "); DRW_DBG(textheight); DRW_DBG(" textwidth "); DRW_DBG(textwidth);
	}
	hookline = buf->getBit();
	arrow = buf->getBit();
	DRW_DBG(" hookline "); DRW_DBG(hookline); DRW_DBG(" arrow flag "); DRW_DBG(arrow);

	if (version < DRW::AC1015) { //R14 -
		DRW_DBG("\nArrow head type "); DRW_DBG(buf->getBitShort());
		DRW_DBG("dimasz "); DRW_DBG(buf->getBitDouble());
		DRW_DBG("\nunk bit "); DRW_DBG(buf->getBit());
		DRW_DBG(" unk bit "); DRW_DBG(buf->getBit());
		DRW_DBG(" unk short "); DRW_DBG(buf->getBitShort());
		DRW_DBG(" byBlock color "); DRW_DBG(buf->getBitShort());
		DRW_DBG(" unk bit "); DRW_DBG(buf->getBit());
		DRW_DBG(" unk bit "); DRW_DBG(buf->getBit());
	}
	else { //R2000+
		DRW_DBG("\nunk short "); DRW_DBG(buf->getBitShort());
		DRW_DBG(" unk bit "); DRW_DBG(buf->getBit());
		DRW_DBG(" unk bit "); DRW_DBG(buf->getBit());
	}
	DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	AnnotH = buf->getHandle();
	annotHandle = AnnotH.ref;
	DRW_DBG("annot block Handle: "); DRW_DBGHL(AnnotH.code, AnnotH.size, dimStyleH.ref); DRW_DBG("\n");
	dimStyleH = buf->getHandle(); /* H 7 STYLE (hard pointer) */
	DRW_DBG("dim style Handle: "); DRW_DBGHL(dimStyleH.code, dimStyleH.size, dimStyleH.ref); DRW_DBG("\n");
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	//    RS crc;   //RS */
	return buf->isGood();
}
