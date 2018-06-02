#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
using namespace std;
std::shared_ptr<Storage> Storage::m_instance = nullptr;
//private:
/**
*   default constructor
*/
Storage::Storage() {
	m_dirty = false;
	readFromFile();
};

/**
*   disallow the copy constructor and assign operator
*/
//Storage(const Storage &t_another) = delete;
//void operator=(const Storage &t_another) = delete;

/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
	//read users
	ifstream usersfin(Path::userPath);
	ifstream meetingsfin(Path::meetingPath);
	if ( !(usersfin.is_open() && meetingsfin.is_open()) )
		return false;

	string str1;
	while (getline(usersfin, str1)) {
		string t_name, t_password, t_email, t_phone;

		int pos = 1, len = 0;
		pos = str1.find("\"") + 1;
		len = str1.find("\",\"", pos) - pos;
		t_name = str1.substr(pos, len);

		pos += len + 3;
		len = str1.find("\",\"", pos) - pos;
		t_password = str1.substr(pos, len);

		pos += len + 3;
		len = str1.find("\",\"", pos) - pos;
		t_email = str1.substr(pos, len);

		pos += len + 3;
		len = str1.find("\"", pos) - pos;
		t_phone = str1.substr(pos, len);
		
		m_userList.push_back(User(t_name, t_password, t_email, t_phone));
	}
	usersfin.close();

	//read meetings
	//"maomao","a&b&c","2017-05-23/17:45","2017-05-24/17:44","maomao2"
	string str2;
	while (getline(meetingsfin, str2)) {
		string t_sponsor, str_participators, t_startDate, t_endDate, t_title;
		
		int pos = 1, len = 0;
		pos = str2.find("\"") + 1;
		len = str2.find("\",\"", pos) - pos;
		t_sponsor = str2.substr(pos, len);

		pos += len + 3;
		len = str2.find("\",\"", pos) - pos;
		str_participators = str2.substr(pos, len);
		str_participators += '&';

		pos += len + 3;
		len = str2.find("\",\"", pos) - pos;
		t_startDate = str2.substr(pos, len);

		pos += len + 3;
		len = str2.find("\",\"", pos) - pos;
		t_endDate = str2.substr(pos, len);

		pos += len + 3;
		len = str2.find("\"", pos) - pos;
		t_title = str2.substr(pos, len);

		//str_participators to vec_participators
		vector<string> vec_participators;
		int pos1 = 0, len1 = 0, str_participators_size = str_participators.size();
		while (pos1 < str_participators_size) {
			len1 = str_participators.find('&', pos1) - pos1;
			string _p = str_participators.substr(pos1, len1);
			vec_participators.push_back(_p);
			pos1 += len1 + 1;
		}

		Meeting ret(t_sponsor, vec_participators, Date::stringToDate(t_startDate), Date::stringToDate(t_endDate), t_title);
		m_meetingList.push_back(ret);
	}
	meetingsfin.close();
	return true;
};

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
	ofstream usersfout(Path::userPath);
	ofstream meetingsfout(Path::meetingPath);
	if ( !(usersfout.is_open() && meetingsfout.is_open()) )
		return false;
	//write users
	for (auto it : m_userList) {
		string s_out;
		s_out += "\"" + it.getName() + "\",\"" + it.getPassword() + "\",\""
				+ it.getEmail() + "\",\"" + it.getPhone() + "\"";
		usersfout << s_out << endl;
	}
	usersfout.close();
	//write meetings
	//"maomao","a&b&c","2017-05-23/17:45","2017-05-24/17:44","maomao2"
	for (auto it : m_meetingList) {
		string str_participators;
		auto vec = it.getParticipator();
		//must judge vec if empty
		if (!vec.empty()) {
			for (auto itvec = vec.begin(); itvec != vec.end(); ++itvec) {
				str_participators += *itvec;
				if (itvec != vec.end()-1)
					str_participators += '&';
			}
		}

		string s_out;
		s_out += "\"" + it.getSponsor() + "\",\"" + str_participators + "\",\"" 
				+ Date::dateToString(it.getStartDate()) + "\",\""
				+ Date::dateToString(it.getEndDate()) + "\",\"" + it.getTitle() + "\"";

		meetingsfout << s_out << endl;
	}
	meetingsfout.close();
	return true;
};

//public:
/**
* get Instance of storage
* @return the pointer of the instance
*/
//static
std::shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == nullptr) {
		m_instance = shared_ptr<Storage>(new Storage());
	}
	return m_instance;
};

/**
*   destructor
*/
Storage::~Storage() {
	sync();
	m_instance = nullptr;
};

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User &t_user) {
	m_userList.push_back(t_user);
	m_dirty = true;
};

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	std::list<User> retUserList;
	for (auto it : m_userList) {
		if (filter(it))
			retUserList.push_back(it);
	}
	return retUserList;
};

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
             std::function<void(User &)> switcher) {
	int count = 0;
	for (auto &it : m_userList) {
		if (filter(it)) {
			switcher(it);
			count++;
		}
	}
	if (count > 0)
		m_dirty = true;
	return count;
};

/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int count = 0;
	for (auto it = m_userList.begin(); it != m_userList.end(); ) {
		if (filter(*it)) {
			it = m_userList.erase(it);
			count++;
		}
		else
			it++;
	}
	if (count > 0)
		m_dirty = true;
	return count;
};

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting &t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = true;
};

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings
*/
std::list<Meeting> Storage::queryMeeting(
  std::function<bool(const Meeting &)> filter) const {
	std::list<Meeting> retMeetingList;
	for (auto it : m_meetingList) {
		if (filter(it))
			retMeetingList.push_back(it);
	}
	return retMeetingList;
};

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                std::function<void(Meeting &)> switcher) {
	int count = 0;
	for (auto &it : m_meetingList) {
		if (filter(it)) {
			switcher(it);
			count++;
		}
	}
	if (count > 0)
		m_dirty = true;
	return count;
};

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int count = 0;
	for (auto it = m_meetingList.begin(); it != m_meetingList.end(); ) {
		if (filter(*it)) {
			it = m_meetingList.erase(it);
			count++;
		}
		else
			it++;
	}
	if (count > 0)
		m_dirty = true;
	return count;
};

/**
* sync with the file
*/
bool Storage::sync(void) {
	m_dirty = false;
	return writeToFile();
};