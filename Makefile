include makeinclude

OBJ	= main.o version.o

all: module.xml limamodule.exe
	mkapp limamodule
	@mkdir -p tmp

module.xml: module.def
	mkmodulexml < module.def > module.xml

limamodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o limamodule.exe $(OBJ) \
	../opencore/api/c++/lib/libcoremodule.a $(LIBS)

version.cpp:
	mkversion version.cpp

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
