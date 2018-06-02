#include "AgendaUI.hpp"
#include <csignal>
AgendaUI aui;

void signalHandler(int signum) {
	aui.~AgendaUI();
    exit(0);  
}

int main() { 
	signal(SIGINT, signalHandler);
  	aui.OperationLoop();
  	return 0;
}
