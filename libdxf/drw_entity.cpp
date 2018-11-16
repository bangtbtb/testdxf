/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2011-2015 José F. Soriano, rallazz@gmail.com               **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <cstdlib>
#include "drw_entities.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


//! Calculate arbitary axis
/*!
*   Calculate arbitary axis for apply extrusions
*  @author Rallaz
*/
void DRW_Entity::calculateAxis(DRW_Coord extPoint) {
	//Follow the arbitrary DXF definitions for extrusion axes.
	if (fabs(extPoint.x) < 0.015625 && fabs(extPoint.y) < 0.015625) {
		//If we get here, implement Ax = Wy x N where Wy is [0,1,0] per the DXF spec.
		//The cross product works out to Wy.y*N.z-Wy.z*N.y, Wy.z*N.x-Wy.x*N.z, Wy.x*N.y-Wy.y*N.x
		//Factoring in the fixed values for Wy gives N.z,0,-N.x
		extAxisX.x = extPoint.z;
		extAxisX.y = 0;
		extAxisX.z = -extPoint.x;
	}
	else {
		//Otherwise, implement Ax = Wz x N where Wz is [0,0,1] per the DXF spec.
		//The cross product works out to Wz.y*N.z-Wz.z*N.y, Wz.z*N.x-Wz.x*N.z, Wz.x*N.y-Wz.y*N.x
		//Factoring in the fixed values for Wz gives -N.y,N.x,0.
		extAxisX.x = -extPoint.y;
		extAxisX.y = extPoint.x;
		extAxisX.z = 0;
	}

	extAxisX.unitize();

	//Ay = N x Ax
	extAxisY.x = (extPoint.y * extAxisX.z) - (extAxisX.y * extPoint.z);
	extAxisY.y = (extPoint.z * extAxisX.x) - (extAxisX.z * extPoint.x);
	extAxisY.z = (extPoint.x * extAxisX.y) - (extAxisX.x * extPoint.y);

	extAxisY.unitize();
}

//! Extrude a point using arbitary axis
/*!
*   apply extrusion in a point using arbitary axis (previous calculated)
*  @author Rallaz
*/
void DRW_Entity::extrudePoint(DRW_Coord extPoint, DRW_Coord *point) {
	double px, py, pz;
	px = (extAxisX.x*point->x) + (extAxisY.x*point->y) + (extPoint.x*point->z);
	py = (extAxisX.y*point->x) + (extAxisY.y*point->y) + (extPoint.y*point->z);
	pz = (extAxisX.z*point->x) + (extAxisY.z*point->y) + (extPoint.z*point->z);

	point->x = px;
	point->y = py;
	point->z = pz;
}

bool DRW_Entity::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 5:
		handle = reader->getHandleString();
		break;
	case 330:
		parentHandle = reader->getHandleString();
		break;
	case 8:
		layer = reader->getUtf8String();
		break;
	case 6:
		lineType = reader->getUtf8String();
		break;
	case 62:
		color = reader->getInt32();
		break;
	case 370:
		lWeight = DRW_LW_Conv::dxfInt2lineWidth(reader->getInt32());
		break;
	case 48:
		ltypeScale = reader->getDouble();
		break;
	case 60:
		visible = reader->getBool();
		break;
	case 420:
		color24 = reader->getInt32();
		break;
	case 430:
		colorName = reader->getString();
		break;
	case 67:
		space = static_cast<DRW::Space>(reader->getInt32());
		break;
	case 102:
		parseDxfGroups(code, reader);
		break;
	case 1000:
	case 1001:
	case 1002:
	case 1003:
	case 1004:
	case 1005:
		extData.push_back(new DRW_Variant(code, reader->getString()));
		break;
	case 1010:
	case 1011:
	case 1012:
	case 1013:
		curr = new DRW_Variant(code, DRW_Coord(reader->getDouble(), 0.0, 0.0));
		extData.push_back(curr);
		break;
	case 1020:
	case 1021:
	case 1022:
	case 1023:
		if (curr)
			curr->setCoordY(reader->getDouble());
		break;
	case 1030:
	case 1031:
	case 1032:
	case 1033:
		if (curr)
			curr->setCoordZ(reader->getDouble());
		curr = NULL;
		break;
	case 1040:
	case 1041:
	case 1042:
		extData.push_back(new DRW_Variant(code, reader->getDouble()));
		break;
	case 1070:
	case 1071:
		extData.push_back(new DRW_Variant(code, reader->getInt32()));
		break;
	default:
		break;
	}
	return true;
}

