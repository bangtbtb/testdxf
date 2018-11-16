#ifndef DRW_VIEWPORT_H
#define DRW_VIEWPORT_H

#include "drw_point.h"

//! Class to handle viewport entity
/*!
*  Class to handle viewport entity
*  @author Rallaz
*/
class DRW_Viewport : public DRW_Point {
	SETENTFRIENDS
public:
	DRW_Viewport() {
		eType = DRW::VIEWPORT;
		vpstatus = 0;
		pswidth = 205;
		psheight = 156;
		centerPX = 128.5;
		centerPY = 97.5;
	}

	virtual void applyExtrusion() {}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	double pswidth;           /*!< Width in paper space units, code 40 */
	double psheight;          /*!< Height in paper space units, code 41 */
	int vpstatus;             /*!< Viewport status, code 68 */
	int vpID;                 /*!< Viewport ID, code 69 */
	double centerPX;          /*!< view center point X, code 12 */
	double centerPY;          /*!< view center point Y, code 22 */
	double snapPX;          /*!< Snap base point X, code 13 */
	double snapPY;          /*!< Snap base point Y, code 23 */
	double snapSpPX;          /*!< Snap spacing X, code 14 */
	double snapSpPY;          /*!< Snap spacing Y, code 24 */
							  //TODO: complete in dxf
	DRW_Coord viewDir;        /*!< View direction vector, code 16, 26 & 36 */
	DRW_Coord viewTarget;     /*!< View target point, code 17, 27, 37 */
	double viewLength;        /*!< Perspective lens length, code 42 */
	double frontClip;         /*!< Front clip plane Z value, code 43 */
	double backClip;          /*!< Back clip plane Z value, code 44 */
	double viewHeight;        /*!< View height in model space units, code 45 */
	double snapAngle;         /*!< Snap angle, code 50 */
	double twistAngle;        /*!< view twist angle, code 51 */

private:
	duint32 frozenLyCount;
};//RLZ: missing 15,25, 72, 331, 90, 340, 1, 281, 71, 74, 110, 120, 130, 111, 121,131, 112,122, 132, 345,346, and more...


#endif
