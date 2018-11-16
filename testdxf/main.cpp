#include <iostream>
#include "libdxfrw.h"
#include "ExampleWriter.h"
#include "inherite.h"
#include <direct.h>

int main()
{
	char cur[1024];
	_getcwd(cur, 1024);
	std::cout << "Current directory: " << cur << std::endl;
	dxfRW dxf("D:\test.dxf");
	ExampleWriter writer(dxf);
	bool res = dxf.write(&writer, DRW::Version::AC1027, false);
	std::cout << "Response of write: " << (res ? "true" : "false") << std::endl;

	system("pause");
	return 0;
}
