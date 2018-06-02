#include "AgendaUI.hpp"
#include <iomanip>
#include <cstring>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;
using std::string;
//public:
AgendaUI::AgendaUI() {
	startAgenda();
};
void AgendaUI::OperationLoop(void) {
	std::string operation;
	while (1) {
		cout << "-----------------------------Agenda-----------------------------\n"
			 << "Action :\n"
			 << "l    - log in Agenda by user name and password\n"
			 << "r    - register an Agenda account\n"
			 << "q    - quit Agenda\n"
			 << "----------------------------------------------------------------\n"
			 << "\n"
			 << "Agenda :~$ ";
		cin >> operation;
		switch(operation[0]) {
			case 'l': userLogIn(); break;
			case 'r': userRegister(); break;
			case 'q': quitAgenda(); break;
			default: break;
		}
		if (operation[0] == 'q') break;
	}
};

//private:
/**
* constructor
*/
void AgendaUI::startAgenda(void) {
	m_agendaService.startAgenda();
};

/**
* catch user's operation
* @return the operation
*/
std::string AgendaUI::getOperation() {
	cout << "-----------------------------Agenda-----------------------------\n"
		 << "Action :\n"
		 << "o    - log out Agenda\n"
		 << "dc   - delete Agenda account\n"
		 << "lu   - list all Agenda user\n"
		 << "cm   - create a meeting\n"
		 << "amp  - add meeting participator\n"
		 << "rmp  - remove meeting participator\n"
		 << "rqm  - request to quit meeting\n"
		 << "la   - list all meetings\n"
		 << "las  - list all sponsor meetings\n"
		 << "lap  - list all participator meetings\n"
		 << "qm   - query meeting by title\n"
		 << "qt   - query meeting by time interval\n"
		 << "dm   - delete meeting by time\n"
		 << "da   - delete all meetings\n"
		 << "----------------------------------------------------------------\n"
		 << "\n"
		 << "Agenda@" << m_userName << " :~# ";
	std::string operation;
	cin >> operation;
	return operation;
};

/**
* execute the operation
* @return if the operationloop continue
*/
bool AgendaUI::executeOperation(const std::string &t_operation) {
	if (t_operation == "o") {
		userLogOut();
		return false;
	}
	else if (t_operation == "dc") {
		deleteUser();
		return false;
	}
	else if (t_operation == "lu") {
		listAllUsers();
		return true;
	}
	else if (t_operation == "cm") {
		createMeeting();
		return true;
	}
	else if (t_operation == "amp") {
		addMeetingParticipator();
		return true;
	}
	else if (t_operation == "rmp") {
		removeMeetingParticipator();
		return true;
	}
	else if (t_operation == "rqm") {
		quitMeeting();
		return true;
	}
	else if (t_operation == "la") {
		listAllMeetings();
		return true;
	}
	else if (t_operation == "las") {
		listAllSponsorMeetings();
		return true;
	}
	else if (t_operation == "lap") {
		listAllParticipateMeetings();
		return true;
	}
	else if (t_operation == "qm") {
		queryMeetingByTitle();
		return true;
	}
	else if (t_operation == "qt") {
		queryMeetingByTimeInterval();
		return true;
	}
	else if (t_operation == "dm") {
		deleteMeetingByTitle();
		return true;
	}
	else if (t_operation == "da") {
		deleteAllMeetings();
		return true;
	}
	return true;
};

/**
* user Login
*/
void AgendaUI::userLogIn(void) {
	cout << "\n"
		 << "[log in] [username] [password]\n"
		 << "[log in] ";
	std::string t_username, t_password;
	cin >> t_username >> t_password;
	if (m_agendaService.userLogIn(t_username, t_password)) {
		m_userName = t_username;
		m_userPassword = t_password;
		cout << "[log in] succeed!\n\n";
		while(executeOperation(getOperation()));
	}
	else
		cout << "[log in] Password error or user doesn't exist!\n\n";
};