//parses dxf 102 groups to read entity
bool DRW_Entity::parseDxfGroups(int code, dxfReader *reader) {
	std::list<DRW_Variant> ls;
	DRW_Variant curr;
	int nc;
	std::string appName = reader->getString();
	if (!appName.empty() && appName.at(0) == '{') {
		curr.addString(code, appName.substr(1, (int)appName.size() - 1));
		ls.push_back(curr);
		while (code != 102 && appName.at(0) == '}') {
			reader->readRec(&nc);//RLZ curr.code = code or nc?
//            curr.code = code;
			//RLZ code == 330 || code == 360 OR nc == 330 || nc == 360 ?
			if (code == 330 || code == 360)
				curr.addInt(code, reader->getHandleString());//RLZ code or nc
			else {
				switch (reader->type) {
				case dxfReader::STRING:
					curr.addString(code, reader->getString());//RLZ code or nc
					break;
				case dxfReader::INT32:
				case dxfReader::INT64:
					curr.addInt(code, reader->getInt32());//RLZ code or nc
					break;
				case dxfReader::DOUBLE:
					curr.addDouble(code, reader->getDouble());//RLZ code or nc
					break;
				case dxfReader::BOOL:
					curr.addInt(code, reader->getInt32());//RLZ code or nc
					break;
				default:
					break;
				}
			}
			ls.push_back(curr);
		}
	}

	appData.push_back(ls);
	return true;
}

