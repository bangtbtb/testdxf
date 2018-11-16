
#ifndef DRW_LWPOLYLINE_H
#define DRW_LWPOLYLINE_H

#include "drw_entity.h"

//! Class to handle lwpolyline entity
/*!
*  Class to handle lwpolyline entity
*  @author Rallaz
*/
class DRW_LWPolyline : public DRW_Entity {
	SETENTFRIENDS
public:
	DRW_LWPolyline() {
		eType = DRW::LWPOLYLINE;
		elevation = thickness = width = 0.0;
		flags = 0;
		extPoint.x = extPoint.y = 0;
		extPoint.z = 1;
		vertex = NULL;
	}

	DRW_LWPolyline(const DRW_LWPolyline& p) :DRW_Entity(p) {
		this->eType = DRW::LWPOLYLINE;
		this->elevation = p.elevation;
		this->thickness = p.thickness;
		this->width = p.width;
		this->flags = p.flags;
		this->extPoint = p.extPoint;
		this->vertex = NULL;
		for (unsigned i = 0; i < p.vertlist.size(); i++)// RLZ ok or new
			this->vertlist.push_back(new DRW_Vertex2D(*(p.vertlist.at(i))));

		this->vertex = NULL;
	}

	~DRW_LWPolyline() {
		while (!vertlist.empty()) {
			vertlist.pop_back();
		}
	}
	virtual void applyExtrusion();
	void addVertex(DRW_Vertex2D v) {
		DRW_Vertex2D *vert = new DRW_Vertex2D();
		vert->x = v.x;
		vert->y = v.y;
		vert->stawidth = v.stawidth;
		vert->endwidth = v.endwidth;
		vert->bulge = v.bulge;
		vertlist.push_back(vert);
	}
	DRW_Vertex2D *addVertex() {
		DRW_Vertex2D *vert = new DRW_Vertex2D();
		vert->stawidth = 0;
		vert->endwidth = 0;
		vert->bulge = 0;
		vertlist.push_back(vert);
		return vert;
	}

protected:
	void parseCode(int code, dxfReader *reader);
	bool parseDwg(DRW::Version v, dwgBuffer *buf, duint32 bs = 0);

public:
	int vertexnum;            /*!< number of vertex, code 90 */
	int flags;                /*!< polyline flag, code 70, default 0 */
	double width;             /*!< constant width, code 43 */
	double elevation;         /*!< elevation, code 38 */
	double thickness;         /*!< thickness, code 39 */
	DRW_Coord extPoint;       /*!<  Dir extrusion normal vector, code 210, 220 & 230 */
	DRW_Vertex2D *vertex;       /*!< current vertex to add data */
	std::vector<DRW_Vertex2D *> vertlist;  /*!< vertex list */
};


#endif
