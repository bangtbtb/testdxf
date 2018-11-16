#include <cstdlib>
#include "drw_ellipse.h"
#include "drw_vertex.h"
#include "drw_polyline.h"
#include "intern/dxfreader.h"
#include "intern/dwgbuffer.h"
#include "intern/drw_dbg.h"


void DRW_Ellipse::parseCode(int code, dxfReader *reader) {
	switch (code) {
	case 40:
		ratio = reader->getDouble();
		break;
	case 41:
		staparam = reader->getDouble();
		break;
	case 42:
		endparam = reader->getDouble();
		break;
	default:
		DRW_Line::parseCode(code, reader);
		break;
	}
}

void DRW_Ellipse::applyExtrusion() {
	if (haveExtrusion) {
		calculateAxis(extPoint);
		extrudePoint(extPoint, &secPoint);
		double intialparam = staparam;
		if (extPoint.z < 0.) {
			staparam = M_PIx2 - endparam;
			endparam = M_PIx2 - intialparam;
		}
	}
}

//if ratio > 1 minor axis are greather than major axis, correct it
void DRW_Ellipse::correctAxis() {
	bool complete = false;
	if (staparam == endparam) {
		staparam = 0.0;
		endparam = M_PIx2; //2*M_PI;
		complete = true;
	}
	if (ratio > 1) {
		if (fabs(endparam - staparam - M_PIx2) < 1.0e-10)
			complete = true;
		double incX = secPoint.x;
		secPoint.x = -(secPoint.y * ratio);
		secPoint.y = incX*ratio;
		ratio = 1 / ratio;
		if (!complete) {
			if (staparam < M_PI_2)
				staparam += M_PI * 2;
			if (endparam < M_PI_2)
				endparam += M_PI * 2;
			endparam -= M_PI_2;
			staparam -= M_PI_2;
		}
	}
}

bool DRW_Ellipse::parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs) {
	bool ret = DRW_Entity::parseDwg(version, buf, NULL, bs);
	if (!ret)
		return ret;
	DRW_DBG("\n***************************** parsing ellipse *********************************************\n");

	basePoint = buf->get3BitDouble();
	DRW_DBG("center: "); DRW_DBGPT(basePoint.x, basePoint.y, basePoint.z);
	secPoint = buf->get3BitDouble();
	DRW_DBG(", axis: "); DRW_DBGPT(secPoint.x, secPoint.y, secPoint.z); DRW_DBG("\n");
	extPoint = buf->get3BitDouble();
	DRW_DBG("Extrusion: "); DRW_DBGPT(extPoint.x, extPoint.y, extPoint.z);
	ratio = buf->getBitDouble();//BD
	DRW_DBG("\nratio: "); DRW_DBG(ratio);
	staparam = buf->getBitDouble();//BD
	DRW_DBG(" start param: "); DRW_DBG(staparam);
	endparam = buf->getBitDouble();//BD
	DRW_DBG(" end param: "); DRW_DBG(endparam); DRW_DBG("\n");

	ret = DRW_Entity::parseDwgEntHandle(version, buf);
	if (!ret)
		return ret;
	//    RS crc;   //RS */
	return buf->isGood();
}



//parts are the number of vertex to split polyline, default 128
void DRW_Ellipse::toPolyline(DRW_Polyline *pol, int parts) {
	double radMajor, radMinor, cosRot, sinRot, incAngle, curAngle;
	double cosCurr, sinCurr;
	radMajor = sqrt(secPoint.x*secPoint.x + secPoint.y*secPoint.y);
	radMinor = radMajor*ratio;
	//calculate sin & cos of included angle
	incAngle = atan2(secPoint.y, secPoint.x);
	cosRot = cos(incAngle);
	sinRot = sin(incAngle);
	incAngle = M_PIx2 / parts;
	curAngle = staparam;
	int i = static_cast<int>(curAngle / incAngle);
	do {
		if (curAngle > endparam) {
			curAngle = endparam;
			i = parts + 2;
		}
		cosCurr = cos(curAngle);
		sinCurr = sin(curAngle);
		double x = basePoint.x + (cosCurr*cosRot*radMajor) - (sinCurr*sinRot*radMinor);
		double y = basePoint.y + (cosCurr*sinRot*radMajor) + (sinCurr*cosRot*radMinor);
		pol->addVertex(DRW_Vertex(x, y, 0.0, 0.0));
		curAngle = (++i)*incAngle;
	} while (i < parts);
	if (fabs(endparam - staparam - M_PIx2) < 1.0e-10) {
		pol->flags = 1;
	}
	pol->layer = this->layer;
	pol->lineType = this->lineType;
	pol->color = this->color;
	pol->lWeight = this->lWeight;
	pol->extPoint = this->extPoint;
}