bool DRW_Entity::parseDwg(DRW::Version version, dwgBuffer *buf, dwgBuffer* strBuf, duint32 bs) {
	objSize = 0;
	DRW_DBG("\n***************************** parsing entity *********************************************\n");
	oType = buf->getObjType(version);
	DRW_DBG("Object type: "); DRW_DBG(oType); DRW_DBG(", "); DRW_DBGH(oType);

	if (version > DRW::AC1014 && version < DRW::AC1024) {//2000 & 2004
		objSize = buf->getRawLong32();  //RL 32bits object size in bits
		DRW_DBG(" Object size: "); DRW_DBG(objSize); DRW_DBG("\n");
	}
	if (version > DRW::AC1021) {//2010+
		duint32 ms = buf->size();
		objSize = ms * 8 - bs;
		DRW_DBG(" Object size: "); DRW_DBG(objSize); DRW_DBG("\n");
	}

	if (strBuf != NULL && version > DRW::AC1018) {//2007+
		strBuf->moveBitPos(objSize - 1);
		DRW_DBG(" strBuf strbit pos 2007: "); DRW_DBG(strBuf->getPosition()); DRW_DBG(" strBuf bpos 2007: "); DRW_DBG(strBuf->getBitPos()); DRW_DBG("\n");
		if (strBuf->getBit() == 1) {
			DRW_DBG("DRW_TableEntry::parseDwg string bit is 1\n");
			strBuf->moveBitPos(-17);
			duint16 strDataSize = strBuf->getRawShort16();
			DRW_DBG("\nDRW_TableEntry::parseDwg string strDataSize: "); DRW_DBGH(strDataSize); DRW_DBG("\n");
			if ((strDataSize & 0x8000) == 0x8000) {
				DRW_DBG("\nDRW_TableEntry::parseDwg string 0x8000 bit is set");
				strBuf->moveBitPos(-33);//RLZ pending to verify
				duint16 hiSize = strBuf->getRawShort16();
				strDataSize = ((strDataSize & 0x7fff) | (hiSize << 15));
			}
			strBuf->moveBitPos(-strDataSize - 16); //-14
			DRW_DBG("strBuf start strDataSize pos 2007: "); DRW_DBG(strBuf->getPosition()); DRW_DBG(" strBuf bpos 2007: "); DRW_DBG(strBuf->getBitPos()); DRW_DBG("\n");
		}
		else
			DRW_DBG("\nDRW_TableEntry::parseDwg string bit is 0");
		DRW_DBG("strBuf start pos 2007: "); DRW_DBG(strBuf->getPosition()); DRW_DBG(" strBuf bpos 2007: "); DRW_DBG(strBuf->getBitPos()); DRW_DBG("\n");
	}

	dwgHandle ho = buf->getHandle();
	handle = ho.ref;
	DRW_DBG("Entity Handle: "); DRW_DBGHL(ho.code, ho.size, ho.ref);
	dint16 extDataSize = buf->getBitShort(); //BS
	DRW_DBG(" ext data size: "); DRW_DBG(extDataSize);
	while (extDataSize > 0 && buf->isGood()) {
		/* RLZ: TODO */
		dwgHandle ah = buf->getHandle();
		DRW_DBG("App Handle: "); DRW_DBGHL(ah.code, ah.size, ah.ref);
		duint8 *tmpExtData = new duint8[extDataSize];
		buf->getBytes(tmpExtData, extDataSize);
		dwgBuffer tmpExtDataBuf(tmpExtData, extDataSize, buf->decoder);

		duint8 dxfCode = tmpExtDataBuf.getRawChar8();
		DRW_DBG(" dxfCode: "); DRW_DBG(dxfCode);
		switch (dxfCode) {
		case 0: {
			duint8 strLength = tmpExtDataBuf.getRawChar8();
			DRW_DBG(" strLength: "); DRW_DBG(strLength);
			duint16 cp = tmpExtDataBuf.getBERawShort16();
			DRW_DBG(" str codepage: "); DRW_DBG(cp);
			for (int i = 0; i < strLength + 1; i++) {//string length + null terminating char
				duint8 dxfChar = tmpExtDataBuf.getRawChar8();
				DRW_DBG(" dxfChar: "); DRW_DBG(dxfChar);
			}
			break;
		}
		default:
			/* RLZ: TODO */
			break;
		}
		delete[]tmpExtData;
		extDataSize = buf->getBitShort(); //BS
		DRW_DBG(" ext data size: "); DRW_DBG(extDataSize);
	} //end parsing extData (EED)
	duint8 graphFlag = buf->getBit(); //B
	DRW_DBG(" graphFlag: "); DRW_DBG(graphFlag); DRW_DBG("\n");
	if (graphFlag) {
		duint32 graphDataSize = buf->getRawLong32();  //RL 32bits
		DRW_DBG("graphData in bytes: "); DRW_DBG(graphDataSize); DRW_DBG("\n");
		// RLZ: TODO
				//skip graphData bytes
		duint8 *tmpGraphData = new duint8[graphDataSize];
		buf->getBytes(tmpGraphData, graphDataSize);
		dwgBuffer tmpGraphDataBuf(tmpGraphData, graphDataSize, buf->decoder);
		DRW_DBG("graph data remaining bytes: "); DRW_DBG(tmpGraphDataBuf.numRemainingBytes()); DRW_DBG("\n");
		delete[]tmpGraphData;
	}
	if (version < DRW::AC1015) {//14-
		objSize = buf->getRawLong32();  //RL 32bits object size in bits
		DRW_DBG(" Object size in bits: "); DRW_DBG(objSize); DRW_DBG("\n");
	}

	duint8 entmode = buf->get2Bits(); //BB
	if (entmode == 0)
		ownerHandle = true;
	//        entmode = 2;
	else if (entmode == 2)
		entmode = 0;
	space = (DRW::Space)entmode; //RLZ verify cast values
	DRW_DBG("entmode: "); DRW_DBG(entmode);
	numReactors = buf->getBitShort(); //BS
	DRW_DBG(", numReactors: "); DRW_DBG(numReactors);

	if (version < DRW::AC1015) {//14-
		if (buf->getBit()) {//is bylayer line type
			lineType = "BYLAYER";
			ltFlags = 0;
		}
		else {
			lineType = "";
			ltFlags = 3;
		}
		DRW_DBG(" lineType: "); DRW_DBG(lineType.c_str());
		DRW_DBG(" ltFlags: "); DRW_DBG(ltFlags);
	}
	if (version > DRW::AC1015) {//2004+
		xDictFlag = buf->getBit();
		DRW_DBG(" xDictFlag: "); DRW_DBG(xDictFlag); DRW_DBG("\n");
	}

	if (version > DRW::AC1024 || version < DRW::AC1018) {
		haveNextLinks = buf->getBit(); //aka nolinks //B
		DRW_DBG(", haveNextLinks (0 yes, 1 prev next): "); DRW_DBG(haveNextLinks); DRW_DBG("\n");
	}
	else {
		haveNextLinks = 1; //aka nolinks //B
		DRW_DBG(", haveNextLinks (forced): "); DRW_DBG(haveNextLinks); DRW_DBG("\n");
	}
	//ENC color
	color = buf->getEnColor(version); //BS or CMC //ok for R14 or negate
	ltypeScale = buf->getBitDouble(); //BD
	DRW_DBG(" entity color: "); DRW_DBG(color);
	DRW_DBG(" ltScale: "); DRW_DBG(ltypeScale); DRW_DBG("\n");
	if (version > DRW::AC1014) {//2000+
		UTF8STRING plotStyleName;
		for (duint8 i = 0; i < 2; ++i) { //two flags in one
			plotFlags = buf->get2Bits(); //BB
			if (plotFlags == 1)
				plotStyleName = "byblock";
			else if (plotFlags == 2)
				plotStyleName = "continuous";
			else if (plotFlags == 0)
				plotStyleName = "bylayer";
			else //handle at end
				plotStyleName = "";
			if (i == 0) {
				ltFlags = plotFlags;
				lineType = plotStyleName; //RLZ: howto solve? if needed plotStyleName;
				DRW_DBG("ltFlags: "); DRW_DBG(ltFlags);
				DRW_DBG(" lineType: "); DRW_DBG(lineType.c_str());
			}
			else {
				DRW_DBG(", plotFlags: "); DRW_DBG(plotFlags);
			}
		}
	}
	if (version > DRW::AC1018) {//2007+
		materialFlag = buf->get2Bits(); //BB
		DRW_DBG("materialFlag: "); DRW_DBG(materialFlag);
		shadowFlag = buf->getRawChar8(); //RC
		DRW_DBG("shadowFlag: "); DRW_DBG(shadowFlag); DRW_DBG("\n");
	}
	if (version > DRW::AC1021) {//2010+
		duint8 visualFlags = buf->get2Bits(); //full & face visual style
		DRW_DBG("shadowFlag 2: "); DRW_DBG(visualFlags); DRW_DBG("\n");
		duint8 unk = buf->getBit(); //edge visual style
		DRW_DBG("unknown bit: "); DRW_DBG(unk); DRW_DBG("\n");
	}
	dint16 invisibleFlag = buf->getBitShort(); //BS
	DRW_DBG(" invisibleFlag: "); DRW_DBG(invisibleFlag);
	if (version > DRW::AC1014) {//2000+
		lWeight = DRW_LW_Conv::dwgInt2lineWidth(buf->getRawChar8()); //RC
		DRW_DBG(" lwFlag (lWeight): "); DRW_DBG(lWeight); DRW_DBG("\n");
	}
	//Only in blocks ????????
//    if (version > DRW::AC1018) {//2007+
//        duint8 unk = buf->getBit();
//        DRW_DBG("unknown bit: "); DRW_DBG(unk); DRW_DBG("\n");
//    }
	return buf->isGood();
}

