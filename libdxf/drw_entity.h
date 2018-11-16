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

#ifndef DRW_ENTITIES_H
#define DRW_ENTITIES_H


#include <string>
#include <vector>
#include <list>
#include "drw_base.h"

class dxfReader;
class dwgBuffer;
class DRW_Polyline;

namespace DRW {

	//! Entity's type.
	enum ETYPE {
		E3DFACE,
		//        E3DSOLID, //encripted propietry data
		//        ACAD_PROXY_ENTITY,
		ARC,
		//        ATTDEF,
		//        ATTRIB,
		BLOCK,// and ENDBLK
//        BODY, //encripted propietry data
CIRCLE,
DIMENSION,
DIMALIGNED,
DIMLINEAR,
DIMRADIAL,
DIMDIAMETRIC,
DIMANGULAR,
DIMANGULAR3P,
DIMORDINATE,
ELLIPSE,
HATCH,
//        HELIX,
IMAGE,
INSERT,
LEADER,
//        LIGHT,
LINE,
LWPOLYLINE,
//        MESH,
//        MLINE,
//        MLEADERSTYLE,
//        MLEADER,
MTEXT,
//        OLEFRAME,
//        OLE2FRAME,
POINT,
POLYLINE,
RAY,
//        REGION, //encripted propietry data
//        SECTION,
//        SEQEND,//not needed?? used in polyline and insert/attrib and dwg
//        SHAPE,
SOLID,
SPLINE,
//        SUN,
//        SURFACE, //encripted propietry data can be four types
//        TABLE,
TEXT,
//        TOLERANCE,
TRACE,
UNDERLAY,
VERTEX,
VIEWPORT,
//        WIPEOUT, //WIPEOUTVARIABLE
XLINE,
UNKNOWN
	};

}
//only in DWG: MINSERT, 5 types of vertex, 4 types of polylines: 2d, 3d, pface & mesh
//shape, dictionary, MLEADER, MLEADERSTYLE

#define SETENTFRIENDS  friend class dxfRW; \
                       friend class dwgReader;

//! Base class for entities
/*!
*  Base class for entities
*  @author Rallaz
*/
class DRW_Entity {
	SETENTFRIENDS
public:
	//initializes default values
		//handles: default no handle (0), color: default BYLAYER (256), 24 bits color: default -1 (not set)
		//line weight: default BYLAYER  (dxf -1, dwg 29), space: default ModelSpace (0)
	DRW_Entity() : eType(DRW::UNKNOWN), handle(DRW::NoHandle), parentHandle(DRW::NoHandle), appData(0),
		space(DRW::ModelSpace), layer("0"), lineType("BYLAYER"), material(DRW::MaterialByLayer),
		color(DRW::ColorByLayer), lWeight(DRW_LW_Conv::widthByLayer), ltypeScale(1.0), visible(true),
		numProxyGraph(0), proxyGraphics(std::string()), color24(-1), colorName(std::string()),
		transparency(DRW::Opaque), plotStyle(DRW::DefaultPlotStyle), shadow(DRW::CastAndReceieveShadows),
		haveExtrusion(false), extData(), haveNextLinks(0), plotFlags(0), ltFlags(0), materialFlag(0),
		shadowFlag(0), lTypeH(dwgHandle()), layerH(dwgHandle()), nextEntLink(0), prevEntLink(0),
		ownerHandle(false), xDictFlag(0), numReactors(0), objSize(0), oType(0), extAxisX(DRW_Coord()),
		extAxisY(DRW_Coord()), curr(NULL) {}

	DRW_Entity(const DRW_Entity& e) {
		eType = e.eType;
		handle = e.handle;
		parentHandle = e.parentHandle; //no handle (0)
		lineType = e.lineType;
		color = e.color; // default BYLAYER (256)
		ltypeScale = e.ltypeScale;
		visible = e.visible;
		layer = e.layer;
		lWeight = e.lWeight;
		space = e.space;
		haveExtrusion = e.haveExtrusion;
		color24 = e.color24; //default -1 not set
		numProxyGraph = e.numProxyGraph;
		shadow = e.shadow;
		material = e.material;
		plotStyle = e.plotStyle;
		transparency = e.transparency;
		nextEntLink = e.nextEntLink;
		prevEntLink = e.prevEntLink;
		numReactors = e.numReactors;
		xDictFlag = e.xDictFlag;
		curr = NULL;
		ownerHandle = false;
		for (std::vector<DRW_Variant*>::const_iterator it = e.extData.begin(); it != e.extData.end(); ++it) {
			extData.push_back(new DRW_Variant(*(*it)));
		}
	}

	virtual ~DRW_Entity() {
		for (std::vector<DRW_Variant*>::iterator it = extData.begin(); it != extData.end(); ++it)
			delete *it;

		extData.clear();
	}

	void reset() {
		for (std::vector<DRW_Variant*>::iterator it = extData.begin(); it != extData.end(); ++it)
			delete *it;
		extData.clear();
	}

	virtual void applyExtrusion() = 0;

protected:
	//parses dxf pair to read entity
	bool parseCode(int code, dxfReader *reader);
	//calculates extrusion axis (normal vector)
	void calculateAxis(DRW_Coord extPoint);
	//apply extrusion to @extPoint and return data in @point
	void extrudePoint(DRW_Coord extPoint, DRW_Coord *point);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0) { return false; };
	//parses dwg common start part to read entity
	bool parseDwg(DRW::Version version, dwgBuffer *buf, dwgBuffer* strBuf, duint32 bs = 0);
	//parses dwg common handles part to read entity
	bool parseDwgEntHandle(DRW::Version version, dwgBuffer *buf);

	//parses dxf 102 groups to read entity
	bool parseDxfGroups(int code, dxfReader *reader);

