
#ifndef DRW_MTEXT_H
#define DRW_MTEXT_H

#include "drw_text.h"

//! Class to handle insert entries
/*!
*  Class to handle insert entries
*  @author Rallaz
*/
class DRW_MText : public DRW_Text {
	SETENTFRIENDS
public:
	//! Attachments.
	enum Attach {
		TopLeft = 1,
		TopCenter,
		TopRight,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		BottomLeft,
		BottomCenter,
		BottomRight
	};

	DRW_MText() {
		eType = DRW::MTEXT;
		interlin = 1;
		alignV = (VAlign)TopLeft;
		textgen = 1;
		haveXAxis = false;    //if true needed to recalculate angle
	}

protected:
	void parseCode(int code, dxfReader *reader);
	void updateAngle();    //recalculate angle if 'haveXAxis' is true
	virtual bool parseDwg(DRW::Version version, dwgBuffer *buf, duint32 bs = 0);

public:
	double interlin;     /*!< width factor, code 44 */
private:
	bool haveXAxis;
};


#endif
