#ifndef DRW_IMAGE_H
#define DRW_IMAGE_H

#include "drw_line.h"
//! Class to handle image entity
/*!
*  Class to handle image entity
*  @author Rallaz
*/
class DRW_Image : public DRW_Line {
	SETENTFRIENDS
public:
	DRW_Image() {
		eType = DRW::IMAGE;
		fade = clip = 0;
		brightness = contrast = 50;
	}

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	duint32 ref;               /*!< Hard reference to imagedef object, code 340 */
	DRW_Coord vVector;         /*!< V-vector of single pixel, x coordinate, code 12, 22 & 32 */
							   //    double vx;                 /*!< V-vector of single pixel, x coordinate, code 12 */
							   //    double vy;                 /*!< V-vector of single pixel, y coordinate, code 22 */
							   //    double vz;                 /*!< V-vector of single pixel, z coordinate, code 32 */
	double sizeu;              /*!< image size in pixels, U value, code 13 */
	double sizev;              /*!< image size in pixels, V value, code 23 */
	double dz;                 /*!< z coordinate, code 33 */
	int clip;                  /*!< Clipping state, code 280, 0=off 1=on */
	int brightness;            /*!< Brightness value, code 281, (0-100) default 50 */
	int contrast;              /*!< Brightness value, code 282, (0-100) default 50 */
	int fade;                  /*!< Brightness value, code 283, (0-100) default 0 */

};


#endif
