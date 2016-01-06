cmake_minimum_required(VERSION 2.6)

project (MBDextract)

set (MBDextract_VERSION 0.2)
set (MBDextract_BUGREPORT "mys721tx\@gmail.com")

configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

add_executable (mbdextract mbdextract.c dicts.h dicts.c)

install(TARGETS mbdextract DESTINATION bin)
