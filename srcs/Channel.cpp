#include "Channel.hpp"

Channel::Channel(std::string name, std::string creator) : _name(name)
{
	_members.push_back(creator);
	// verif param de bases
	_inviteOnly = false;
	_topicRights = false;
	_userLimit = -1;
}

Channel::~Channel() {}

std::string	Channel::getName() const { return _name; }

std::string	Channel::getTopic() const { return _topic; }
void		Channel::setTopic(std::string topic) { _topic = topic; }

bool		Channel::getInviteOnly() const { return _inviteOnly; }
void		Channel::setInviteOnly(bool mode) { _inviteOnly = mode; }

bool		Channel::getTopicRights() const { return _topicRights; }
void		Channel::setTopicRights(bool mode) { _topicRights = mode; }

std::string	Channel::getPassword() const { return _password; }
void		Channel::setPassword(std::string password) { _password = password; }

int			Channel::getUserLimit() const { return _userLimit; }
void		Channel::setUserLimit(int limit) { _userLimit = limit; }

std::vector<std::string> Channel::getMembers() const { return _members; }

void		Channel::addOperator(std::string nickname)
{
	// case sensitive ? a verif
	for (size_t i = 0; i < _operators.size(); i++) {
		if (_operators[i] == nickname)
			return ;
	}
	_operators.push_back(nickname);
}

void		Channel::rmOperator(std::string nickname)
{
	for (size_t i = 0; i < _operators.size(); i++) {
		if (_operators[i] == nickname) {
			_operators.erase(_operators.begin() + i);
			return;  //pas de doublons normalement
		}
	}
}

bool		Channel::isOperator(std::string nickname) const
{
	for (size_t i = 0; i < _operators.size(); i++) {
		if (_operators[i] == nickname)
			return true;
	}
	return false;
}

void		Channel::addMember(std::string nickname)
{
	// case sensitive ? a verif
	for (size_t i = 0; i < _members.size(); i++) {
		if (_members[i] == nickname)
			return ;
	}
	_members.push_back(nickname);
}

void		Channel::rmMember(std::string nickname)
{
	for (size_t i = 0; i < _members.size(); i++) {
		if (_members[i] == nickname) {
			_members.erase(_members.begin() + i);
			break;  //pas de doublons normalement
		}
	}
	// if member is also op
	for (size_t i = 0; i < _operators.size(); i++) {
		if (_operators[i] == nickname) {
			_operators.erase(_operators.begin() + i);
			return;  //pas de doublons normalement
		}
	}
}

bool		Channel::isMember(std::string nickname) const
{
	for (size_t i = 0; i < _members.size(); i++) {
		if (_members[i] == nickname)
			return true;
	}
	return false;
}
