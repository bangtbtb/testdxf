#include <cstdlib>
#include "drw_hatch.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Hatch::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 2:
		name = reader->getUtf8String();
		break;
	case 70:
		solid = reader->getInt32();
		break;
	case 71:
		associative = reader->getInt32();
		break;
	case 72:        /*edge type*/
		if (ispol) { //if is polyline is a as_bulge flag
			break;
		}
		else if (reader->getInt32() == 1) { //line
			addLine();
		}
		else if (reader->getInt32() == 2) { //arc
			addArc();
		}
		else if (reader->getInt32() == 3) { //elliptic arc
			addEllipse();
		}
		else if (reader->getInt32() == 4) { //spline
			addSpline();
		}
		break;
	case 10:
		if (pt) pt->basePoint.x = reader->getDouble();
		else if (pline) {
			plvert = pline->addVertex();
			plvert->x = reader->getDouble();
		}
		break;
	case 20:
		if (pt) pt->basePoint.y = reader->getDouble();
		else if (plvert) plvert->y = reader->getDouble();
		break;
	case 11:
		if (line) line->secPoint.x = reader->getDouble();
		else if (ellipse) ellipse->secPoint.x = reader->getDouble();
		break;
	case 21:
		if (line) line->secPoint.y = reader->getDouble();
		else if (ellipse) ellipse->secPoint.y = reader->getDouble();
		break;
	case 40:
		if (arc) arc->radious = reader->getDouble();
		else if (ellipse) ellipse->ratio = reader->getDouble();
		break;
	case 41:
		scale = reader->getDouble();
		break;
	case 42:
		if (plvert) plvert->bulge = reader->getDouble();
		break;
	case 50:
		if (arc) arc->staangle = reader->getDouble() / ARAD;
		else if (ellipse) ellipse->staparam = reader->getDouble() / ARAD;
		break;
	case 51:
		if (arc) arc->endangle = reader->getDouble() / ARAD;
		else if (ellipse) ellipse->endparam = reader->getDouble() / ARAD;
		break;
	case 52:
		angle = reader->getDouble();
		break;
	case 73:
		if (arc) arc->isccw = reader->getInt32();
		else if (pline) pline->flags = reader->getInt32();
		break;
	case 75:
		hstyle = reader->getInt32();
		break;
	case 76:
		hpattern = reader->getInt32();
		break;
	case 77:
		doubleflag = reader->getInt32();
		break;
	case 78:
		deflines = reader->getInt32();
		break;
	case 91:
		loopsnum = reader->getInt32();
		looplist.reserve(loopsnum);
		break;
	case 92:
		loop = new DRW_HatchLoop(reader->getInt32());
		looplist.push_back(loop);
		if (reader->getInt32() & 2) {
			ispol = true;
			clearEntities();
			pline = new DRW_LWPolyline;
			loop->objlist.push_back(pline);
		}
		else ispol = false;
		break;
	case 93:
		if (pline) pline->vertexnum = reader->getInt32();
		else loop->numedges = reader->getInt32();//aqui reserve
		break;
	case 98: //seed points ??
		clearEntities();
		break;
	default:
		DRW_Point::parseCode(code, reader);
		break;
	}
}

