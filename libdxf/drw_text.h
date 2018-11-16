
#ifndef DRW_TEXT_H
#define DRW_TEXT_H

#include "drw_line.h"

//! Class to handle insert entries
/*!
*  Class to handle insert entries
*  @author Rallaz
*/
class DRW_Text : public DRW_Line {
	SETENTFRIENDS
public:
	//! Vertical alignments.
	enum VAlign {
		VBaseLine = 0,  /*!< Top = 0 */
		VBottom,        /*!< Bottom = 1 */
		VMiddle,        /*!< Middle = 2 */
		VTop            /*!< Top = 3 */
	};

	//! Horizontal alignments.
	enum HAlign {
		HLeft = 0,     /*!< Left = 0 */
		HCenter,       /*!< Centered = 1 */
		HRight,        /*!< Right = 2 */
		HAligned,      /*!< Aligned = 3 (if VAlign==0) */
		HMiddle,       /*!< middle = 4 (if VAlign==0) */
		HFit           /*!< fit into point = 5 (if VAlign==0) */
	};

	DRW_Text() {
		eType = DRW::TEXT;
		angle = 0;
		widthscale = 1;
		oblique = 0;
		style = "STANDARD";
		textgen = 0;
		alignH = HLeft;
		alignV = VBaseLine;
	}

	virtual void applyExtrusion() {} //RLZ TODO

protected:
	void parseCode(int code, dxfReader *reader);
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	double height;             /*!< height text, code 40 */
	UTF8STRING text;           /*!< text string, code 1 */
	double angle;              /*!< rotation angle in degrees (360), code 50 */
	double widthscale;         /*!< width factor, code 41 */
	double oblique;            /*!< oblique angle, code 51 */
	UTF8STRING style;          /*!< style name, code 7 */
	int textgen;               /*!< text generation, code 71 */
	enum HAlign alignH;        /*!< horizontal align, code 72 */
	enum VAlign alignV;        /*!< vertical align, code 73 */
	dwgHandle styleH;          /*!< handle for text style */
};

#endif