bool DRW_Entity::parseDwgEntHandle(DRW::Version version, dwgBuffer *buf) {
	if (version > DRW::AC1018) {//2007+ skip string area
		buf->setPosition(objSize >> 3);
		buf->setBitPos(objSize & 7);
	}

	if (ownerHandle) {//entity are in block or in a polyline
		dwgHandle ownerH = buf->getOffsetHandle(handle);
		DRW_DBG("owner (parent) Handle: "); DRW_DBGHL(ownerH.code, ownerH.size, ownerH.ref); DRW_DBG("\n");
		DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		parentHandle = ownerH.ref;
		DRW_DBG("Block (parent) Handle: "); DRW_DBGHL(ownerH.code, ownerH.size, parentHandle); DRW_DBG("\n");
	}
	else
		DRW_DBG("NO Block (parent) Handle\n");

	DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	for (int i = 0; i < numReactors; ++i) {
		dwgHandle reactorsH = buf->getHandle();
		DRW_DBG(" reactorsH control Handle: "); DRW_DBGHL(reactorsH.code, reactorsH.size, reactorsH.ref); DRW_DBG("\n");
	}
	if (xDictFlag != 1) {//linetype in 2004 seems not have XDicObjH or NULL handle
		dwgHandle XDicObjH = buf->getHandle();
		DRW_DBG(" XDicObj control Handle: "); DRW_DBGHL(XDicObjH.code, XDicObjH.size, XDicObjH.ref); DRW_DBG("\n");
	}
	DRW_DBG("Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");

	if (version < DRW::AC1015) {//R14-
		//layer handle
		layerH = buf->getOffsetHandle(handle);
		DRW_DBG(" layer Handle: "); DRW_DBGHL(layerH.code, layerH.size, layerH.ref); DRW_DBG("\n");
		DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		//lineType handle
		if (ltFlags == 3) {
			lTypeH = buf->getOffsetHandle(handle);
			DRW_DBG("linetype Handle: "); DRW_DBGHL(lTypeH.code, lTypeH.size, lTypeH.ref); DRW_DBG("\n");
			DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		}
	}
	if (version < DRW::AC1018) {//2000+
		if (haveNextLinks == 0) {
			dwgHandle nextLinkH = buf->getOffsetHandle(handle);
			DRW_DBG(" prev nextLinkers Handle: "); DRW_DBGHL(nextLinkH.code, nextLinkH.size, nextLinkH.ref); DRW_DBG("\n");
			DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
			prevEntLink = nextLinkH.ref;
			nextLinkH = buf->getOffsetHandle(handle);
			DRW_DBG(" next nextLinkers Handle: "); DRW_DBGHL(nextLinkH.code, nextLinkH.size, nextLinkH.ref); DRW_DBG("\n");
			DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
			nextEntLink = nextLinkH.ref;
		}
		else {
			nextEntLink = handle + 1;
			prevEntLink = handle - 1;
		}
	}
	if (version > DRW::AC1015) {//2004+
		//Parses Bookcolor handle
	}
	if (version > DRW::AC1014) {//2000+
		//layer handle
		layerH = buf->getOffsetHandle(handle);
		DRW_DBG(" layer Handle: "); DRW_DBGHL(layerH.code, layerH.size, layerH.ref); DRW_DBG("\n");
		DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		//lineType handle
		if (ltFlags == 3) {
			lTypeH = buf->getOffsetHandle(handle);
			DRW_DBG("linetype Handle: "); DRW_DBGHL(lTypeH.code, lTypeH.size, lTypeH.ref); DRW_DBG("\n");
			DRW_DBG("   Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		}
	}
	if (version > DRW::AC1014) {//2000+
		if (version > DRW::AC1018) {//2007+
			if (materialFlag == 3) {
				dwgHandle materialH = buf->getOffsetHandle(handle);
				DRW_DBG(" material Handle: "); DRW_DBGHL(materialH.code, materialH.size, materialH.ref); DRW_DBG("\n");
				DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
			}
			if (shadowFlag == 3) {
				dwgHandle shadowH = buf->getOffsetHandle(handle);
				DRW_DBG(" shadow Handle: "); DRW_DBGHL(shadowH.code, shadowH.size, shadowH.ref); DRW_DBG("\n");
				DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
			}
		}
		if (plotFlags == 3) {
			dwgHandle plotStyleH = buf->getOffsetHandle(handle);
			DRW_DBG(" plot style Handle: "); DRW_DBGHL(plotStyleH.code, plotStyleH.size, plotStyleH.ref); DRW_DBG("\n");
			DRW_DBG("\n Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
		}
	}
	DRW_DBG("\n DRW_Entity::parseDwgEntHandle Remaining bytes: "); DRW_DBG(buf->numRemainingBytes()); DRW_DBG("\n");
	return buf->isGood();
}
//
//void DRW_Solid::parseCode(int code, dxfReader *reader) {
//	DRW_Trace::parseCode(code, reader);
//}
//
//bool DRW_Solid::parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs) {
//	DRW_DBG("\n***************************** parsing Solid *********************************************\n");
//	return DRW_Trace::parseDwg(v, buf, bs);
//}