bool DRW_Hatch::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	dwgBuffer sBuff = *buf;
	dwgBuffer *sBuf = buf;
	duint32 totalBoundItems = 0;
	bool havePixelSize = false;

	if (version > DRW::AC1018) {//2007+
		sBuf = &sBuff; //separate buffer for strings
	}
	bool ret = DRW_Entity::parseDwg(version, buf, sBuf, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing hatch *********************************************\n");

	//Gradient data, RLZ: is ok or if grad > 0 continue read ?
	if (version > DRW::AC1015) { //2004+
		dint32 isGradient = buf->getBitLong();
		DRW_DBG("is Gradient: "); DRW_DBG(isGradient);
		dint32 res = buf->getBitLong();
		DRW_DBG(" reserved: "); DRW_DBG(res);
		double gradAngle = buf->getBitDouble();
		DRW_DBG(" Gradient angle: "); DRW_DBG(gradAngle);
		double gradShift = buf->getBitDouble();
		DRW_DBG(" Gradient shift: "); DRW_DBG(gradShift);
		dint32 singleCol = buf->getBitLong();
		DRW_DBG("\nsingle color Grad: "); DRW_DBG(singleCol);
		double gradTint = buf->getBitDouble();
		DRW_DBG(" Gradient tint: "); DRW_DBG(gradTint);
		dint32 numCol = buf->getBitLong();
		DRW_DBG(" num colors: "); DRW_DBG(numCol);
		for (dint32 i = 0; i < numCol; ++i) {
			double unkDouble = buf->getBitDouble();
			DRW_DBG("\nunkDouble: "); DRW_DBG(unkDouble);
			duint16 unkShort = buf->getBitShort();
			DRW_DBG(" unkShort: "); DRW_DBG(unkShort);
			dint32 rgbCol = buf->getBitLong();
			DRW_DBG(" rgb color: "); DRW_DBG(rgbCol);
			duint8 ignCol = buf->getRawChar8();
			DRW_DBG(" ignored color: "); DRW_DBG(ignCol);
		}
		UTF8STRING gradName = sBuf->getVariableText(version, false);
		DRW_DBG("\ngradient name: "); DRW_DBG(gradName.c_str()); DRW_DBG("\n");
	}
	basePoint.z = buf->getBitDouble();
	extPoint = buf->get3BitDouble();
	DRW_DBG("base point: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	DRW_DBG("\nextrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);
	name = sBuf->getVariableText(version, false);
	DRW_DBG("\nhatch pattern name: "); DRW_DBG(name.c_str()); DRW_DBG("\n");
	solid = buf->getBit();
	associative = buf->getBit();
	loopsnum = buf->getBitLong();

	//read loops
	for (dint32 i = 0; i < loopsnum; ++i) {
		loop = new DRW_HatchLoop(buf->getBitLong());
		havePixelSize |= loop->type & 4;
		if (!(loop->type & 2)) { //Not polyline
			dint32 numPathSeg = buf->getBitLong();
			for (dint32 j = 0; j < numPathSeg; ++j) {
				duint8 typePath = buf->getRawChar8();
				if (typePath == 1) { //line
					addLine();
					line->basePoint = buf->get2RawDouble();
					line->secPoint = buf->get2RawDouble();
				}
				else if (typePath == 2) { //circle arc
					addArc();
					arc->basePoint = buf->get2RawDouble();
					arc->radious = buf->getBitDouble();
					arc->staangle = buf->getBitDouble();
					arc->endangle = buf->getBitDouble();
					arc->isccw = buf->getBit();
				}
				else if (typePath == 3) { //ellipse arc
					addEllipse();
					ellipse->basePoint = buf->get2RawDouble();
					ellipse->secPoint = buf->get2RawDouble();
					ellipse->ratio = buf->getBitDouble();
					ellipse->staparam = buf->getBitDouble();
					ellipse->endparam = buf->getBitDouble();
					ellipse->isccw = buf->getBit();
				}
				else if (typePath == 4) { //spline
					addSpline();
					spline->degree = buf->getBitLong();
					bool isRational = !!buf->getBit();
					spline->flags |= (isRational << 2); //rational
					spline->flags |= (buf->getBit() << 1); //periodic
					spline->nknots = buf->getBitLong();
					spline->knotslist.reserve(spline->nknots);
					for (dint32 j = 0; j < spline->nknots; ++j) {
						spline->knotslist.push_back(buf->getBitDouble());
					}
					spline->ncontrol = buf->getBitLong();
					spline->controllist.reserve(spline->ncontrol);
					for (dint32 j = 0; j < spline->ncontrol; ++j) {
						DRW_Coord* crd = new DRW_Coord(buf->get3BitDouble());
						spline->controllist.push_back(crd);
						if (isRational)
							crd->z = buf->getBitDouble(); //RLZ: investigate how store weight
						spline->controllist.push_back(crd);
					}
					if (version > DRW::AC1021) { //2010+
						spline->nfit = buf->getBitLong();
						spline->fitlist.reserve(spline->nfit);
						for (dint32 j = 0; j < spline->nfit; ++j) {
							DRW_Coord* crd = new DRW_Coord(buf->get3BitDouble());
							spline->fitlist.push_back(crd);
						}
						spline->tgStart = buf->get2RawDouble();
						spline->tgEnd = buf->get2RawDouble();
					}
				}
			}
		}
		else { //end not pline, start polyline
			pline = new DRW_LWPolyline;
			bool asBulge = buf->getBit();
			pline->flags = buf->getBit();//closed bit
			dint32 numVert = buf->getBitLong();
			for (dint32 j = 0; j < numVert; ++j) {
				DRW_Vertex2D v;
				v.x = buf->getRawDouble();
				v.y = buf->getRawDouble();
				if (asBulge)
					v.bulge = buf->getBitDouble();
				pline->addVertex(v);
			}
			loop->objlist.push_back(pline);
		}//end polyline
		loop->update();
		looplist.push_back(loop);
		totalBoundItems += buf->getBitLong();
		DRW_DBG(" totalBoundItems: "); DRW_DBG(totalBoundItems);
	} //end read loops

	hstyle = buf->getBitShort();
	hpattern = buf->getBitShort();
	DRW_DBG("\nhatch style: "); DRW_DBG(hstyle); DRW_DBG(" pattern type"); DRW_DBG(hpattern);
	if (!solid) {
		angle = buf->getBitDouble();
		scale = buf->getBitDouble();
		doubleflag = buf->getBit();
		deflines = buf->getBitShort();
		for (dint32 i = 0; i < deflines; ++i) {
			DRW_Coord ptL, offL;
			double angleL = buf->getBitDouble();
			ptL.x = buf->getBitDouble();
			ptL.y = buf->getBitDouble();
			offL.x = buf->getBitDouble();
			offL.y = buf->getBitDouble();
			duint16 numDashL = buf->getBitShort();
			DRW_DBG("\ndef line: "); DRW_DBG(angleL); DRW_DBG(","); DRW_DBG(ptL.x); DRW_DBG(","); DRW_DBG(ptL.y);
			DRW_DBG(","); DRW_DBG(offL.x); DRW_DBG(","); DRW_DBG(offL.y); DRW_DBG(","); DRW_DBG(angleL);
			for (duint16 i = 0; i < numDashL; ++i) {
				double lenghtL = buf->getBitDouble();
				DRW_DBG(","); DRW_DBG(lenghtL);
			}
		}//end deflines
	} //end not solid

	if (havePixelSize) {
		ddouble64 pixsize = buf->getBitDouble();
		DRW_DBG("\npixel size: "); DRW_DBG(pixsize);
	}
	dint32 numSeedPoints = buf->getBitLong();
	DRW_DBG("\nnum Seed Points  "); DRW_DBG(numSeedPoints);
	//read Seed Points
	DRW_Coord seedPt;
	for (dint32 i = 0; i < numSeedPoints; ++i) {
		seedPt.x = buf->getRawDouble();
		seedPt.y = buf->getRawDouble();
		DRW_DBG("\n  "); DRW_DBG(seedPt.x); DRW_DBG(","); DRW_DBG(seedPt.y);
	}

	DRW_DBG("\n");
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	for (duint32 i = 0; i < totalBoundItems; ++i) {
		dwgHandle biH = buf->getHandle();
		DRW_DBG("Boundary Items Handle: "); DRW_DBGHL(biH.code, biH.size, biH.ref);
	}
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	//    RS crc;   //RS */
	return buf->isGood();
}
