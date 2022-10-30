CXX 		=g++
LIBS		=`root-config --libs`

CXXFLAGS 	=-g3 -std=c++11  `root-config --cflags` 
INCPATH     = include/

TARGET		= turn_on_productionv, resolution_production
all:	$(TARGET) 

turn_on_production : src/turn_on_production.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

turn_on_productionNoRunFilter : src/turn_on_productionNoRunFilter.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

turn_on_productionSCEt : src/turn_on_productionSCEt.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

turn_on_productionPFEt : src/turn_on_productionPFEt.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

turn_on_productionGenEt : src/turn_on_productionGenEt.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_production : src/resolution_production.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_production20GeV : src/resolution_production20GeV.cc 
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_productionSCEt : src/resolution_productionSCEt.cc
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_productionPFEt : src/resolution_productionPFEt.cc
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_productionGenEt : src/resolution_productionGenEt.cc
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

resolution_productionGen_PFEt : src/resolution_productionGen_PFEt.cc
	$(CXX) $(CXXFLAGS) -I$(INCPATH) -o $@.exe $<  $(LIBS)

clean :
	@rm *.exe
