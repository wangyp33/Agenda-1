#include "AgendaService.hpp"


/**
 * constructor
 */
AgendaService::AgendaService() {
    startAgenda();
};

/**
 * destructor
 */
AgendaService::~AgendaService() {
    quitAgenda();
};

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    auto filter = [userName, password](const User &user) {
        return user.getName() == userName && user.getPassword() == password;
    };
    return (m_storage->queryUser(filter)).size() > 0;
};

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
    auto filter = [userName](const User &user) {
        return user.getName() == userName;
    };
    if ((m_storage->queryUser(filter)).size() == 0) {
        m_storage->createUser(User(userName, password, email, phone));
        return true;
    }
    else
        return false;
};

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto filter = [userName, password](const User &user) {
        return user.getName() == userName && user.getPassword() == password;
    };
    deleteAllMeetings(userName);
    auto t_par_meetings_list = listAllParticipateMeetings(userName);
    for (auto it : t_par_meetings_list)
        quitMeeting(userName, it.getTitle());
    return m_storage->deleteUser(filter) > 0;
};

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
    auto filter = [](const User &user) { return true; };
    return m_storage->queryUser(filter);
};

/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endDate the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {
    //test if sponsor has registored
    bool is_spon_regist = false;
    for (auto it : listAllUsers()) {
        if (it.getName() == userName) {
            is_spon_regist = true;
            break;
        }
    }
    if (!is_spon_regist)
        return false;
    //test the participator's size
    if (participator.size() == 0)
        return false;
    //test if the title is unique
    auto t_filter = [title](const Meeting &meeting) {
        return meeting.getTitle() == title;
    };
    if ((m_storage->queryMeeting(t_filter)).size() > 0)
        return false;
    //test the Date
    Date s_date(startDate);
    Date e_date(endDate);
    if (!Date::isValid(s_date) || !Date::isValid(e_date) || s_date >= e_date)
        return false;
    //test if there is time conflict of the sponsor
    auto t1_filter = [userName](const Meeting &meeting) {
        return meeting.getSponsor() == userName;
    };
    auto my_meeting_list = listAllMeetings(userName);
    for (auto it : my_meeting_list) {
        if (  !(it.getEndDate() <= s_date || it.getStartDate() >= e_date)  )
            return false;
    }
    //create meeting
    std::vector<std::string> t_empty_vec;
    m_storage->createMeeting(Meeting(userName, t_empty_vec, s_date, e_date, title));
    //add participators one by one
    bool _isParticipator = false;
    for (auto &it : participator) {
        _isParticipator = addMeetingParticipator(userName, title, it);
        if (_isParticipator == false) {
            m_storage->deleteMeeting(t_filter);
            return false;
        }
    }
    return true;
};

