#include <stdio.h>
#include "hiscore.h"

static const char *hiscore_file = "hiscore";

int hiscore_read()
{
        int hi;
        FILE *fp;

        fp = fopen(hiscore_file, "rb");
        if (!fp)
		return 0;

	fread(&hi, sizeof(hi), 1, fp);
	fclose(fp);

        return hi;
}

void hiscore_write(int hi)
{
	FILE *fp;

	fp = fopen(hiscore_file, "wb");
	if (!fp)
		return;

	fwrite(&hi, sizeof(hi), 1, fp);
	fclose(fp);
}
