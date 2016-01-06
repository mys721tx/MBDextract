/*
 * mbdextract.c: extract game scripts from MBD files.
 *
 * Copyright (c) 2014-2015 Yishen Miao
 *
 * This file is part of MBDextract
 *
 * MBDextract is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * MBDextract is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBDextract.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dicts.h"

const char *ver_str = PACKAGE_STRING;
const char *bugreport = PACKAGE_BUGREPORT;

void printUsage(char pname[])
{
	printf("Version: %s\nUsage: %s file\n\nplease send bug reports to <%s>.\n", 
			ver_str, pname, bugreport);
}

int main(int argc, char *argv[])
{
	FILE *ifp;
	const char *mode = "rb";
	char *fbuff, *mbdp, *p, *tmp[8];
	unsigned long ifp_len;
	int i;
	unsigned long offset;

	const char magicnum[] = {0x4d, 0x42, 0x44, 0x00};
	

	if (argc < 2) {
		printUsage(argv[0]);
		return 0;
	}

	ifp = fopen(argv[1], mode);
	if (!ifp) {
		fprintf(stderr, "Unable to open %s: %s\n", 
				argv[1], strerror(errno));
		return errno;
	}

	fseek(ifp, 0 * sizeof(char), SEEK_END);
	ifp_len = ftell(ifp);
	rewind(ifp);

	fbuff = (char *)malloc(ifp_len + 1 * sizeof(char));
	if (!fbuff) {
		fprintf(stderr, "Unable to allocate memory: %s\n", 
				strerror(errno));
		fclose(ifp);
		return errno;
	}

	fread(fbuff, ifp_len, 1 * sizeof(char), ifp);
	fclose(ifp);

	mbdp = memmem(fbuff, ifp_len, magicnum, 4 * sizeof(char));
	if (!mbdp) {
		fprintf(stderr, "Cannot find MBD header.\n");
		return 1;
	}

	offset = (int)(mbdp - fbuff);

	for (i = offset * sizeof(char); i < ifp_len; i += 4 * sizeof(char)) {
		memcpy(tmp, &fbuff[i], 8 * sizeof(char));
		p = memmem(lhash, 256 * 8 * sizeof(char), tmp, 8 * sizeof(char));
		if (p) {
			i += 8 * sizeof(char);
			printf("%s", ldict[(int)(p - lhash) / 8 * sizeof(char)]);
		}
		
		memcpy(tmp, &fbuff[i], 4 * sizeof(char));
		p = memmem(shash, 115 * 4 * sizeof(char), tmp, 4 * sizeof(char));
		if (p) {
			printf("%s", sdict[(int)(p - shash) / 4 * sizeof(char)]);
		}
	}

	printf("\n");

	free(fbuff);

	return 0;
}
