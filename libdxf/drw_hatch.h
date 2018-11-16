
#ifndef DRW_HATCH_H
#define DRW_HATCH_H

#include "drw_point.h"
#include "drw_arc.h"
#include "drw_lwpolyline.h"
#include "drw_ellipse.h"
#include "drw_spline.h"
//! Class to handle hatch entity
/*!
*  Class to handle hatch entity
*  @author Rallaz
*/
//TODO: handle lwpolylines, splines and ellipses
class DRW_Hatch : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Hatch() {
		eType = DRW::HATCH;
		angle = scale = 0.0;
		basePoint.x = basePoint.y = basePoint.z = 0.0;
		loopsnum = hstyle = associative = 0;
		solid = hpattern = 1;
		deflines = doubleflag = 0;
		loop = NULL;
		clearEntities();
	}

	~DRW_Hatch() {
		while (!looplist.empty()) {
			looplist.pop_back();
		}
	}

	void appendLoop(DRW_HatchLoop *v) {
		looplist.push_back(v);
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	UTF8STRING name;           /*!< hatch pattern name, code 2 */
	int solid;                 /*!< solid fill flag, code 70, solid=1, pattern=0 */
	int associative;           /*!< associativity, code 71, associatve=1, non-assoc.=0 */
	int hstyle;                /*!< hatch style, code 75 */
	int hpattern;              /*!< hatch pattern type, code 76 */
	int doubleflag;            /*!< hatch pattern double flag, code 77, double=1, single=0 */
	int loopsnum;              /*!< namber of boundary paths (loops), code 91 */
	double angle;              /*!< hatch pattern angle, code 52 */
	double scale;              /*!< hatch pattern scale, code 41 */
	int deflines;              /*!< number of pattern definition lines, code 78 */

	std::vector<DRW_HatchLoop *> looplist;  /*!< polyline list */

private:
	void clearEntities() {
		pt = line = NULL;
		pline = NULL;
		arc = NULL;
		ellipse = NULL;
		spline = NULL;
		plvert = NULL;
	}

	void addLine() {
		clearEntities();
		if (loop) {
			pt = line = new DRW_Line;
			loop->objlist.push_back(line);
		}
	}

	void addArc() {
		clearEntities();
		if (loop) {
			pt = arc = new DRW_Arc;
			loop->objlist.push_back(arc);
		}
	}

	void addEllipse() {
		clearEntities();
		if (loop) {
			pt = ellipse = new DRW_Ellipse;
			loop->objlist.push_back(ellipse);
		}
	}

	void addSpline() {
		clearEntities();
		if (loop) {
			pt = NULL;
			spline = new DRW_Spline;
			loop->objlist.push_back(spline);
		}
	}

	DRW_HatchLoop *loop;       /*!< current loop to add data */
	DRW_Line *line;
	DRW_Arc *arc;
	DRW_Ellipse *ellipse;
	DRW_Spline *spline;
	DRW_LWPolyline *pline;
	DRW_Point *pt;
	DRW_Vertex2D *plvert;
	bool ispol;
};


#endif