public:
	enum DRW::ETYPE eType;     /*!< enum: entity type, code 0 */
	duint32 handle;            /*!< entity identifier, code 5 */
	duint32 parentHandle;      /*!< Soft-pointer ID/handle to owner BLOCK_RECORD object, code 330 */
	std::list<std::list<DRW_Variant> > appData; /*!< list of application data, code 102 */
	DRW::Space space;          /*!< space indicator, code 67*/
	UTF8STRING layer;          /*!< layer name, code 8 */
	UTF8STRING lineType;       /*!< line type, code 6 */
	duint32 material;          /*!< hard pointer id to material object, code 347 */
	int color;                 /*!< entity color, code 62 */
	enum DRW_LW_Conv::lineWidth lWeight; /*!< entity lineweight, code 370 */
	double ltypeScale;         /*!< linetype scale, code 48 */
	bool visible;              /*!< entity visibility, code 60 */
	int numProxyGraph;         /*!< Number of bytes in proxy graphics, code 92 */
	std::string proxyGraphics; /*!< proxy graphics bytes, code 310 */
	int color24;               /*!< 24-bit color, code 420 */
	std::string colorName;     /*!< color name, code 430 */
	int transparency;          /*!< transparency, code 440 */
	int plotStyle;             /*!< hard pointer id to plot style object, code 390 */
	DRW::ShadowMode shadow;    /*!< shadow mode, code 284 */
	bool haveExtrusion;        /*!< set to true if the entity have extrusion*/
	std::vector<DRW_Variant*> extData; /*!< FIFO list of extended data, codes 1000 to 1071*/

protected: //only for read dwg
	duint8 haveNextLinks; //aka nolinks //B
	duint8 plotFlags; //presence of plot style //BB
	duint8 ltFlags; //presence of linetype handle //BB
	duint8 materialFlag; //presence of material handle //BB
	duint8 shadowFlag; //presence of shadow handle ?? (in dwg may be plotflag)//RC
	dwgHandle lTypeH;
	dwgHandle layerH;
	duint32 nextEntLink;
	duint32 prevEntLink;
	bool ownerHandle;

	duint8 xDictFlag;
	dint32 numReactors; //
	duint32 objSize;  //RL 32bits object data size in bits
	dint16 oType;

private:
	DRW_Coord extAxisX;
	DRW_Coord extAxisY;
	DRW_Variant* curr;
};


//! Class to handle hatch loop
/*!
*  Class to handle hatch loop
*  @author Rallaz
*/
class DRW_HatchLoop {
public:
	DRW_HatchLoop(int t) {
		type = t;
		numedges = 0;
	}

	~DRW_HatchLoop() {
		/*        while (!pollist.empty()) {
				   pollist.pop_back();
				 }*/
		while (!objlist.empty()) {
			objlist.pop_back();
		}
	}

	void update() {
		numedges = objlist.size();
	}

public:
	int type;               /*!< boundary path type, code 92, polyline=2, default=0 */
	int numedges;           /*!< number of edges (if not a polyline), code 93 */
//TODO: store lwpolylines as entities
//    std::vector<DRW_LWPolyline *> pollist;  /*!< polyline list */
	std::vector<DRW_Entity *> objlist;      /*!< entities list */
};



//used  //DRW_Coord basePoint;      /*!<  base point, code 10, 20 & 30 */

//double thickness;         /*!< thickness, code 39 */
//DRW_Coord extPoint;       /*!<  Dir extrusion normal vector, code 210, 220 & 230 */
//enum DRW::ETYPE eType;     /*!< enum: entity type, code 0 */
//duint32 handle;            /*!< entity identifier, code 5 */
//std::list<std::list<DRW_Variant> > appData; /*!< list of application data, code 102 */
//duint32 parentHandle;      /*!< Soft-pointer ID/handle to owner BLOCK_RECORD object, code 330 */
//DRW::Space space;          /*!< space indicator, code 67*/
//UTF8STRING layer;          /*!< layer name, code 8 */
//UTF8STRING lineType;       /*!< line type, code 6 */
//duint32 material;          /*!< hard pointer id to material object, code 347 */
//int color;                 /*!< entity color, code 62 */
//enum DRW_LW_Conv::lineWidth lWeight; /*!< entity lineweight, code 370 */
//double ltypeScale;         /*!< linetype scale, code 48 */
//bool visible;              /*!< entity visibility, code 60 */
//int numProxyGraph;         /*!< Number of bytes in proxy graphics, code 92 */
//std::string proxyGraphics; /*!< proxy graphics bytes, code 310 */
//int color24;               /*!< 24-bit color, code 420 */
//std::string colorName;     /*!< color name, code 430 */
//int transparency;          /*!< transparency, code 440 */
//int plotStyle;             /*!< hard pointer id to plot style object, code 390 */
//DRW::ShadowMode shadow;    /*!< shadow mode, code 284 */
//bool haveExtrusion;        /*!< set to true if the entity have extrusion*/

#endif

// EOF

