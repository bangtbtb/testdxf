#include <iostream>
#include "libdxfrw.h"
#include "ExampleWriter.h"
#include "inherite.h"
#include <direct.h>
#include <drw_entities.h>
int main()
{
	char cur[1024];
	_getcwd(cur, 1024);
	std::cout << "Current directory: " << cur << std::endl;
	dxfRW dxf("./test.dxf");
	ExampleWriter writer(dxf);

	DRW_Circle *cir = new DRW_Circle();
	cir->basePoint = DRW_Coord(0, 5, 0);
	cir->radious = 5;

	DRW_Line *line = new DRW_Line();
	line->basePoint = DRW_Coord(0, 5, 0);
	line->secPoint = DRW_Coord(5, 5, 0);

	DRW_Arc *arc = new DRW_Arc();
	arc->basePoint = DRW_Coord(-8, 5, 0);
	arc->radious = 5;
	arc->isccw = true;
	arc->staangle = -1;
	arc->endangle = 5;

	DRW_Arc *arc1 = new DRW_Arc();
	arc1->basePoint = DRW_Coord(8, 5, 0);
	arc1->radious = 5;
	arc1->isccw = true;
	arc1->staangle = 1;
	arc1->endangle = 3.1;

	writer.addEntity(cir);
	writer.addEntity(line);
	writer.addEntity(arc);
	writer.addEntity(arc1);

	bool res = dxf.write(&writer, DRW::Version::AC1027, false);
	std::cout << "Response of write: " << (res ? "true" : "false") << std::endl;

	system("pause");
	return 0;
}
