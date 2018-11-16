#include <cstdlib>
#include "drw_block.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Block::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 2:
		name = reader->getUtf8String();
		break;
	case 70:
		flags = reader->getInt32();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

bool DRW_Block::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	if (!isEnd) {
		DRW_DBG("\n***************************** parsing block *********************************************\n");
		name = sBuf->getVariableText(version, false);
		DRW_DBG("Block name: "); DRW_DBG(name.c_str()); DRW_DBG("\n");
	}
	else {
		DRW_DBG("\n***************************** parsing end block *********************************************\n");
	}
	if (version > DRW::AC1018) {//2007+
		duint8 unk = buf->getBit();
		DRW_DBG("unknown bit: "); DRW_DBG(unk); DRW_DBG("\n");
	}
	//    X handleAssoc;   //X
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