/**
* user regist
*/
void AgendaUI::userRegister(void) {
	cout << "\n"
		 << "[register] [username] [password] [email] [phone]\n"
		 << "[register] ";
	std::string t_username, t_password, t_email, t_phone;
	cin >> t_username >> t_password >> t_email >> t_phone;
	if (m_agendaService.userRegister(t_username, t_password, t_email, t_phone))
		cout << "[register] succeed!\n\n";
	else
		cout << "[register] This username has been registered!\n\n";
};

/**
* user logout
*/
void AgendaUI::userLogOut(void) {
	m_userName.clear();
	m_userPassword.clear();
};

/**
* quit the Agenda
*/
void AgendaUI::quitAgenda(void) {
	m_agendaService.quitAgenda();
};

/**
* delete a user from storage
*/
void AgendaUI::deleteUser(void) {
	if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
		cout << "[delete agenda account] succeed!\n\n";
		userLogOut();
	}
	else
		cout << "[delete agenda account] error!\n\n";
};

/**
* list all users from storage
*/
void AgendaUI::listAllUsers(void) {
	cout << "\n"
		 << "[list all users]\n"
		 << "\n";
	auto user_list = m_agendaService.listAllUsers();
	unsigned max_name_length = string("name").size();
	unsigned max_email_length = string("email").size();
	unsigned max_phone_length = string("phone").size();
	for (auto it : user_list) {
		max_name_length = (it.getName()).size() > max_name_length ? (it.getName()).size() : max_name_length;
		max_email_length = (it.getEmail()).size() > max_email_length ? (it.getEmail()).size() : max_email_length;
		max_phone_length = (it.getPhone()).size() > max_phone_length ? (it.getPhone()).size() : max_phone_length;
	}
	cout << std::left << std::setw(max_name_length+3) << "name"
		 << std::setw(max_email_length+3) << "email"
		 << std::setw(max_phone_length+3) << "phone" << endl;
	for (auto it : user_list) {
		cout << std::left << std::setw(max_name_length+3) << it.getName()
			 << std::setw(max_email_length+3) << it.getEmail()
			 << std::setw(max_phone_length+3) << it.getPhone() << endl;
	}
	cout << endl;
};

/**
* user create a meeting with someone else
*/
void AgendaUI::createMeeting(void) {
	cout << "\n";
	int participators_num = 0;
	string par_num_str;
	par_num_enter: cout << "[create meeting] [the number of participators]\n"
		 << "[create meeting] ";
	cin >> par_num_str;
	for (auto it : par_num_str) {
		if(!isdigit(it)) {
			cout << "[error] Invalid number! Please try again!\n";
			goto par_num_enter;
		}
	}
	participators_num = atoi(par_num_str.c_str());
	if (participators_num == 0) {
		cout << "[error] Invalid number! Please try again!\n";
		goto par_num_enter;
	}
	std::vector<string> participators_vec;
	for (int i = 1; i <= participators_num; ++i) {
		string t_par;
		cout << "[create meeting] [please enter the participator " << i << " ]\n"
			 << "[create meeting] ";
		cin >> t_par;
		participators_vec.push_back(t_par);
	}
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
		 << "[create meeting] ";
	string t_title, t_stime, t_etime;
	cin >> t_title >> t_stime >> t_etime;
	if (m_agendaService.createMeeting(m_userName, t_title, t_stime, t_etime, participators_vec))
		cout << "[create meeting] succeed!\n\n";
	else
		cout << "[create meeting] error!\n\n";
};

/**
* sponsor add a participator to the meeting
*/
void AgendaUI::addMeetingParticipator(void) {
	cout << "\n"
		 << "[add participator] [meeting title] [participator username]\n"
		 << "[add participator] ";
	string t_title, t_par_name;
	cin >> t_title >> t_par_name;
	if (m_agendaService.addMeetingParticipator(m_userName, t_title, t_par_name))
		cout << "[add participator] succeed!\n\n";
	else
		cout << "[add participator] error!\n\n";
};

/**
* sponsor add a participator to the meeting
*/
void AgendaUI::removeMeetingParticipator(void) {
	cout << "\n"
		 << "[remove participator] [meeting title] [participator username]\n"
		 << "[remove participator] ";
	string t_title, t_par_name;
	cin >> t_title >> t_par_name;
	if (m_agendaService.removeMeetingParticipator(m_userName, t_title, t_par_name))
		cout << "[remove participator] succeed!\n\n";
	else
		cout << "[remove participator] error!\n\n";
};