/**
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
    //test if participator has registored
    bool is_par_regist = false;
    for (auto it : listAllUsers()) {
        if (it.getName() == participator) {
            is_par_regist = true;
            break;
        }
    }
    if (!is_par_regist)
        return false;
    //test if there exist such a meeting
    auto t_meeting_list = m_storage->queryMeeting([&](const Meeting &meeting){
        return meeting.getSponsor() == userName && meeting.getTitle() == title;});
    if (t_meeting_list.size() == 0)
        return false;
    //get the meeting
    auto t_it = t_meeting_list.begin();
    auto t_meeting = *t_it;
    //test if there is time conflict
    auto participator_meeting_list = listAllMeetings(participator);
    for (auto it : participator_meeting_list) {
        if ( !(t_meeting.getEndDate() <= it.getStartDate() || t_meeting.getStartDate() >= it.getEndDate()) )
            return false;
    }
    //add the participator
    auto filter = [userName, title](const Meeting &meeting) {
        return meeting.getSponsor() == userName && meeting.getTitle() == title;
    };
    auto switcher = [participator](Meeting &meeting) {
        meeting.addParticipator(participator);
    };
    m_storage->updateMeeting(filter, switcher);
    return true;
};

/**
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &meeting) {
        if (meeting.getSponsor() != userName || meeting.getTitle() != title)
            return false;
        bool _isParticipator = false;
        for (auto it : meeting.getParticipator()) {
            if (it == participator) {
                _isParticipator = true;
                break;
            }
        }
        return _isParticipator;
    };
    auto switcher = [participator](Meeting &meeting) {
        meeting.removeParticipator(participator);
    };
    int temp = m_storage->updateMeeting(filter, switcher);
    auto delete_emptyMeeting_filter = [](const Meeting &t_meeting) {
        return (t_meeting.getParticipator()).size() == 0;
    };
    m_storage->deleteMeeting(delete_emptyMeeting_filter);

    return temp > 0;
};

/**
 * quit from a meeting
 * @param userName the current userName. no need to be the sponsor
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title](const Meeting &meeting) {
        if (meeting.getTitle() != title)
            return false;
        bool _isParticipator = false;
        for (auto it : meeting.getParticipator()) {
            if (it == userName) {
                _isParticipator = true;
                break;
            }
        }
        return _isParticipator;
    };
    auto switcher = [userName](Meeting &meeting) {
        meeting.removeParticipator(userName);
    };
    int temp = m_storage->updateMeeting(filter, switcher);
    auto delete_emptyMeeting_filter = [](const Meeting &t_meeting) {
        return (t_meeting.getParticipator()).size() == 0;
    };
    m_storage->deleteMeeting(delete_emptyMeeting_filter);
    return temp > 0;
};

/**
 * search a meeting by username and title
 * @param uesrName the sponsor's userName
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const {
    auto filter = [userName, title](const Meeting &meeting) {
        bool _isParticipator = false;
        for (auto it : meeting.getParticipator()) {
            if (it == userName) {
                _isParticipator = true;
                break;
            }
        }
        return (meeting.getSponsor() == userName || _isParticipator) && meeting.getTitle() == title;
    };
    return m_storage->queryMeeting(filter);
};
/**
 * search a meeting by username, time interval
 * @param uesrName the sponsor's userName
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
    //test the Dates
    Date s_date(startDate);
    Date e_date(endDate);
    if (!Date::isValid(s_date) || !Date::isValid(e_date) || s_date > e_date) {
        std::list<Meeting> empty_list;
        return empty_list;
    }
    auto filter = [userName, s_date, e_date](const Meeting &meeting) {
        bool _isParticipator = false;
        for (auto it : meeting.getParticipator()) {
            if (it == userName) {
                _isParticipator = true;
                break;
            }
        }
        return (meeting.getSponsor() == userName || _isParticipator) && (
            (meeting.getStartDate() >= s_date && meeting.getStartDate() <= e_date) ||
            (meeting.getEndDate() >= s_date && meeting.getEndDate() <= e_date) ||
            (s_date == e_date && meeting.getStartDate() <= s_date && meeting.getEndDate() >= s_date)   );
    };
    return m_storage->queryMeeting(filter);
};

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        bool _isParticipator = false;
        for (auto it : meeting.getParticipator()) {
            if (it == userName) {
                _isParticipator = true;
                break;
            }
        }
        return meeting.getSponsor() == userName || _isParticipator;
    };
    return m_storage->queryMeeting(filter);
};

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        return meeting.getSponsor() == userName;
    };
    return m_storage->queryMeeting(filter);
};

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
    auto filter = [userName](const Meeting &meeting) {
        for (auto it : meeting.getParticipator()) {
            if (it == userName)
                return true;
        }
        return false;
    };
    return m_storage->queryMeeting(filter);
};

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title](const Meeting &meeting) {
        return meeting.getSponsor() == userName && meeting.getTitle() == title;
    };
    return m_storage->deleteMeeting(filter) > 0;
};

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
    auto filter = [userName](const Meeting &meeting) {
        return meeting.getSponsor() == userName;
    };
    return m_storage->deleteMeeting(filter) > 0;
};

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
};

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
    m_storage->sync();
};