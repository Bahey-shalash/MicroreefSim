OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17 
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
OFILES = shape.o message.o lifeform.o CircularLifeform.o SegmentLifeform.o algae.o coral.o scavenger.o simulation.o DrawingArea.o SimulationWindow.o main.o

all: $(OUT)

shape.o: shape.cpp shape.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cpp message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

lifeform.o: Lifeform.cpp Lifeform.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

CircularLifeform.o: CircularLifeform.cpp CircularLifeform.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

SegmentLifeform.o: SegmentLifeform.cpp SegmentLifeform.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

algae.o: Algae.cpp Algae.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

coral.o: Coral.cpp Coral.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

scavenger.o: Scavenger.cpp Scavenger.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

simulation.o: Simulation.cpp Simulation.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)


#GraphicsController.o: GraphicsController.cpp GraphicsController.h
#	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

DrawingArea.o: DrawingArea.cpp DrawingArea.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

SimulationWindow.o: SimulationWindow.cpp SimulationWindow.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "Cleaning compilation files"
	@rm *.o $(OUT) *.cpp~ *.h~
