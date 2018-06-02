CC := g++
FLAGS := -std=c++11 -Wall
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o
	$(CC) $(FLAGS) $(INCLUDE) $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o -o $@

$(BUILD_DIR)/Date.o: $(SRC_DIR)/Date.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Date.cpp

$(BUILD_DIR)/User.o: $(SRC_DIR)/User.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/User.cpp

$(BUILD_DIR)/Meeting.o: $(SRC_DIR)/Meeting.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Meeting.cpp

$(BUILD_DIR)/Storage.o: $(SRC_DIR)/Storage.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Storage.cpp

$(BUILD_DIR)/AgendaService.o: $(SRC_DIR)/AgendaService.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaService.cpp

$(BUILD_DIR)/AgendaUI.o: $(SRC_DIR)/AgendaUI.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/AgendaUI.cpp

$(BUILD_DIR)/Agenda.o: $(SRC_DIR)/Agenda.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $(SRC_DIR)/Agenda.cpp

clean:
	@del /q $(BUILD_DIR)
	@del /q $(BIN_DIR)

run:
	@./bin/main.exe

