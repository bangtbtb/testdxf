#include <cstdlib>
#include "drw_dimension.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Dimension::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 1:
		text = reader->getUtf8String();
		break;
	case 2:
		name = reader->getString();
		break;
	case 3:
		style = reader->getUtf8String();
		break;
	case 70:
		type = reader->getInt32();
		break;
	case 71:
		align = reader->getInt32();
		break;
	case 72:
		linesty = reader->getInt32();
		break;
	case 10:
		defPoint.x = reader->getDouble();
		break;
	case 20:
		defPoint.y = reader->getDouble();
		break;
	case 30:
		defPoint.z = reader->getDouble();
		break;
	case 11:
		textPoint.x = reader->getDouble();
		break;
	case 21:
		textPoint.y = reader->getDouble();
		break;
	case 31:
		textPoint.z = reader->getDouble();
		break;
	case 12:
		clonePoint.x = reader->getDouble();
		break;
	case 22:
		clonePoint.y = reader->getDouble();
		break;
	case 32:
		clonePoint.z = reader->getDouble();
		break;
	case 13:
		def1.x = reader->getDouble();
		break;
	case 23:
		def1.y = reader->getDouble();
		break;
	case 33:
		def1.z = reader->getDouble();
		break;
	case 14:
		def2.x = reader->getDouble();
		break;
	case 24:
		def2.y = reader->getDouble();
		break;
	case 34:
		def2.z = reader->getDouble();
		break;
	case 15:
		circlePoint.x = reader->getDouble();
		break;
	case 25:
		circlePoint.y = reader->getDouble();
		break;
	case 35:
		circlePoint.z = reader->getDouble();
		break;
	case 16:
		arcPoint.x = reader->getDouble();
		break;
	case 26:
		arcPoint.y = reader->getDouble();
		break;
	case 36:
		arcPoint.z = reader->getDouble();
		break;
	case 41:
		linefactor = reader->getDouble();
		break;
	case 53:
		rot = reader->getDouble();
		break;
	case 50:
		angle = reader->getDouble();
		break;
	case 52:
		oblique = reader->getDouble();
		break;
	case 40:
		length = reader->getDouble();
		break;
	case 51:
		hdir = reader->getDouble();
		break;
	default:
		DRW_Entity::parseCode(code, reader);
		break;
	}
}

bool DRW_Dimension::parseDwg(DRW::Version version, dwgBuffer *buf, dwgBuffer *sBuf) {
	DRW_DBG("\n***************************** parsing dimension *********************************************");
	if (version > DRW::AC1021) { //2010+
		duint8 dimVersion = buf->getRawChar8();
		DRW_DBG("\ndimVersion: "); DRW_DBG(dimVersion);
	}
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG("\nextPoint: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);
	if (version > DRW::AC1014) { //2000+
		DRW_DBG("\nFive unknown bits: "); DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit());
		DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit()); DRW_DBG(buf->getBit());
	}
	textPoint.x = buf->getRawDouble();
	textPoint.y = buf->getRawDouble();
	textPoint.z = buf->getBitDouble();
	DRW_DBG("\ntextPoint: "); DRW_DBGPT(textPoint.x, textPoint.y, textPoint.z);
	type = buf->getRawChar8();
	DRW_DBG("\ntype (70) read: "); DRW_DBG(type);
	type = (type & 1) ? type & 0x7F : type | 0x80; //set bit 7
	type = (type & 2) ? type | 0x20 : type & 0xDF; //set bit 5
	DRW_DBG(" type (70) set: "); DRW_DBG(type);
	//clear last 3 bits to set integer dim type
	type &= 0xF8;
	text = sBuf->getVariableText(version, false);
	DRW_DBG("\nforced dim text: "); DRW_DBG(text.c_str());
	rot = buf->getBitDouble();
	hdir = buf->getBitDouble();
	DRW_Coord inspoint = buf->get3BitDouble();
	DRW_DBG("\ninspoint: "); DRW_DBGPT(inspoint.x, inspoint.y, inspoint.z);
	double insRot_code54 = buf->getBitDouble(); //RLZ: unknown, investigate
	DRW_DBG(" insRot_code54: "); DRW_DBG(insRot_code54);
	if (version > DRW::AC1014) { //2000+
		align = buf->getBitShort();
		linesty = buf->getBitShort();
		linefactor = buf->getBitDouble();
		double actMeas = buf->getBitDouble();
		DRW_DBG("\n  actMeas_code42: "); DRW_DBG(actMeas);
		if (version > DRW::AC1018) { //2007+
			bool unk = !!buf->getBit();
			bool flip1 = !!buf->getBit();
			bool flip2 = !!buf->getBit();
			DRW_DBG("\n2007, unk, flip1, flip2: "); DRW_DBG(unk); DRW_DBG(flip1); DRW_DBG(flip2);
		}
	}
	clonePoint.x = buf->getRawDouble();
	clonePoint.y = buf->getRawDouble();
	DRW_DBG("\nclonePoint: "); DRW_DBGPT(clonePoint.x, clonePoint.y, clonePoint.z);

	return buf->isGood();
}
