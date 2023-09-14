#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"

# include <string>
# include <vector>

class Channel {

	private :
		std::string		_name;
		std::string		_topic;
		bool			_inviteOnly;  	// -i
		bool			_topicRights; 	// -t
		std::string		_password;    	// -k
		int				_userLimit;   	// -l

		std::vector<Client *>	_operators;
		std::vector<Client *>	_members;
		std::vector<Client *>	_invited;

	public :
		Channel(std::string name, Client *creator);
		~Channel();

		std::string	getName() const;
		std::string	getTopic() const;
		void		setTopic(std::string topic);
		bool		getInviteOnly() const;
		void		setInviteOnly(bool mode);
		bool		getTopicRights() const;
		void		setTopicRights(bool mode);
		std::string	& getPassword();
		void		setPassword(std::string password);
		int			getUserLimit() const;
		void		setUserLimit(int limit);

		std::vector<Client *> getMembers() const;

		void		addOperator(Client *client);
		void		rmOperator(Client *client);
		bool		isOperator(Client *client) const;

		void		addMember(Client *client);
		void		rmMember(Client *client);
		bool		isMember(Client *client) const;

		void		addInvited(Client *client);
		void		rmInvited(Client *client);
		bool		isInvited(Client *client) const;

};

#endif
