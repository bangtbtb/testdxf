#ifndef DRW_DIMENSION_H
#define DRW_DIMENSION_H


#include "drw_entity.h"

//! Base class for dimension entity
/*!
*  Base class for dimension entity
*  @author Rallaz
*/
class DRW_Dimension : public DRW_Entity {
	SETENTFRIENDS
public:
	DRW_Dimension() {
		eType = DRW::DIMENSION;
		type = 0;
		linesty = 1;
		linefactor = extPoint.z = 1.0;
		angle = oblique = rot = 0.0;
		align = 5;
		style = "STANDARD";
		defPoint.z = extPoint.x = extPoint.y = 0;
		textPoint.z = rot = 0;
		clonePoint.x = clonePoint.y = clonePoint.z = 0;
	}

	DRW_Dimension(const DRW_Dimension& d) : DRW_Entity(d) {
		eType = DRW::DIMENSION;
		type = d.type;
		name = d.name;
		defPoint = d.defPoint;
		textPoint = d.textPoint;
		text = d.text;
		style = d.style;
		align = d.align;
		linesty = d.linesty;
		linefactor = d.linefactor;
		rot = d.rot;
		extPoint = d.extPoint;
		clonePoint = d.clonePoint;
		def1 = d.def1;
		def2 = d.def2;
		angle = d.angle;
		oblique = d.oblique;
		arcPoint = d.arcPoint;
		circlePoint = d.circlePoint;
		length = d.length;
		//RLZ needed a def value for this: hdir = ???
	}
	virtual ~DRW_Dimension() {}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	bool parseDwg(DRW::Version version, dwgBuffer *buf, dwgBuffer *sBuf);
	virtual bool parseDwg(DRW::Version version, dwgBuffer* buf, duint32 bs = 0) {
		DRW_UNUSED(version); DRW_UNUSED(buf); DRW_UNUSED(bs); return true;
	}

public:
	DRW_Coord getDefPoint() const { return defPoint; }      /*!< Definition point, code 10, 20 & 30 */
	void setDefPoint(const DRW_Coord p) { defPoint = p; }
	DRW_Coord getTextPoint() const { return textPoint; }    /*!< Middle point of text, code 11, 21 & 31 */
	void setTextPoint(const DRW_Coord p) { textPoint = p; }
	std::string getStyle() const { return style; }          /*!< Dimension style, code 3 */
	void setStyle(const std::string s) { style = s; }
	int getAlign() const { return align; }                 /*!< attachment point, code 71 */
	void setAlign(const int a) { align = a; }
	int getTextLineStyle() const { return linesty; }       /*!< Dimension text line spacing style, code 72, default 1 */
	void setTextLineStyle(const int l) { linesty = l; }
	std::string getText() const { return text; }            /*!< Dimension text explicitly entered by the user, code 1 */
	void setText(const std::string t) { text = t; }
	double getTextLineFactor() const { return linefactor; } /*!< Dimension text line spacing factor, code 41, default 1? */
	void setTextLineFactor(const double l) { linefactor = l; }
	double getDir() const { return rot; }                  /*!< rotation angle of the dimension text, code 53 (optional) default 0 */
	void setDir(const double d) { rot = d; }

	DRW_Coord getExtrusion() { return extPoint; }            /*!< extrusion, code 210, 220 & 230 */
	void setExtrusion(const DRW_Coord p) { extPoint = p; }
	std::string getName() { return name; }                   /*!< Name of the block that contains the entities, code 2 */
	void setName(const std::string s) { name = s; }
	//    int getType(){ return type;}                      /*!< Dimension type, code 70 */

protected:
	DRW_Coord getPt2() const { return clonePoint; }
	void setPt2(const DRW_Coord p) { clonePoint = p; }
	DRW_Coord getPt3() const { return def1; }
	void setPt3(const DRW_Coord p) { def1 = p; }
	DRW_Coord getPt4() const { return def2; }
	void setPt4(const DRW_Coord p) { def2 = p; }
	DRW_Coord getPt5() const { return circlePoint; }
	void setPt5(const DRW_Coord p) { circlePoint = p; }
	DRW_Coord getPt6() const { return arcPoint; }
	void setPt6(const DRW_Coord p) { arcPoint = p; }
	double getAn50() const { return angle; }      /*!< Angle of rotated, horizontal, or vertical dimensions, code 50 */
	void setAn50(const double d) { angle = d; }
	double getOb52() const { return oblique; }    /*!< oblique angle, code 52 */
	void setOb52(const double d) { oblique = d; }
	double getRa40() const { return length; }    /*!< Leader length, code 40 */
	void setRa40(const double d) { length = d; }
public:
	int type;                  /*!< Dimension type, code 70 */
private:
	std::string name;          /*!< Name of the block that contains the entities, code 2 */
	DRW_Coord defPoint;        /*!<  definition point, code 10, 20 & 30 (WCS) */
	DRW_Coord textPoint;       /*!< Middle point of text, code 11, 21 & 31 (OCS) */
	UTF8STRING text;           /*!< Dimension text explicitly entered by the user, code 1 */
	UTF8STRING style;          /*!< Dimension style, code 3 */
	int align;                 /*!< attachment point, code 71 */
	int linesty;               /*!< Dimension text line spacing style, code 72, default 1 */
	double linefactor;         /*!< Dimension text line spacing factor, code 41, default 1? (value range 0.25 to 4.00*/
	double rot;                /*!< rotation angle of the dimension text, code 53 */
	DRW_Coord extPoint;        /*!<  extrusion normal vector, code 210, 220 & 230 */

	double hdir;               /*!< horizontal direction for the dimension, code 51, default ? */
	DRW_Coord clonePoint;      /*!< Insertion point for clones (Baseline & Continue), code 12, 22 & 32 (OCS) */
	DRW_Coord def1;            /*!< Definition point 1for linear & angular, code 13, 23 & 33 (WCS) */
	DRW_Coord def2;            /*!< Definition point 2, code 14, 24 & 34 (WCS) */
	double angle;              /*!< Angle of rotated, horizontal, or vertical dimensions, code 50 */
	double oblique;            /*!< oblique angle, code 52 */

	DRW_Coord circlePoint;     /*!< Definition point for diameter, radius & angular dims code 15, 25 & 35 (WCS) */
	DRW_Coord arcPoint;        /*!< Point defining dimension arc, x coordinate, code 16, 26 & 36 (OCS) */
	double length;             /*!< Leader length, code 40 */

protected:
	dwgHandle dimStyleH;
	dwgHandle blockH;
};



#endif
