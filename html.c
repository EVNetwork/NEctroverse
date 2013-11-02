#ifndef GLOBALINCLUDED
#include "global.h"
#endif


#include "cpuinfo.c"

#include "html_gameplay.c"

#include "html_main.c"

#include "html_status.c"

#include "html_forum.c"

#include "html_user.c"

#include "html_admin.c"

#include "html_ajax.c"


/*This function will be use to approve file uploaded by player to our image server*/
void iohttpFunc_Approve (svConnectionPtr cnt)
{
	int nAllow = 0; // == 1 if this user have the right of allowing image
	int id, nAction;  //Action == 1 approve 2 == Disapprove (file deleted, msg send to id)
	int a;						//Because Mal always use a and not i
	char *szAction;
	char *szID;
	char szName[50];
	char szPass[50];
	char szFileName[50];
	char szTemp[50];
	FILE *fDesc;
	
	if((( id = iohttpIdentify( cnt, 0 ) ) >= 0 ) && (cnt != NULL))
 {
 	if( (cnt->dbuser)->level >= LEVEL_MODERATOR )
 		nAllow ++;
 }
 
 iohttpVarsInit( cnt );
 //Look for variable here
 
 szAction = iohttpVarsFind( "action" );
 szID = iohttpVarsFind( "id" );
 
 iohttpVarsCut();
 iohttpBase( cnt, 1 ); //Init of the html code for the player
 iohttpBodyInit( cnt, "Approving" );
 
 if(nAllow)
 {
 	//Read all the input given by Post then show link to the file to download it before approving it.
 	if((szAction)&&(szID))
 	{
 		nAction = atoi(szAction);
 		//Open the desc file of this file ID
 		sprintf(szFileName, "pending/%s.txt", szID);
 	 fDesc = fopen(szFileName, "r");
 		if(fDesc)
 		{
 			if( ( fscanf(fDesc, "%s", szName) || fscanf(fDesc, "%s", szPass) || fscanf(fDesc, "%s", szFileName) ) != 1 ) {
				fclose(fDesc);
				return;
			}
			
 			//In both case we must check if the user exist and what is his id
	 		nAllow = -1;
	 		//this if is for formating the user and pass put ' ' instead of +
	 		if( strlen( szName ) && strlen( szPass ) )
			 {
			  for( a = 0 ; szName[a] ; a++ )
			  {
			   if( szName[a] == '+' )
			    szName[a] = ' ';
			   else if( ( szName[a] == 10 ) || ( szName[a] == 13 ) )
			    szName[a] = 0;
			  }
			  for( a = 0 ; szPass[a] ; a++ )
			  {
			   if( szPass[a] == '+' )
			    szPass[a] = ' ';
			   else if( ( szPass[a] == 10 ) || ( szPass[a] == 13 ) )
			    szPass[a] = 0;
			  }
			  
			  if( ( id = dbUserSearch( szName ) ) >= 0 )
			  {
				  if( dbUserRetrievePassword( id, szTemp ) >= 0 )
				  {
				  	if( ioCompareExact( szPass, szTemp) ) 
				   	nAllow = id;
				   }
				 }
				 //nAllow will be equal to the id of the user if the good password was supplied
				 
	 		}
	 		
	 		if(nAction == 1)			//Approve
	 		{
	 			svSendPrintf(cnt, "The file %s is approved<br>", szID);
	 		}
	 		else if(nAction == 2)	//Disapprove
	 		{
	 			svSendPrintf(cnt, "This file %s is deleted and a message was sent to the user<br>", szID);
	 		}
				fclose(fDesc);
			}
 	}
 	
 	//List all the file "pending" here
 	
 }
 else
 {
 	svSendString(cnt, "Sorry you don't have the necessary right to approve file upload by all the players!");
 }
 iohttpBodyEnd( cnt );
 return;
}

