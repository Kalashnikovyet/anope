/* NickServ core functions
 *
 * (C) 2003-2009 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 *
 * $Id$
 *
 */
/*************************************************************************/

#include "module.h"

void myNickServHelp(User *u);

class CommandNSUpdate : public Command
{
 public:
	CommandNSUpdate() : Command("UPDATE", 0, 0)
	{
	}

	CommandReturn Process(User *u, std::vector<std::string> &params)
	{
		NickAlias *na = findnick(u->nick);

		if (!na)
		{
			return MOD_CONT;
		}

		if (NSModeOnID)
			do_setmodes(u);
		check_memos(u);

		if (na->last_realname)
			delete [] na->last_realname;
		na->last_realname = sstrdup(u->realname);
		na->status |= NS_IDENTIFIED;
		na->last_seen = time(NULL);
		if (ircd->vhost)
			do_on_id(u);
		notice_lang(s_NickServ, u, NICK_UPDATE_SUCCESS, s_NickServ);
		return MOD_CONT;
	}

	bool OnHelp(User *u, const std::string &)
	{
		notice_help(s_NickServ, u, NICK_HELP_UPDATE);
		return true;
	}
};

class NSUpdate : public Module
{
 public:
	NSUpdate(const std::string &modname, const std::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetVersion("$Id$");
		this->SetType(CORE);
		this->AddCommand(NICKSERV, new CommandNSUpdate(), MOD_UNIQUE);

		this->SetNickHelp(myNickServHelp);
	}
};

/**
 * Add the help response to anopes /ns help output.
 * @param u The user who is requesting help
 **/
void myNickServHelp(User *u)
{
	notice_lang(s_NickServ, u, NICK_HELP_CMD_UPDATE);
}

MODULE_INIT("ns_update", NSUpdate)
