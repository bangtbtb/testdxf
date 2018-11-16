#include <cstdlib>
#include "drw_text.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Text::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 40:
		height = reader->getDouble();
		break;
	case 41:
		widthscale = reader->getDouble();
		break;
	case 50:
		angle = reader->getDouble();
		break;
	case 51:
		oblique = reader->getDouble();
		break;
	case 71:
		textgen = reader->getInt32();
		break;
	case 72:
		alignH = (HAlign)reader->getInt32();
		break;
	case 73:
		alignV = (VAlign)reader->getInt32();
		break;
	case 1:
		text = reader->getUtf8String();
		break;
	case 7:
		style = reader->getUtf8String();
		break;
	default:
		DRW_Line::parseCode(code, reader);
		break;
	}
}

bool DRW_Text::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing text *********************************************\n");

	// DataFlags RC Used to determine presence of subsquent data, set to 0xFF for R14-
	duint8 data_flags = 0x00;
	if (version > DRW::AC1014) {//2000+
		data_flags = buf->getRawChar8(); /* DataFlags RC Used to determine presence of subsquent data */
		DRW_DBG("data_flags: "); DRW_DBG(data_flags); DRW_DBG("\n");
		if (!(data_flags & 0x01)) { /* Elevation RD --- present if !(DataFlags & 0x01) */
			basePoint.z = buf->getRawDouble();
		}
	}
	else {//14-
		basePoint.z = buf->getBitDouble(); /* Elevation BD --- */
	}
	basePoint.x = buf->getRawDouble(); /* Insertion pt 2RD 10 */
	basePoint.y = buf->getRawDouble();
	DRW_DBG("Insert point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z); DRW_DBG("\n");
	if (version > DRW::AC1014) {//2000+
		if (!(data_flags & 0x02)) { /* Alignment pt 2DD 11 present if !(DataFlags & 0x02), use 10 & 20 values for 2 default values.*/
			secPoint.x = buf->getDefaultDouble(basePoint.x);
			secPoint.y = buf->getDefaultDouble(basePoint.y);
		}
		else {
			secPoint = basePoint;
		}
	}
	else {//14-
		secPoint.x = buf->getRawDouble();  /* Alignment pt 2RD 11 */
		secPoint.y = buf->getRawDouble();
	}
	secPoint.z = basePoint.z;
	DRW_DBG("Alignment: "); DRW_DBGPT(secPoint.x, secPoint.y, basePoint.z); DRW_DBG("\n");
	extPoint = buf->getExtrusion(version > DRW::AC1014);
	DRW_DBG("Extrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z); DRW_DBG("\n");
	thickness = buf->getThickness(version > DRW::AC1014); /* Thickness BD 39 */

	if (version > DRW::AC1014) {//2000+
		if (!(data_flags & 0x04)) { /* Oblique ang RD 51 present if !(DataFlags & 0x04) */
			oblique = buf->getRawDouble();
		}
		if (!(data_flags & 0x08)) { /* Rotation ang RD 50 present if !(DataFlags & 0x08) */
			angle = buf->getRawDouble();
		}
		height = buf->getRawDouble(); /* Height RD 40 */
		if (!(data_flags & 0x10)) { /* Width factor RD 41 present if !(DataFlags & 0x10) */
			widthscale = buf->getRawDouble();
		}
	}
	else {//14-
		oblique = buf->getBitDouble(); /* Oblique ang BD 51 */
		angle = buf->getBitDouble(); /* Rotation ang BD 50 */
		height = buf->getBitDouble(); /* Height BD 40 */
		widthscale = buf->getBitDouble(); /* Width factor BD 41 */
	}
	DRW_DBG("thickness: "); DRW_DBG(thickness); DRW_DBG(", Oblique ang: "); DRW_DBG(oblique); DRW_DBG(", Width: ");
	DRW_DBG(widthscale); DRW_DBG(", Rotation: "); DRW_DBG(angle); DRW_DBG(", height: "); DRW_DBG(height); DRW_DBG("\n");
	text = sBuf->getVariableText(version, false); /* Text value TV 1 */
	DRW_DBG("text string: "); DRW_DBG(text.c_str()); DRW_DBG("\n");
	//textgen, alignH, alignV always present in R14-, data_flags set in initialisation
	if (!(data_flags & 0x20)) { /* Generation BS 71 present if !(DataFlags & 0x20) */
		textgen = buf->getBitShort();
		DRW_DBG("textgen: "); DRW_DBG(textgen);
	}
	if (!(data_flags & 0x40)) { /* Horiz align. BS 72 present if !(DataFlags & 0x40) */
		alignH = (HAlign)buf->getBitShort();
		DRW_DBG(", alignH: "); DRW_DBG(alignH);
	}
	if (!(data_flags & 0x80)) { /* Vert align. BS 73 present if !(DataFlags & 0x80) */
		alignV = (VAlign)buf->getBitShort();
		DRW_DBG(", alignV: "); DRW_DBG(alignV);
	}
	DRW_DBG("\n");

	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;

	styleH = buf->getHandle(); /* H 7 STYLE (hard pointer) */
	DRW_DBG("text style Handle: "); DRW_DBGHL(styleH.code, styleH.size, styleH.ref); DRW_DBG("\n");

	/* CRC X --- */
	return buf->isGood();
}
