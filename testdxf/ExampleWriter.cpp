#include "drw_line.h"
#include <drw_entities.h>
#include "ExampleWriter.h"

ExampleWriter::ExampleWriter(dxfRW &dxfW) : _dxfW(dxfW) {
}

void ExampleWriter::writeEntities() {
	for (auto &it : this->_entities) {
		switch (it->eType)
		{
		case DRW::ETYPE::E3DFACE:
			_dxfW.write3dface(static_cast<DRW_3Dface*>(it));
			break;
		case DRW::ETYPE::ARC:
			_dxfW.writeArc(static_cast<DRW_Arc*>(it));
			break;
		case DRW::ETYPE::BLOCK:
			_dxfW.writeBlock(static_cast<DRW_Block*>(it));
			break;
		case DRW::ETYPE::CIRCLE:
			_dxfW.writeCircle(static_cast<DRW_Circle*>(it));
			break;
		case DRW::ETYPE::DIMENSION:
			_dxfW.writeDimension(static_cast<DRW_Dimension*>(it));
			break;
		case DRW::ETYPE::ELLIPSE:
			_dxfW.writeEllipse(static_cast<DRW_Ellipse*>(it));
			break;
		case DRW::ETYPE::HATCH:
			_dxfW.writeHatch(static_cast<DRW_Hatch*>(it));
			break;
			//case DRW::ETYPE::IMAGE:
			//	_dxfW.writeImage(static_cast<DRW_Image*>(it));
			//	break;
		case DRW::ETYPE::INSERT:
			_dxfW.writeInsert(static_cast<DRW_Insert*>(it));
			break;
		case DRW::ETYPE::LEADER:
			_dxfW.writeLeader(static_cast<DRW_Leader*>(it));
			break;
		case DRW::ETYPE::LINE:
			_dxfW.writeLine(static_cast<DRW_Line*>(it));
			break;
		case DRW::ETYPE::LWPOLYLINE:
			_dxfW.writeLWPolyline(static_cast<DRW_LWPolyline*>(it));
			break;
		case DRW::ETYPE::MTEXT:
			_dxfW.writeMText(static_cast<DRW_MText*>(it));
			break;
		case DRW::ETYPE::POINT:
			_dxfW.writePoint(static_cast<DRW_Point*>(it));
			break;
		case DRW::ETYPE::POLYLINE:
			_dxfW.writePolyline(static_cast<DRW_Polyline*>(it));
			break;
		case DRW::ETYPE::RAY:
			_dxfW.writeRay(static_cast<DRW_Ray*>(it));
			break;
		case DRW::ETYPE::SOLID:
			_dxfW.writeSolid(static_cast<DRW_Solid*>(it));
			break;
		case DRW::ETYPE::SPLINE:
			_dxfW.writeSpline(static_cast<DRW_Spline*>(it));
			break;
		case DRW::ETYPE::TEXT:
			_dxfW.writeText(static_cast<DRW_Text*>(it));
			break;
		case DRW::ETYPE::TRACE:
			_dxfW.writeTrace(static_cast<DRW_Trace*>(it));
			break;
			//case DRW::ETYPE::UNDERLAY:
			//	_dxfW.write(static_cast<*>(it));
			//	break;
			//case DRW::ETYPE::VERTEX:
			//	_dxfW.writeArc(static_cast<DRW_Arc*>(it));
			//	break;
		case DRW::ETYPE::XLINE:
			_dxfW.writeXline(static_cast<DRW_Xline*>(it));
			break;
		default:
			break;
		}
	}
}