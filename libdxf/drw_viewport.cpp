#include <cstdlib>
#include "drw_viewport.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Viewport::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 40:
		pswidth = reader->getDouble();
		break;
	case 41:
		psheight = reader->getDouble();
		break;
	case 68:
		vpstatus = reader->getInt32();
		break;
	case 69:
		vpID = reader->getInt32();
		break;
	case 12: {
		centerPX = reader->getDouble();
		break; }
	case 22:
		centerPY = reader->getDouble();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}
//ex 22 dec 34
bool DRW_Viewport::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing viewport *****************************************\n");
	basePoint.x = buf->getBitDouble();
	basePoint.y = buf->getBitDouble();
	basePoint.z = buf->getBitDouble();
	DRW_DBG("center "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	pswidth = buf->getBitDouble();
	psheight = buf->getBitDouble();
	DRW_DBG("\nWidth: "); DRW_DBG(pswidth); DRW_DBG(", Height: "); DRW_DBG(psheight); DRW_DBG("\n");
	//RLZ TODO: complete in dxf
	if (version > DRW::AC1014) {//2000+
		viewTarget.x = buf->getBitDouble();
		viewTarget.y = buf->getBitDouble();
		viewTarget.z = buf->getBitDouble();
		DRW_DBG("view Target "); DRW_DBGPT(viewTarget.x, viewTarget.y, viewTarget.z);
		viewDir.x = buf->getBitDouble();
		viewDir.y = buf->getBitDouble();
		viewDir.z = buf->getBitDouble();
		DRW_DBG("\nview direction "); DRW_DBGPT(viewDir.x, viewDir.y, viewDir.z);
		twistAngle = buf->getBitDouble();
		DRW_DBG("\nView twist Angle: "); DRW_DBG(twistAngle);
		viewHeight = buf->getBitDouble();
		DRW_DBG("\nview Height: "); DRW_DBG(viewHeight);
		viewLength = buf->getBitDouble();
		DRW_DBG(" Lens Length: "); DRW_DBG(viewLength);
		frontClip = buf->getBitDouble();
		DRW_DBG("\nfront Clip Z: "); DRW_DBG(frontClip);
		backClip = buf->getBitDouble();
		DRW_DBG(" back Clip Z: "); DRW_DBG(backClip);
		snapAngle = buf->getBitDouble();
		DRW_DBG("\n snap Angle: "); DRW_DBG(snapAngle);
		centerPX = buf->getRawDouble();
		centerPY = buf->getRawDouble();
		DRW_DBG("\nview center X: "); DRW_DBG(centerPX); DRW_DBG(", Y: "); DRW_DBG(centerPX);
		snapPX = buf->getRawDouble();
		snapPY = buf->getRawDouble();
		DRW_DBG("\nSnap base point X: "); DRW_DBG(snapPX); DRW_DBG(", Y: "); DRW_DBG(snapPY);
		snapSpPX = buf->getRawDouble();
		snapSpPY = buf->getRawDouble();
		DRW_DBG("\nSnap spacing X: "); DRW_DBG(snapSpPX); DRW_DBG(", Y: "); DRW_DBG(snapSpPY);
		//RLZ: need to complete
		DRW_DBG("\nGrid spacing X: "); DRW_DBG(buf->getRawDouble()); DRW_DBG(", Y: "); DRW_DBG(buf->getRawDouble()); DRW_DBG("\n");
		DRW_DBG("Circle zoom?: "); DRW_DBG(buf->getBitShort()); DRW_DBG("\n");
	}
	if (version > DRW::AC1018) {//2007+
		DRW_DBG("Grid major?: "); DRW_DBG(buf->getBitShort()); DRW_DBG("\n");
	}
	if (version > DRW::AC1014) {//2000+
		frozenLyCount = buf->getBitLong();
		DRW_DBG("Frozen Layer count?: "); DRW_DBG(frozenLyCount); DRW_DBG("\n");
		DRW_DBG("Status Flags?: "); DRW_DBG(buf->getBitLong()); DRW_DBG("\n");
		//RLZ: Warning needed separate string bufer
		DRW_DBG("Style sheet?: "); DRW_DBG(sBuf->getVariableText(version, false)); DRW_DBG("\n");
		DRW_DBG("Render mode?: "); DRW_DBG(buf->getRawChar8()); DRW_DBG("\n");
		DRW_DBG("UCS OMore...: "); DRW_DBG(buf->getBit()); DRW_DBG("\n");
		DRW_DBG("UCS VMore...: "); DRW_DBG(buf->getBit()); DRW_DBG("\n");
		DRW_DBG("UCS OMore...: "); DRW_DBGPT(buf->getBitDouble(), buf->getBitDouble(), buf->getBitDouble()); DRW_DBG("\n");
		DRW_DBG("ucs XAMore...: "); DRW_DBGPT(buf->getBitDouble(), buf->getBitDouble(), buf->getBitDouble()); DRW_DBG("\n");
		DRW_DBG("UCS YMore....: "); DRW_DBGPT(buf->getBitDouble(), buf->getBitDouble(), buf->getBitDouble()); DRW_DBG("\n");
		DRW_DBG("UCS EMore...: "); DRW_DBG(buf->getBitDouble()); DRW_DBG("\n");
		DRW_DBG("UCS OVMore...: "); DRW_DBG(buf->getBitShort()); DRW_DBG("\n");
	}
	if (version > DRW::AC1015) {//2004+
		DRW_DBG("ShadePlot Mode...: "); DRW_DBG(buf->getBitShort()); DRW_DBG("\n");
	}
	if (version > DRW::AC1018) {//2007+
		DRW_DBG("Use def Ligth...: "); DRW_DBG(buf->getBit()); DRW_DBG("\n");
		DRW_DBG("Def ligth tipe?: "); DRW_DBG(buf->getRawChar8()); DRW_DBG("\n");
		DRW_DBG("Brightness: "); DRW_DBG(buf->getBitDouble()); DRW_DBG("\n");
		DRW_DBG("Contrast: "); DRW_DBG(buf->getBitDouble()); DRW_DBG("\n");
		//        DRW_DBG("Ambient Cmc or Enc: "); DRW_DBG(buf->getCmColor(version)); DRW_DBG("\n");
		DRW_DBG("Ambient (Cmc or Enc?), Enc: "); DRW_DBG(buf->getEnColor(version)); DRW_DBG("\n");
	}
	ret = DRW_Entity::parseDwgEntHandle(version, buf);

	dwgHandle someHdl;
	if (version < DRW::AC1015) {//R13 & R14 only
		DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		someHdl = buf->getHandle();
		DRW_DBG("ViewPort ent header: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
	}
	if (version > DRW::AC1014) {//2000+
		for (duint8 i = 0; i < frozenLyCount; ++i) {
			someHdl = buf->getHandle();
			DRW_DBG("Frozen layer handle "); DRW_DBG(i); DRW_DBG(": "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		}
		someHdl = buf->getHandle();
		DRW_DBG("Clip bpundary handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		if (version == DRW::AC1015) {//2000 only
			someHdl = buf->getHandle();
			DRW_DBG("ViewPort ent header: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		}
		someHdl = buf->getHandle();
		DRW_DBG("Named ucs handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		someHdl = buf->getHandle();
		DRW_DBG("base ucs handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
	}
	if (version > DRW::AC1018) {//2007+
		someHdl = buf->getHandle();
		DRW_DBG("background handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		someHdl = buf->getHandle();
		DRW_DBG("visual style handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		someHdl = buf->getHandle();
		DRW_DBG("shadeplot ID handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
		DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		someHdl = buf->getHandle();
		DRW_DBG("SUN handle: "); DRW_DBGHL(someHdl.code, someHdl.size, someHdl.ref); DRW_DBG("\n");
	}
	DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	if (!ret)
		return ret;
	return buf->isGood();
}
