// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/


#ifndef _lima_H
#define _lima_H 1
#include <grace/application.h>
#include <openpanel-core/moduleapp.h>

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
