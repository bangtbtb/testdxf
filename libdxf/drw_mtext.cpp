#include <cstdlib>
#include "drw_mtext.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"
void DRW_MText::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 1:
		text += reader->getString();
		text = reader->toUtf8String(text);
		break;
	case 11:
		haveXAxis = true;
		DRW_Text::parseCode(code, reader);
		break;
	case 3:
		text += reader->getString();
		break;
	case 44:
		interlin = reader->getDouble();
		break;
	default:
		DRW_Text::parseCode(code, reader);
		break;
	}
}

bool DRW_MText::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing mtext *********************************************\n");

	basePoint = buf->get3BitDouble(); /* Insertion pt 3BD 10 - First picked point. */
	DRW_DBG("Insertion: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z); DRW_DBG("\n");
	extPoint = buf->get3BitDouble(); /* Extrusion 3BD 210 Undocumented; */
	secPoint = buf->get3BitDouble(); /* X-axis dir 3BD 11 */
	updateAngle();
	widthscale = buf->getBitDouble(); /* Rect width BD 41 */
	if (version > DRW::AC1018) {//2007+
								/* Rect height BD 46 Reference rectangle height. */
		/** @todo */buf->getBitDouble();
	}
	height = buf->getBitDouble();/* Text height BD 40 Undocumented */
	textgen = buf->getBitShort(); /* Attachment BS 71 Similar to justification; */
								  /* Drawing dir BS 72 Left to right, etc.; see DXF doc */
	dint16 draw_dir = buf->getBitShort();
	DRW_UNUSED(draw_dir);
	/* Extents ht BD Undocumented and not present in DXF or entget */
	double ext_ht = buf->getBitDouble();
	DRW_UNUSED(ext_ht);
	/* Extents wid BD Undocumented and not present in DXF or entget The extents
	rectangle, when rotated the same as the text, fits the actual text image on
	the screen (altough we've seen it include an extra row of text in height). */
	double ext_wid = buf->getBitDouble();
	DRW_UNUSED(ext_wid);
	/* Text TV 1 All text in one long string (without '\n's 3 for line wrapping).
	ACAD seems to add braces ({ }) and backslash-P's to indicate paragraphs
	based on the "\r\n"'s found in the imported file. But, all the text is in
	this one long string -- not broken into 1- and 3-groups as in DXF and
	entget. ACAD's entget breaks this string into 250-char pieces (not 255 as
	doc'd) – even if it's mid-word. The 1-group always gets the tag end;
	therefore, the 3's are always 250 chars long. */
	text = sBuf->getVariableText(version, false); /* Text value TV 1 */
	if (version > DRW::AC1014) {//2000+
		buf->getBitShort();/* Linespacing Style BS 73 */
		buf->getBitDouble();/* Linespacing Factor BD 44 */
		buf->getBit();/* Unknown bit B */
	}
	if (version > DRW::AC1015) {//2004+
								/* Background flags BL 0 = no background, 1 = background fill, 2 =background
								fill with drawing fill color. */
		dint32 bk_flags = buf->getBitLong(); /** @todo add to DRW_MText */
		if (bk_flags == 1) {
			/* Background scale factor BL Present if background flags = 1, default = 1.5*/
			buf->getBitLong();
			/* Background color CMC Present if background flags = 1 */
			buf->getCmColor(version); //RLZ: warning CMC or ENC
									  /** @todo buf->getCMC */
									  /* Background transparency BL Present if background flags = 1 */
			buf->getBitLong();
		}
	}

	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;

	styleH = buf->getHandle(); /* H 7 STYLE (hard pointer) */
	DRW_DBG("text style Handle: "); DRW_DBG(styleH.code); DRW_DBG(".");
	DRW_DBG(styleH.size); DRW_DBG("."); DRW_DBG(styleH.ref); DRW_DBG("\n");

	/* CRC X --- */
	return buf->isGood();
}

void DRW_MText::updateAngle() {
	if (haveXAxis) {
		angle = atan2(secPoint.y, secPoint.x) * 180 / M_PI;
	}
}
