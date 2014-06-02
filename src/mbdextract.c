/*
 * mbdextract.c: extract game scripts from MBD files.
 *
 * Copyright (c) 2014 Yishen Miao
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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char *ver_str = PACKAGE_STRING;
const char *bugreport = PACKAGE_BUGREPORT;

void printUsage(char pname[])
{
	printf("%s\nUsage: %s file\n\nplease send bug reports to <%s>.\n", 
			ver_str, pname, bugreport);
}


int main(int argc, char *argv[])
{
	FILE *ifp;
	char *mode = "rb";
	char *fbuff;
	char *mbdp;
	unsigned long ifp_len;
	int i;
	unsigned long offset;
	char magicnum[4] = {0x4d, 0x42, 0x44, 0x00};
	

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

	fseek(ifp, 0, SEEK_END);
	ifp_len = ftell(ifp);
	rewind(ifp);

	fbuff = (char *)malloc(ifp_len+1);
	if (!fbuff) {
		fprintf(stderr, "Unable to allocate memory: %s\n", 
				strerror(errno));
		fclose(ifp);
		return errno;
	}

	fread(fbuff, ifp_len, 1, ifp);
	fclose(ifp);

	mbdp = memmem(fbuff, ifp_len, magicnum, 4);
	if (!mbdp) {
		fprintf(stderr, "Cannot find MBD header.\n");
		return 1;
	}

	offset = (int)(mbdp - fbuff);
	printf("<MBD>");
/*
 * The converation talbe comes from 
 * http://forums.qhimm.com/index.php?topic=8163.msg98625#msg98625
 *
 * 0x80000000 - 0x80001900 are capital case letters.
 * 0x80002000 - 0x80003300 are lower case letters.
 * 0x80003400 - 0x80003D00 are digits.
 * 0x80003E00 - 0x80005A00 are symbols.
 */

	for (i = offset + 4; i < ifp_len; i+= 4){
		switch (fbuff[i]) {
			case (char)0x01:
				printf("<STRING>\n");
				break;
			case (char)0x0c:
				if (fbuff[i - 4] <= (char)0xff) {
                    	    	    printf("<%dTEXT>", fbuff[i - 4] & ~ (~0 << 2));
			    	}
                		printf("<???TEXT>");
                		break;
			case (char)0x40:
				switch (fbuff[i + 1]) {
					case (char)0x02:
						printf("<NL>\n");
						break;
					case (char)0x03:
						printf("<NUK1>");
						break;
					case (char)0x04:
						printf("<END>\n\n");
						break;
					case (char)0x05:
						if (fbuff[i + 3] == (char)0x01) {
							printf("<CHOICE2>");
						} else {
							printf("<M-CHOICE%d>", fbuff[i + 3]);
						}
						break;
					case (char)0x06:
						printf("<CROSS-PAD>");
						break;
					case (char)0x07:
						printf("<CROSS-SQUARE>");
						break;
					case (char)0x10:
						printf("<CHOICE1>");
						break;
					case (char)0x11:
						printf("<VAR4>");
						break;
					case (char)0x12:
						printf("<VAR5>");
						break;
					case (char)0x2b:
						printf("<VAR1>");
						break;
					case (char)0x2c:
						printf("<VAR2>");
						break;
					case (char)0x2d:
						printf("<VAR3>");
						break;
					case (char)0x36:
						printf("<FUNCTION1>");
						break;
				}
				break;
			case (char)0x80:
				if (fbuff[i + 2] < (char)0x1a) {
					printf("%c", (char)((int)fbuff[i + 2] + 0x41));
				} else if (fbuff[i + 2] < (char)0x34) {
					printf("%c", (char)((int)fbuff[i + 2] + 0x47));
				} else if (fbuff[i + 2] < (char)0x3E) {
					printf("%c", (char)((int)fbuff[i + 2] - 0x04));
				} else {
					switch (fbuff[i + 2]) {
						case (char)0x3E:
							printf("!");
							break;
						case (char)0x3F:
							printf("?");
							break;
						case (char)0x40:
							printf(" ");
							break;
						case (char)0x41:
							printf("\"");
							break;
						case (char)0x42:
							printf("\'");
							break;
						case (char)0x43:
							printf(",");
							break;
						case (char)0x44:
							printf(".");
							break;
						case (char)0x45:
							printf(":");
							break;
						case (char)0x46:
							printf(";");
							break;
						case (char)0x47:
							printf("--");
							break;
						case (char)0x48:
							printf("/");
							break;
						case (char)0x49:
							printf("(");
							break;
						case (char)0x4A:
							printf(")");
							break;
						case (char)0x4B:
							printf("$");
							break;
						case (char)0x4C:
							printf("<");
							break;
						case (char)0x4D:
							printf(">");
							break;
						case (char)0x4E:
							printf("%%");
							break;
						case (char)0x4F:
							printf("[");
							break;
						case (char)0x50:
							printf("]");
							break;
						case (char)0x51:
							printf("YEN");
							break;
						case (char)0x52:
							printf("@Cruz");
							break;
						case (char)0x53:
							printf("#");
							break;
						case (char)0x54:
							printf("_");
							break;
						case (char)0x55:
							printf("|");
							break;
						case (char)0x56:
							printf("^");
							break;
						case (char)0x57:
							printf("Âº");
							break;
						case (char)0x58:
							printf("-");
							break;
						case (char)0x59:
							printf("+");
							break;
						case (char)0x5A:
							printf("=");
							break;
					}
				}
				break;
			}
		}

	free(fbuff);

	return 0;
}
