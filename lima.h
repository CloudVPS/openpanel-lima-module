#ifndef _lima_H
#define _lima_H 1
#include <grace/application.h>
#include <opencore/moduleapp.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class limamoduleApp : public moduleapp
{
public:
		 	 limamoduleApp (void) :
				moduleapp ("openpanel.module.lima")
			 {
			 }
			~limamoduleApp (void)
			 {
			 }

	int		 main (void);
	bool	 handleCreateList (void);
	bool	 handleDeleteList (void);
	
	bool	 rewritePostfixFiles (void);
	bool	 rewritePostfixFile (const string &baseName);
};

#endif