/**
* user quit from meeting
*/
void AgendaUI::quitMeeting(void) {
	cout << "\n"
		 << "[quit meeting] [meeting title]\n"
		 << "[quit meeting] ";
	string t_title;
	cin >> t_title;
	if (m_agendaService.quitMeeting(m_userName, t_title))
		cout << "[quit meeting] succeed!\n\n";
	else
		cout << "[quit meeting] error!\n\n";
};

/**
* list all meetings from storage
*/
void AgendaUI::listAllMeetings(void) {
	cout << "\n"
		 << "[list all meetings]\n"
		 << "\n";
	printMeetings(m_agendaService.listAllMeetings(m_userName));
};

/**
* list all meetings that this user sponsored
*/
void AgendaUI::listAllSponsorMeetings(void) {
	cout << "\n"
		 << "[list all sponsor meetings]\n"
		 << "\n";
	printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
};

/**
* list all meetings that this user take part in
*/
void AgendaUI::listAllParticipateMeetings(void) {
	cout << "\n"
		 << "[list all participator meetings]\n"
		 << "\n";
	printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
};

/**
* search meetings by title from storage
*/
void AgendaUI::queryMeetingByTitle(void) {
	cout << "\n"
		 << "[query meetings] [title]\n"
		 << "[query meetings] ";
	string t_title;
	cin >> t_title;
	printMeetings(m_agendaService.meetingQuery(m_userName, t_title));
};

/**
* search meetings by timeinterval from storage
*/
void AgendaUI::queryMeetingByTimeInterval(void) {
	cout << "\n"
		 << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
		 << "[query meetings] ";
	string t_stime, t_etime;
	cin >> t_stime >> t_etime;
	printMeetings(m_agendaService.meetingQuery(m_userName, t_stime, t_etime));
};

/**
* delete meetings by title from storage
*/
void AgendaUI::deleteMeetingByTitle(void) {
	cout << "\n"
		 << "[delete meeting] [title]\n"
		 << "[delete meeting] ";
	string t_title;
	cin >> t_title;
	if (m_agendaService.deleteMeeting(m_userName, t_title))
		cout << "[delete meeting] succeed!\n\n";
	else
		cout << "[error] delete meeting fail!\n\n";
};

/**
* delete all meetings that this user sponsored
*/
void AgendaUI::deleteAllMeetings(void) {
	m_agendaService.deleteAllMeetings(m_userName);
	cout << "\n"
		 << "[delete all meetings] succeed!\n"
		 << "\n";
};

/**
* show the meetings in the screen
*/
void AgendaUI::printMeetings(const std::list<Meeting> &t_meetings) {
	unsigned max_title_length = string("title").size();
	unsigned max_sponsor_length = string("sponsor").size();
	unsigned time_length = 16;
	for (auto it : t_meetings) {
		max_title_length = (it.getTitle()).size() > max_title_length ? (it.getTitle()).size() : max_title_length;
		max_sponsor_length = (it.getSponsor()).size() > max_sponsor_length ? (it.getSponsor()).size() : max_sponsor_length;
	}
	cout << std::left << std::setw(max_title_length+3) << "title"
		 << std::setw(max_sponsor_length+3) << "sponsor"
		 << std::setw(time_length+3) << "start time"
		 << std::setw(time_length+3) << "end time"
		 << "participators" << endl;
	for (auto it : t_meetings) {
		cout << std::left << std::setw(max_title_length+3) << it.getTitle()
			 << std::setw(max_sponsor_length+3) << it.getSponsor()
			 << std::setw(time_length+3) << Date::dateToString(it.getStartDate())
			 << std::setw(time_length+3) << Date::dateToString(it.getEndDate());
		auto par_vec = it.getParticipator();
		int par_size = par_vec.size();
		if(!par_vec.empty()) {
			for (auto itvec : par_vec) {
				cout << itvec;
				par_size--;
				if (par_size > 0) cout << ",";
			}
		}
		cout << "\n";
	}
	cout << "\n";
};