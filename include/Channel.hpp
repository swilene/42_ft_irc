#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
		std::vector<std::string>	_operators; // -o
		std::vector<std::string>	_members;   //save nickname

	public :
		Channel(std::string name, std::string creator);  //envoyer password?
		~Channel();

		std::string	getName() const;  //pas de setter
		std::string	getTopic() const;
		void		setTopic(std::string topic);
		bool		getInviteOnly() const;
		void		setInviteOnly(bool mode);
		bool		getTopicRights() const;
		void		setTopicRights(bool mode);
		std::string	getPassword() const;
		void		setPassword(std::string password);
		int			getUserLimit() const;
		void		setUserLimit(int limit);

		std::vector<std::string> getMembers() const;

		void		addOperator(std::string nickname);
		void		rmOperator(std::string nickname);
		bool		isOperator(std::string nickname) const;

		void		addMember(std::string nickname);
		void		rmMember(std::string nickname);
		bool		isMember(std::string nickname) const;

};

#endif

// MODES
// -i  :  bool
// -t  :  bool
// -k  :  string
// -o  :  vecteur de string (membres)
// -l  :  int (-1 if no limit)
