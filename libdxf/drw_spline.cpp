#include <cstdlib>
#include "drw_spline.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"

void DRW_Spline::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 210:
		normalVec.x = reader->getDouble();
		break;
	case 220:
		normalVec.y = reader->getDouble();
		break;
	case 230:
		normalVec.z = reader->getDouble();
		break;
	case 12:
		tgStart.x = reader->getDouble();
		break;
	case 22:
		tgStart.y = reader->getDouble();
		break;
	case 32:
		tgStart.z = reader->getDouble();
		break;
	case 13:
		tgEnd.x = reader->getDouble();
		break;
	case 23:
		tgEnd.y = reader->getDouble();
		break;
	case 33:
		tgEnd.z = reader->getDouble();
		break;
	case 70:
		flags = reader->getInt32();
		break;
	case 71:
		degree = reader->getInt32();
		break;
	case 72:
		nknots = reader->getInt32();
		break;
	case 73:
		ncontrol = reader->getInt32();
		break;
	case 74:
		nfit = reader->getInt32();
		break;
	case 42:
		tolknot = reader->getDouble();
		break;
	case 43:
		tolcontrol = reader->getDouble();
		break;
	case 44:
		tolfit = reader->getDouble();
		break;
	case 10: {
		controlpoint = new DRW_Coord();
		controllist.push_back(controlpoint);
		controlpoint->x = reader->getDouble();
		break; }
	case 20:
		if (controlpoint != NULL)
			controlpoint->y = reader->getDouble();
		break;
	case 30:
		if (controlpoint != NULL)
			controlpoint->z = reader->getDouble();
		break;
	case 11: {
		fitpoint = new DRW_Coord();
		fitlist.push_back(fitpoint);
		fitpoint->x = reader->getDouble();
		break; }
	case 21:
		if (fitpoint != NULL)
			fitpoint->y = reader->getDouble();
		break;
	case 31:
		if (fitpoint != NULL)
			fitpoint->z = reader->getDouble();
		break;
	case 40:
		knotslist.push_back(reader->getDouble());
		break;
		//    case 41:
		//        break;
	default:
		DRW_Entity::parseCode(code, reader);
		break;
	}
}

bool DRW_Spline::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing spline *********************************************\n");
	duint8 weight = 0; // RLZ ??? flags, weight, code 70, bit 4 (16)

	dint32 scenario = buf->getBitLong();
	DRW_DBG("scenario: "); DRW_DBG(scenario);
	if (version > DRW::AC1024) {
		dint32 splFlag1 = buf->getBitLong();
		if (splFlag1 & 1)
			scenario = 2;
		dint32 knotParam = buf->getBitLong();
		DRW_DBG("2013 splFlag1: "); DRW_DBG(splFlag1); DRW_DBG(" 2013 knotParam: ");
		DRW_DBG(knotParam);
		//        DRW_DBG("unk bit: "); DRW_DBG(buf->getBit());
	}
	degree = buf->getBitLong(); //RLZ: code 71, verify with dxf
	DRW_DBG(" degree: "); DRW_DBG(degree); DRW_DBG("\n");
	if (scenario == 2) {
		flags = 8;//scenario 2 = not rational & planar
		tolfit = buf->getBitDouble();//BD
		DRW_DBG("flags: "); DRW_DBG(flags); DRW_DBG(" tolfit: "); DRW_DBG(tolfit);
		tgStart = buf->get3BitDouble();
		DRW_DBG(" Start Tangent: "); DRW_DBGPT(tgStart.x, tgStart.y, tgStart.z);
		tgEnd = buf->get3BitDouble();
		DRW_DBG("\nEnd Tangent: "); DRW_DBGPT(tgEnd.x, tgEnd.y, tgEnd.z);
		nfit = buf->getBitLong();
		DRW_DBG("\nnumber of fit points: "); DRW_DBG(nfit);
	}
	else if (scenario == 1) {
		flags = 8;//scenario 1 = rational & planar
		flags |= buf->getBit() << 2; //flags, rational, code 70, bit 2 (4)
		flags |= buf->getBit(); //flags, closed, code 70, bit 0 (1)
		flags |= buf->getBit() << 1; //flags, periodic, code 70, bit 1 (2)
		tolknot = buf->getBitDouble();
		tolcontrol = buf->getBitDouble();
		DRW_DBG("flags: "); DRW_DBG(flags); DRW_DBG(" knot tolerance: "); DRW_DBG(tolknot);
		DRW_DBG(" control point tolerance: "); DRW_DBG(tolcontrol);
		nknots = buf->getBitLong();
		ncontrol = buf->getBitLong();
		weight = buf->getBit(); // RLZ ??? flags, weight, code 70, bit 4 (16)
		DRW_DBG("\nnum of knots: "); DRW_DBG(nknots); DRW_DBG(" num of control pt: ");
		DRW_DBG(ncontrol); DRW_DBG(" weight bit: "); DRW_DBG(weight);
	}
	else {
		DRW_DBG("\ndwg Ellipse, unknouwn scenario\n");
		return false; //RLZ: from doc only 1 or 2 are ok ?
	}

	knotslist.reserve(nknots);
	for (dint32 i = 0; i < nknots; ++i) {
		knotslist.push_back(buf->getBitDouble());
	}
	controllist.reserve(ncontrol);
	for (dint32 i = 0; i < ncontrol; ++i) {
		DRW_Coord* crd = new DRW_Coord(buf->get3BitDouble());
		controllist.push_back(crd);
		if (weight) {
			DRW_DBG("\n w: "); DRW_DBG(buf->getBitDouble()); //RLZ Warning: D (BD or RD)
		}
	}
	fitlist.reserve(nfit);
	for (dint32 i = 0; i < nfit; ++i) {
		DRW_Coord* crd = new DRW_Coord(buf->get3BitDouble());
		fitlist.push_back(crd);
	}
	if (DRW_DBGGL == DRW_dbg::DEBUG) {
		DRW_DBG("\nknots list: ");
		for (std::vector<double>::iterator it = knotslist.begin(); it != knotslist.end(); ++it) {
			DRW_DBG("\n"); DRW_DBG(*it);
		}
		DRW_DBG("\ncontrol point list: ");
		for (std::vector<DRW_Coord *>::iterator it = controllist.begin(); it != controllist.end(); ++it) {
			DRW_DBG("\n"); DRW_DBGPT((*it)->x, (*it)->y, (*it)->z);
		}
		DRW_DBG("\nfit point list: ");
		for (std::vector<DRW_Coord *>::iterator it = fitlist.begin(); it != fitlist.end(); ++it) {
			DRW_DBG("\n"); DRW_DBGPT((*it)->x, (*it)->y, (*it)->z);
		}
	}

	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}
