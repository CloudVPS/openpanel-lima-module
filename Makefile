# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

include makeinclude

OBJ	= main.o version.o

all: module.xml limamodule.exe
	grace mkapp limamodule
	@mkdir -p tmp

module.xml: module.def
	mkmodulexml < module.def > module.xml

limamodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o limamodule.exe $(OBJ) \
	/usr/lib/opencore/libcoremodule.a $(LIBS)

version.cpp:
	grace mkversion version.cpp

clean:
	rm -f *.o *.exe
	rm -rf limamodule.app
	rm -f limamodule

makeinclude:
	@echo please run ./configure
	@false

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I../opencore/api/c++/include -c $<
