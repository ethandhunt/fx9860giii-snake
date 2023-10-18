#include <gint/display.h>
#include <gint/keyboard.h>

int main(void)
{
	dclear(C_WHITE);
	dtext(1, 1, C_BLACK, "Sample fxSDK add-in.");
	dupdate();

	getkey();
	return 1;
}
