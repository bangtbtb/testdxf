#ifndef DRW_SPLINE_H
#define DRW_SPLINE_H

#include "drw_entity.h"

//! Class to handle spline entity
/*!
*  Class to handle spline entity
*  @author Rallaz
*/
class DRW_Spline : public DRW_Entity {
	SETENTFRIENDS
public:
	DRW_Spline() {
		eType = DRW::SPLINE;
		flags = nknots = ncontrol = nfit = 0;
		tolknot = tolcontrol = tolfit = 0.0000001;

	}
	~DRW_Spline() {
		while (!controllist.empty()) {
			controllist.pop_back();
		}
		while (!fitlist.empty()) {
			fitlist.pop_back();
		}
	}
	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	//    double ex;                /*!< normal vector x coordinate, code 210 */
	//    double ey;                /*!< normal vector y coordinate, code 220 */
	//    double ez;                /*!< normal vector z coordinate, code 230 */
	DRW_Coord normalVec;      /*!< normal vector, code 210, 220, 230 */
	DRW_Coord tgStart;        /*!< start tangent, code 12, 22, 32 */
							  //    double tgsx;              /*!< start tangent x coordinate, code 12 */
							  //    double tgsy;              /*!< start tangent y coordinate, code 22 */
							  //    double tgsz;              /*!< start tangent z coordinate, code 32 */
	DRW_Coord tgEnd;          /*!< end tangent, code 13, 23, 33 */
							  //    double tgex;              /*!< end tangent x coordinate, code 13 */
							  //    double tgey;              /*!< end tangent y coordinate, code 23 */
							  //    double tgez;              /*!< end tangent z coordinate, code 33 */
	int flags;                /*!< spline flag, code 70 */
	int degree;               /*!< degree of the spline, code 71 */
	dint32 nknots;            /*!< number of knots, code 72, default 0 */
	dint32 ncontrol;          /*!< number of control points, code 73, default 0 */
	dint32 nfit;              /*!< number of fit points, code 74, default 0 */
	double tolknot;           /*!< knot tolerance, code 42, default 0.0000001 */
	double tolcontrol;        /*!< control point tolerance, code 43, default 0.0000001 */
	double tolfit;            /*!< fit point tolerance, code 44, default 0.0000001 */

	std::vector<double> knotslist;           /*!< knots list, code 40 */
	std::vector<DRW_Coord *> controllist;  /*!< control points list, code 10, 20 & 30 */
	std::vector<DRW_Coord *> fitlist;      /*!< fit points list, code 11, 21 & 31 */

private:
	DRW_Coord *controlpoint;   /*!< current control point to add data */
	DRW_Coord *fitpoint;       /*!< current fit point to add data */
};


#endif
