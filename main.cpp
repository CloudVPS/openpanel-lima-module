// --------------------------------------------------------------------------
// OpenPanel - The Open Source Control Panel
// Copyright (c) 2006-2007 PanelSix
//
// This software and its source code are subject to version 2 of the
// GNU General Public License. Please be aware that use of the OpenPanel
// and PanelSix trademarks and the IconBase.com iconset may be subject
// to additional restrictions. For more information on these restrictions
// and for a copy of version 2 of the GNU General Public License, please
// visit the Legal and Privacy Information section of the OpenPanel
// website on http://www.openpanel.com/
// --------------------------------------------------------------------------

#include <moduleapp.h>
#include "lima.h"

#include <grace/file.h>
#include <grace/filesystem.h>
#include <dbfile/db4file.h>

APPOBJECT(limamoduleApp);

#define PATH_STAGING "/var/opencore/conf/staging/LIMA"

//  =========================================================================
/// Main method.
//  =========================================================================
int limamoduleApp::main (void)
{
	string conferr;
	
	chdir (PATH_STAGING);
	fs.cd (PATH_STAGING);
	
	if (data["OpenCORE:Command"] == "create")
	{
		// We accept one type only at this point.
		if (! data.exists ("Mail:List"))
		{
			sendresult (moderr::err_command, "Unsupported create");
			authd.quit ();
			return 0;
		}
		
		if (! handleCreateList ()) return 0;
		
	}
	else if (data["OpenCORE:Command"] == "delete")
	{
		if (! data.exists ("Mail:List"))
		{
			sendresult (moderr::err_command, "Unsupported delete");
			authd.rollback ();
			authd.quit ();
			return 0;
		}
		
		if (! handleDeleteList ()) return 0;
	}
	else
	{
		sendresult (moderr::err_command, "Unknown command");
		authd.quit ();
		return 0;
	}
	
	authd.quit ();
	sendresult (moderr::ok, "");
	return 0;
}

// ==========================================================================
// METHOD ::handleCreateList
// ==========================================================================
bool limamoduleApp::handleCreateList (void)
{
	// Get arguments, build runcommand arg.
	statstring listname = data["Mail:List"]["id"].sval();
	string listowner = data["Mail:List"]["owner"];
	value args;
	args.newval() = listname;
	args.newval() = listowner;
	
	// Set up dbfile objects to use for transports/virtuals.
	db4file DBTransports;
	db4file DBVirtuals;
	DBTransports.setencoding (dbfile::flat);
	DBVirtuals.setencoding (dbfile::flat);
	
	if (! DBTransports.open ("list_transports.db"))
	{
		sendresult (moderr::err_command, "Failed to open list_transports");
		authd.quit ();
		return false;
	}
	
	if (! DBVirtuals.open ("list_virtuals.db"))
	{
		sendresult (moderr::err_command, "Failed to open list_virtuals.db");
		authd.quit ();
		DBTransports.close ();
		return false;
	}

	// We have a firm grip on the db files, time to run the actual
	// mklist tool.
	if (authd.runuserscript ("mklist", args, "lima"))
	{
		sendresult (moderr::err_authdaemon, "List creation failed");
		authd.rollback ();
		authd.quit ();
		return false;
	}
	
	// Fill in the database records.
	DBTransports.db[listname] = (string) "lima:";
	DBVirtuals.db[listname] = (string) "VIRTUAL";
	
	// Commit them.
	DBTransports.commit ();
	DBVirtuals.commit ();
	
	// Close the database files.
	DBTransports.close ();
	DBVirtuals.close ();
	
	if (! rewritePostfixFiles ())
	{
		sendresult (moderr::err_authdaemon, "Error writing/installing files");
		authd.rollback ();
		authd.quit ();
		return false;
	}
	
	return true;
}

// ==========================================================================
// METHOD ::handleDeleteList
// ==========================================================================
bool limamoduleApp::handleDeleteList (void)
{
	string listname = data["Mail:List"]["id"];
	
	db4file DBTransports;
	db4file DBVirtuals;
	DBTransports.setencoding (dbfile::flat);
	DBVirtuals.setencoding (dbfile::flat);
	
	if (! DBTransports.open ("list_transports.db"))
	{
		sendresult (moderr::err_command, "Failed to open list_transports");
		authd.quit ();
		return false;
	}
	
	if (! DBVirtuals.open ("list_virtuals.db"))
	{
		sendresult (moderr::err_command, "Failed to open list_virtuals.db");
		authd.quit ();
		DBTransports.close ();
		return false;
	}
	
	value args;
	args.newval() = listname;

	if (authd.runuserscript ("rmlist", args, "lima"))
	{
		return true;
	}
	
	DBTransports.rmval (listname);
	DBVirtuals.rmval (listname);
	DBTransports.commit ();
	DBVirtuals.commit ();
	
	DBTransports.close ();
	DBVirtuals.close ();
	
	if (! rewritePostfixFiles ())
	{
		sendresult (moderr::err_authdaemon, "Error writing/installing files");
		authd.rollback ();
		authd.quit ();
		return false;
	}
	
	return true;
}

// ==========================================================================
// METHOD ::rewritePostfixFiles
// ==========================================================================
bool limamoduleApp::rewritePostfixFiles (void)
{
	if (! rewritePostfixFile ("list_virtuals")) return false;
	return rewritePostfixFile ("list_transports");
}

// ==========================================================================
// METHOD ::rewritePostfixFile
// ==========================================================================
bool limamoduleApp::rewritePostfixFile (const string &baseName)
{
	db4file DB;
	string dbName = baseName;
	dbName += ".db";
	
	file f;
	if (! f.openwrite (baseName, 0600)) return false;
	if (! DB.open (dbName))
	{
		f.close ();
		return false;
	}
	
	DB.setencoding (dbfile::flat);
	
	foreach (dumprow, DB.db)
	{
		f.printf ("%s\t%s\n", dumprow.id().str(), dumprow.cval());
	}
	
	f.close ();
	DB.close ();
	
	if (authd.installfile (baseName, "/etc/postfix/openpanel")) return false;
	if (authd.installfile (dbName, "/etc/postfix/openpanel")) return false;
	
	return true;
}

