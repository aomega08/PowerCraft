#include "stdafx.h"

int main() {
	return 0;
}

#ifdef WIN32
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)	{
	return main();
}
#endif
