#include <cstdlib>
#include "drw_image.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Image::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 12:
		vVector.x = reader->getDouble();
		break;
	case 22:
		vVector.y = reader->getDouble();
		break;
	case 32:
		vVector.z = reader->getDouble();
		break;
	case 13:
		sizeu = reader->getDouble();
		break;
	case 23:
		sizev = reader->getDouble();
		break;
	case 340:
		ref = reader->getHandleString();
		break;
	case 280:
		clip = reader->getInt32();
		break;
	case 281:
		brightness = reader->getInt32();
		break;
	case 282:
		contrast = reader->getInt32();
		break;
	case 283:
		fade = reader->getInt32();
		break;
	default:
		DRW_Line::parseCode(code, reader);
		break;
	}
}

bool DRW_Image::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing image *********************************************\n");

	dint32 classVersion = buf->getBitLong();
	DRW_DBG("class Version: "); DRW_DBG(classVersion);
	basePoint = buf->get3BitDouble();
	DRW_DBG("\nbase point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	secPoint = buf->get3BitDouble();
	DRW_DBG("\nU vector: "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z);
	vVector = buf->get3BitDouble();
	DRW_DBG("\nV vector: "); DRW_DBGPT(vVector.x, vVector.y, vVector.z);
	sizeu = buf->getRawDouble();
	sizev = buf->getRawDouble();
	DRW_DBG("\nsize U: "); DRW_DBG(sizeu); DRW_DBG("\nsize V: "); DRW_DBG(sizev);
	duint16 displayProps = buf->getBitShort();
	DRW_UNUSED(displayProps);//RLZ: temporary, complete API
	clip = buf->getBit();
	brightness = buf->getRawChar8();
	contrast = buf->getRawChar8();
	fade = buf->getRawChar8();
	if (version > DRW::AC1021) { //2010+
		bool clipMode = !!buf->getBit();
		DRW_UNUSED(clipMode);//RLZ: temporary, complete API
	}
	duint16 clipType = buf->getBitShort();
	if (clipType == 1) {
		buf->get2RawDouble();
		buf->get2RawDouble();
	}
	else { //clipType == 2
		dint32 numVerts = buf->getBitLong();
		for (int i = 0; i < numVerts; ++i)
			buf->get2RawDouble();
	}

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	dwgHandle biH = buf->getHandle();
	DRW_DBG("ImageDef Handle: "); DRW_DBGHL(biH.code, biH.size, biH.ref);
	ref = biH.ref;
	biH = buf->getHandle();
	DRW_DBG("ImageDefReactor Handle: "); DRW_DBGHL(biH.code, biH.size, biH.ref);
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	//    RS crc;   //RS */
	return buf->isGood();
}
