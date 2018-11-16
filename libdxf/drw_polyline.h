#ifndef DRW_POLYLINE_H
#define DRW_POLYLINE_H

#include "drw_vertex.h"

//! Class to handle polyline entity
/*!
*  Class to handle polyline entity
*  @author Rallaz
*/
class DRW_Polyline : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Polyline() {
		eType = DRW::POLYLINE;
		defstawidth = defendwidth = 0.0;
		basePoint.x = basePoint.y = 0.0;
		flags = vertexcount = facecount = 0;
		smoothM = smoothN = curvetype = 0;
	}
	~DRW_Polyline() {
		while (!vertlist.empty()) {
			vertlist.pop_back();
		}
	}
	void addVertex(DRW_Vertex v) {
		DRW_Vertex *vert = new DRW_Vertex();
		vert->basePoint.x = v.basePoint.x;
		vert->basePoint.y = v.basePoint.y;
		vert->basePoint.z = v.basePoint.z;
		vert->stawidth = v.stawidth;
		vert->endwidth = v.endwidth;
		vert->bulge = v.bulge;
		vertlist.push_back(vert);
	}
	void appendVertex(DRW_Vertex *v) {
		vertlist.push_back(v);
	}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	int flags;               /*!< polyline flag, code 70, default 0 */
	double defstawidth;      /*!< Start width, code 40, default 0 */
	double defendwidth;      /*!< End width, code 41, default 0 */
	int vertexcount;         /*!< polygon mesh M vertex or  polyface vertex num, code 71, default 0 */
	int facecount;           /*!< polygon mesh N vertex or  polyface face num, code 72, default 0 */
	int smoothM;             /*!< smooth surface M density, code 73, default 0 */
	int smoothN;             /*!< smooth surface M density, code 74, default 0 */
	int curvetype;           /*!< curves & smooth surface type, code 75, default 0 */

	std::vector<DRW_Vertex *> vertlist;  /*!< vertex list */

private:
	std::list<duint32>hadlesList; //list of handles, only in 2004+
	duint32 firstEH;      //handle of first entity, only in pre-2004
	duint32 lastEH;       //handle of last entity, only in pre-2004
	dwgHandle seqEndH;    //handle of SEQEND entity
};



#endif
