#ifndef DRW_LEADER_H
#define DRW_LEADER_H

#include "drw_entity.h"

//! Class to handle leader entity
/*!
*  Class to handle leader entity
*  @author Rallaz
*/
class DRW_Leader : public DRW_Entity {
	SETENTFRIENDS
public:
	DRW_Leader() {
		eType = DRW::LEADER;
		flag = 3;
		hookflag = vertnum = leadertype = 0;
		extrusionPoint.x = extrusionPoint.y = 0.0;
		arrow = 1;
		extrusionPoint.z = 1.0;
	}
	~DRW_Leader() {
		while (!vertexlist.empty()) {
			vertexlist.pop_back();
		}
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	UTF8STRING style;          /*!< Dimension style name, code 3 */
	int arrow;                 /*!< Arrowhead flag, code 71, 0=Disabled; 1=Enabled */
	int leadertype;            /*!< Leader path type, code 72, 0=Straight line segments; 1=Spline */
	int flag;                  /*!< Leader creation flag, code 73, default 3 */
	int hookline;              /*!< Hook line direction flag, code 74, default 1 */
	int hookflag;              /*!< Hook line flag, code 75 */
	double textheight;         /*!< Text annotation height, code 40 */
	double textwidth;          /*!< Text annotation width, code 41 */
	int vertnum;               /*!< Number of vertices, code 76 */
	int coloruse;              /*!< Color to use if leader's DIMCLRD = BYBLOCK, code 77 */
	duint32 annotHandle;       /*!< Hard reference to associated annotation, code 340 */
	DRW_Coord extrusionPoint;  /*!< Normal vector, code 210, 220 & 230 */
	DRW_Coord horizdir;        /*!< "Horizontal" direction for leader, code 211, 221 & 231 */
	DRW_Coord offsetblock;     /*!< Offset of last leader vertex from block, code 212, 222 & 232 */
	DRW_Coord offsettext;      /*!< Offset of last leader vertex from annotation, code 213, 223 & 233 */

	std::vector<DRW_Coord *> vertexlist;  /*!< vertex points list, code 10, 20 & 30 */

private:
	DRW_Coord *vertexpoint;   /*!< current control point to add data */
	dwgHandle dimStyleH;
	dwgHandle AnnotH;
};


#endif
