#include <cstdlib>
#include "drw_lwpolyline.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_LWPolyline::applyExtrusion() {
	if (haveExtrusion) {
		calculateAxis(extPoint);
		for (unsigned int i = 0; i < vertlist.size(); i++) {
			DRW_Vertex2D *vert = vertlist.at(i);
			DRW_Coord v(vert->x, vert->y, elevation);
			extrudePoint(extPoint, &v);
			vert->x = v.x;
			vert->y = v.y;
		}
	}
}

void DRW_LWPolyline::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 10: {
		vertex = new DRW_Vertex2D();
		vertlist.push_back(vertex);
		vertex->x = reader->getDouble();
		break; }
	case 20:
		if (vertex != NULL)
			vertex->y = reader->getDouble();
		break;
	case 40:
		if (vertex != NULL)
			vertex->stawidth = reader->getDouble();
		break;
	case 41:
		if (vertex != NULL)
			vertex->endwidth = reader->getDouble();
		break;
	case 42:
		if (vertex != NULL)
			vertex->bulge = reader->getDouble();
		break;
	case 38:
		elevation = reader->getDouble();
		break;
	case 39:
		thickness = reader->getDouble();
		break;
	case 43:
		width = reader->getDouble();
		break;
	case 70:
		flags = reader->getInt32();
		break;
	case 90:
		vertexnum = reader->getInt32();
		vertlist.reserve(vertexnum);
		break;
	case 210:
		haveExtrusion = true;
		extPoint.x = reader->getDouble();
		break;
	case 220:
		extPoint.y = reader->getDouble();
		break;
	case 230:
		extPoint.z = reader->getDouble();
		break;
	default:
		DRW_Entity::parseCode(code, reader);
		break;
	}
}
bool DRW_LWPolyline::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing LWPolyline *******************************************\n");

	flags = buf->getBitShort();
	DRW_DBG("flags value: "); DRW_DBG(flags);
	if (flags & 4)
		width = buf->getBitDouble();
	if (flags & 8)
		elevation = buf->getBitDouble();
	if (flags & 2)
		thickness = buf->getBitDouble();
	if (flags & 1)
		extPoint = buf->getExtrusion(false);
	vertexnum = buf->getBitLong();
	vertlist.reserve(vertexnum);
	unsigned int bulgesnum = 0;
	if (flags & 16)
		bulgesnum = buf->getBitLong();
	int vertexIdCount = 0;
	if (version > DRW::AC1021) {//2010+
		if (flags & 1024)
			vertexIdCount = buf->getBitLong();
	}

	unsigned int widthsnum = 0;
	if (flags & 32)
		widthsnum = buf->getBitLong();
	DRW_DBG("\nvertex num: "); DRW_DBG(vertexnum); DRW_DBG(" bulges num: "); DRW_DBG(bulgesnum);
	DRW_DBG(" vertexIdCount: "); DRW_DBG(vertexIdCount); DRW_DBG(" widths num: "); DRW_DBG(widthsnum);
	//clear all bit except 128 = plinegen and set 1 to open/close //RLZ:verify plinegen & open
	//dxf: plinegen 128 & open 1
	flags = (flags & 512) ? (flags | 1) : (flags | 0);
	flags &= 129;
	DRW_DBG("end flags value: "); DRW_DBG(flags);

	if (vertexnum > 0) { //verify if is lwpol without vertex (empty)
						 // add vertexs
		vertex = new DRW_Vertex2D();
		vertex->x = buf->getRawDouble();
		vertex->y = buf->getRawDouble();
		vertlist.push_back(vertex);
		DRW_Vertex2D* pv = vertex;
		for (int i = 1; i < vertexnum; i++) {
			vertex = new DRW_Vertex2D();
			if (version < DRW::AC1015) {//14-
				vertex->x = buf->getRawDouble();
				vertex->y = buf->getRawDouble();
			}
			else {
				//                DRW_Vertex2D *pv = vertlist.back();
				vertex->x = buf->getDefaultDouble(pv->x);
				vertex->y = buf->getDefaultDouble(pv->y);
			}
			pv = vertex;
			vertlist.push_back(vertex);
		}
		//add bulges
		for (unsigned int i = 0; i < bulgesnum; i++) {
			double bulge = buf->getBitDouble();
			if (vertlist.size() > i)
				vertlist.at(i)->bulge = bulge;
		}
		//add vertexId
		if (version > DRW::AC1021) {//2010+
			for (int i = 0; i < vertexIdCount; i++) {
				dint32 vertexId = buf->getBitLong();
				//TODO implement vertexId, do not exist in dxf
				DRW_UNUSED(vertexId);
				//                if (vertlist.size()< i)
				//                    vertlist.at(i)->vertexId = vertexId;
			}
		}
		//add widths
		for (unsigned int i = 0; i < widthsnum; i++) {
			double staW = buf->getBitDouble();
			double endW = buf->getBitDouble();
			if (vertlist.size() < i) {
				vertlist.at(i)->stawidth = staW;
				vertlist.at(i)->endwidth = endW;
			}
		}
	}
	if (DRW_DBGGL == DRW_dbg::DEBUG) {
		DRW_DBG("\nVertex list: ");
		for (std::vector<DRW_Vertex2D *>::iterator it = vertlist.begin(); it != vertlist.end(); ++it) {
			DRW_Vertex2D* pv = *it;
			DRW_DBG("\n   x: "); DRW_DBG(pv->x); DRW_DBG(" y: "); DRW_DBG(pv->y); DRW_DBG(" bulge: "); DRW_DBG(pv->bulge);
			DRW_DBG(" stawidth: "); DRW_DBG(pv->stawidth); DRW_DBG(" endwidth: "); DRW_DBG(pv->endwidth);
		}
	}

	DRW_DBG("\n");
	/* Common Entity Handle Data */
	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	/* CRC X --- */
	return buf->isGood();
}
