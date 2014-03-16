/**************************************************************************/
/*                                                                        */
/*                 WWIV Initialization Utility Version 5.0                */
/*             Copyright (C)1998-2014, WWIV Software Services             */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/*                                                                        */
/**************************************************************************/

#include "wwivinit.h"


void edit_arc(int nn)
{
	arcrec arc[MAX_ARCS];
	
	int i = nn;
	char szFileName[ MAX_PATH ];
	sprintf(szFileName,"%sARCHIVER.DAT",syscfg.datadir);
	int hFile=_open(szFileName, O_RDWR | O_BINARY);
	if (hFile<0) 
	{
		create_arcs();
		hFile=_open(szFileName, O_RDWR | O_BINARY);
	}
	
	_read(hFile, &arc, MAX_ARCS * sizeof(arcrec));
	
	// display arcs and edit menu, get choice
	
    bool done1 = false;
	do 
	{
		int cp = 4;
		done1 = false;
		app->localIO->LocalCls();
		textattr(11);
		Printf("                 Archiver Configuration\r\n\n");
		textattr(3);
		if (i == 0) 
		{
			Printf("Archiver #%d  ", i+1);
			textattr(14);
			Printf("(Default)\r\n\n");
		} 
		else
		{
			Printf("Archiver #%d           \n\n", i+1);
		}
		textattr(3);
		Printf("Archiver Name      : %s\n", arc[i].name);
		Printf("Archiver Extension : %s\n", arc[i].extension);
		Printf("List Archive       : %s\n", arc[i].arcl);
		Printf("Extract Archive    : %s\n", arc[i].arce);
		Printf("Add to Archive     : %s\n", arc[i].arca);
		Printf("Delete from Archive: %s\n", arc[i].arcd);
		Printf("Comment Archive    : %s\n", arc[i].arck);
		Printf("Test Archive       : %s\n", arc[i].arct);
		app->localIO->LocalGotoXY(0,13);
		Printf("                                                             \r\n");
		textattr(11);
		Printf("[ = Previous Archiver  ] = Next Archiver\r\n");
		textattr(3);
		Printf("                                                             \r\n");
		Printf("                                                             \r\n");
		textattr(14);
		Puts("<ENTER> to edit    <ESC> when done.");
		textattr(3);
		nlx();
		char ch=onek("\033[]\r");
		switch(ch) 
		{
		case '\r':
            {
			    app->localIO->LocalGotoXY(0,13);
			    Printf("                                                             \r\n");
			    Printf("%%1 %%2 ect. are parameters passed.  Minimum of two on Add and \r\n");
			    Printf("Extract command lines. For added security, a complete path to\r\n");
			    Printf("the archiver and extension should be used. I.E.:             \r\n");
			    Printf("c:\\bin\\arcs\\zip.exe -a %%1 %%2                              \r\n");
			    Printf("                                                             \r\n");
			    textattr(14);
			    Printf("<ESC> when done\r\n");
			    textattr(3);
                bool done = false;
			    do 
			    {
                    int i1 = 0;
				    app->localIO->LocalGotoXY(21, cp);
				    switch(cp) 
				    {
				    case 4:
					    editline(arc[i].name,31,ALL,&i1,"");
					    trimstr(arc[i].name);
					    break;
				    case 5:
					    editline(arc[i].extension,3,UPPER_ONLY,&i1,"");
					    trimstr(arc[i].extension);
					    break;
				    case 6:
					    editline(arc[i].arcl,49,ALL,&i1,"");
					    trimstr(arc[i].arcl);
					    break;
				    case 7:
					    editline(arc[i].arce,49,ALL,&i1,"");
					    trimstr(arc[i].arce);
					    break;
				    case 8:
					    editline(arc[i].arca,49,ALL,&i1,"");
					    trimstr(arc[i].arca);
					    break;
				    case 9:
					    editline(arc[i].arcd,49,ALL,&i1,"");
					    trimstr(arc[i].arcd);
					    break;
				    case 10:
					    editline(arc[i].arck,49,ALL,&i1,"");
					    trimstr(arc[i].arck);
					    break;
				    case 11:
					    editline(arc[i].arct,49,ALL,&i1,"");
					    trimstr(arc[i].arct);
					    break;
				    }
                    cp = GetNextSelectionPosition( 4, 11, cp, i1 );
                    if ( i1 == DONE )
                    {
                        done = true;
                    }
			    } while ( !done && !hangup );
            }
			break;
		case '\033':
            {
			    done1=true;
            }
			break;
		case ']':
			i++;
			if (i > MAX_ARCS-1)
			{
				i=0;
			}
			break;
		case '[':
			i--;
			if (i < 0)
			{
				i=MAX_ARCS-1;
			}
			break;
		}
  } while ( !done1 && !hangup );
  
  // copy first four new fomat archivers to oldarcsrec 
  
  for (int j = 0; j < 4; j++) 
  {
	  strncpy(syscfg.arcs[j].extension,arc[j].extension, 4);
	  strncpy(syscfg.arcs[j].arca     ,arc[j].arca     , 32);
	  strncpy(syscfg.arcs[j].arce     ,arc[j].arce     , 32);
	  strncpy(syscfg.arcs[j].arcl     ,arc[j].arcl     , 32);
  }
  
  // seek to beginning of file, write arcrecs, close file 
  
  _lseek(hFile, 0L, SEEK_SET);
  _write(hFile, (void *)arc, MAX_ARCS * sizeof(arcrec));
  _close(hFile);
  
}


void up_str(char *s, int cursl, int an)
{
	strcpy(s, (syscfg.sl[cursl].ability & an) ? "Y" : "N");
}

void pr_st(int cursl, int ln, int an)
{
	char s[81];
	
	up_str(s,cursl,an);
	app->localIO->LocalGotoXY(19,ln);
	Puts(s);
}


void up_sl(int cursl)
{
	slrec ss = syscfg.sl[cursl];
	app->localIO->LocalGotoXY(19,0);
	Printf("%-3u",cursl);
	app->localIO->LocalGotoXY(19,1);
	Printf("%-5u",ss.time_per_day);
	app->localIO->LocalGotoXY(19,2);
	Printf("%-5u",ss.time_per_logon);
	app->localIO->LocalGotoXY(19,3);
	Printf("%-5u",ss.messages_read);
	app->localIO->LocalGotoXY(19,4);
	Printf("%-3u",ss.emails);
	app->localIO->LocalGotoXY(19,5);
	Printf("%-3u",ss.posts);
	pr_st(cursl,6,ability_post_anony);
	pr_st(cursl,7,ability_email_anony);
	pr_st(cursl,8,ability_read_post_anony);
	pr_st(cursl,9,ability_read_email_anony);
	pr_st(cursl,10,ability_limited_cosysop);
	pr_st(cursl,11,ability_cosysop);
}



void ed_slx(int *sln)
{
	int i;
	char s[81];
	
	int cursl = *sln;
	up_sl(cursl);
	bool done = false;
	int cp=0;
	do 
	{
        int i1 = 0;
		app->localIO->LocalGotoXY(19,cp);
		switch(cp) 
		{
		case 0:
			_ultoa(cursl,s,10);
			editline(s,3,NUM_ONLY,&i1,"");
			i=atoi(s);
			while (i<0)
			{
				i+=255;
			}
			while (i>255)
			{
				i-=255;
			}
			if (i!=cursl) 
			{
				cursl=i;
				up_sl(cursl);
			}
			break;
		case 1:
			_ultoa(syscfg.sl[cursl].time_per_day,s,10);
			editline(s,5,NUM_ONLY,&i1,"");
			i=atoi(s);
			syscfg.sl[cursl].time_per_day=i;
			Printf("%-5u",i);
			break;
		case 2:
			_ultoa(syscfg.sl[cursl].time_per_logon,s,10);
			editline(s,5,NUM_ONLY,&i1,"");
			i=atoi(s);
			syscfg.sl[cursl].time_per_logon=i;
			Printf("%-5u",i);
			break;
		case 3:
			_ultoa(syscfg.sl[cursl].messages_read,s,10);
			editline(s,5,NUM_ONLY,&i1,"");
			i=atoi(s);
			syscfg.sl[cursl].messages_read=i;
			Printf("%-5u",i);
			break;
		case 4:
			_ultoa(syscfg.sl[cursl].emails,s,10);
			editline(s,3,NUM_ONLY,&i1,"");
			i=atoi(s);
			syscfg.sl[cursl].emails=i;
			Printf("%-3u",i);
			break;
		case 5:
			_ultoa(syscfg.sl[cursl].posts,s,10);
			editline(s,3,NUM_ONLY,&i1,"");
			i=atoi(s);
			syscfg.sl[cursl].posts=i;
			Printf("%-3u",i);
			break;
		case 6:
			i=ability_post_anony;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		case 7:
			i=ability_email_anony;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		case 8:
			i=ability_read_post_anony;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		case 9:
			i=ability_read_email_anony;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		case 10:
			i=ability_limited_cosysop;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		case 11:
			i=ability_cosysop;
			up_str(s,cursl,i);
			editline(s,1,UPPER_ONLY,&i1,"");
			if (s[0]=='Y')
			{
				syscfg.sl[cursl].ability |= i;
			}
			else
			{
				syscfg.sl[cursl].ability &= (~i);
			}
			pr_st(cursl,cp,i);
			break;
		}
        cp = GetNextSelectionPosition( 0, 11, cp, i1 );
        if ( i1 == DONE )
        {
            done = true;
        }
	} while ( !done && !hangup );
	*sln=cursl;
}


void sec_levs()
{
	app->localIO->LocalCls();
	textattr(3);
	Printf("Security level   : \r\n");
	Printf("Time per day     : \r\n");
	Printf("Time per logon   : \r\n");
	Printf("Messages read    : \r\n");
	Printf("Emails per day   : \r\n");
	Printf("Posts per day    : \r\n");
	Printf("Post anony       : \r\n");
	Printf("Email anony      : \r\n");
	Printf("Read anony posts : \r\n");
	Printf("Read anony email : \r\n");
	Printf("Limited co-sysop : \r\n");
	Printf("Co-sysop         : \r\n");
	int cursl = 10;
	up_sl(cursl);
	bool done = false;
	app->localIO->LocalGotoXY(0,12);
	textattr(14);
	Puts("\r\n<ESC> to exit\r\n");
	textattr(11);
	Printf("[ = down one SL    ] = up one SL\r\n");
	Printf("{ = down 10 SL     } = up 10 SL\r\n");
	Printf("<C/R> = edit SL data\r\n");
	textattr(3);
	do 
	{
		app->localIO->LocalGotoXY(0,18);
		Puts("Command: ");
		char ch = onek("\033[]{}\r");
		switch(ch) 
		{
		case '\r':
			app->localIO->LocalGotoXY(0,12);
			textattr(14);
			Puts("\r\n<ESC> to exit\r\n");
			textattr(3);
			Printf("                                \r\n");
			Printf("                               \r\n");
			Printf("                    \r\n");
			Puts("\r\n          \r\n");
			ed_slx(&cursl);
			app->localIO->LocalGotoXY(0,12);
			textattr(14);
			Puts("\r\n<ESC> to exit\r\n");
			textattr(11);
			Printf("[ = down one SL    ] = up one SL\r\n");
			Printf("{ = down 10 SL     } = up 10 SL\r\n");
			Printf("<C/R> = edit SL data\r\n");
			textattr(3);
			break;
		case '\033':
			done = true;
			break;
		case ']':
			cursl+=1;
			if (cursl>255)
			{
				cursl-=256;
			}
			if (cursl<0)
			{
				cursl+=256;
			}
			up_sl(cursl);
			break;
		case '[':
			cursl-=1;
			if (cursl>255)
			{
				cursl-=256;
			}
			if (cursl<0)
			{
				cursl+=256;
			}
			up_sl(cursl);
			break;
		case '}':
			cursl+=10;
			if (cursl>255)
			{
				cursl-=256;
			}
			if (cursl<0)
			{
				cursl+=256;
			}
			up_sl(cursl);
			break;
		case '{':
			cursl-=10;
			if (cursl>255)
			{
				cursl-=256;
			}
			if (cursl<0)
			{
				cursl+=256;
			}
			up_sl(cursl);
			break;
		}
	} while ( !hangup && !done );
	save_config();
}


void list_autoval()
{
	char s3[81],ar[20],dar[20],r[20];
	int i;
	valrec v;
	
	app->localIO->LocalCls();
	Printf("NUM  SL   DSL  AR                DAR               RESTRICTIONS\r\n");
	Printf("---  ---  ---  ----------------  ----------------  ----------------\r\n");
	strcpy(s3,restrict_string);
	for ( int i1 = 0; i1 < 10; i1++ ) 
	{
		v=syscfg.autoval[i1];
		for (i=0; i<=15; i++) 
		{
			if (v.ar & (1 << i))
			{
				ar[i]='A'+i;
			}
			else
			{
				ar[i]=32;
			}
			if (v.dar & (1 << i))
			{
				dar[i]='A'+i;
			}
			else
			{
				dar[i]=32;
			}
			if (v.restrict & (1 << i))
			{
				r[i]=s3[i];
			}
			else
			{
				r[i]=32;
			}
		}
		r[16]=0;
		ar[16]=0;
		dar[16]=0;
		
		Printf( "%3d  %3d  %3d  %16s  %16s  %16s\r\n",i1+1,v.sl,v.dsl,ar,dar,r );
	}
	nlx(2);
}


void edit_autoval(int n)
{
	char s[81],ar[20],dar[20],r[20];
	int i,cp;
	valrec v;
	
	v=syscfg.autoval[n];
	strcpy(s,restrict_string);
	for (i=0; i<=15; i++) 
	{
		if (v.ar & (1 << i))
		{
			ar[i]='A'+i;
		}
		else
		{
			ar[i]=32;
		}
		if (v.dar & (1 << i))
		{
			dar[i]='A'+i;
		}
		else
		{
			dar[i]=32;
		}
		if (v.restrict & (1 << i))
		{
			r[i]=s[i];
		}
		else
		{
			r[i]=32;
		}
	}
	r[16]=0;
	ar[16]=0;
	dar[16]=0;
	app->localIO->LocalCls();
	Printf("Auto-validation data for: Alt-F%d\n\n",n+1);
	Printf("SL           : %d\n",v.sl);
	Printf("DSL          : %d\n",v.dsl);
	Printf("AR           : %s\n",ar);
	Printf("DAR          : %s\n",dar);
	Printf("Restrictions : %s\n",r);
	textattr(14);
	Puts("\r\n\r\n<ESC> to exit\r\n");
	textattr(3);
	bool done = false;
	cp=0;
	do 
	{
        int i1 = 0;
		app->localIO->LocalGotoXY(15,cp+2);
		switch(cp) 
		{
		case 0:
			_ultoa(v.sl,s,10);
			editline(s,3,NUM_ONLY,&i1,"");
			i=atoi(s);
			if ((i<0) || (i>254))
			{
				i=10;
			}
			v.sl=i;
			Printf("%-3d",i);
			break;
		case 1:
			_ultoa(v.dsl,s,10);
			editline(s,3,NUM_ONLY,&i1,"");
			i=atoi(s);
			if ((i<0) || (i>254))
			{
				i=0;
			}
			v.dsl=i;
			Printf("%-3d",i);
			break;
		case 2:
			editline(ar,16,SET,&i1,"ABCDEFGHIJKLMNOP");
			v.ar=0;
			for (i=0; i<16; i++)
			{
				if (ar[i]!=32)
				{
					v.ar |= (1 << i);
				}
			}
			break;
		case 3:
			editline(dar,16,SET,&i1,"ABCDEFGHIJKLMNOP");
			v.dar=0;
			for (i=0; i<16; i++)
			{
				if (dar[i]!=32)
				{
					v.dar |= (1 << i);
				}
			}
			break;
		case 4:
			editline(r,16,SET,&i1,restrict_string);
			v.restrict=0;
			for (i=0; i<16; i++)
			{
				if (r[i]!=32)
				{
					v.restrict |= (1 << i);
				}
			}
			break;
		}
        cp = GetNextSelectionPosition( 0, 4, cp, i1 );
        if ( i1 == DONE )
        {
            done = true;
        }
	} while ( !done && !hangup );
	syscfg.autoval[n]=v;
}


void autoval_levs()
{
	bool done = false;
	do 
	{
		list_autoval();
		textattr(14);
		Puts("Which (0-9, Q=Quit) ? ");
		textattr(3);
		char ch = onek("Q0123456789\033");
		if ( ch == 'Q' || ch == '\033' )
		{
			done = true;
		}
		else 
		{
			if (ch=='0')
			{
				edit_autoval(9);
			}
			else
			{
				edit_autoval(ch-'1');
			}
		}
	} while ( !done && !hangup );
	save_config();
}


void create_arcs()
{
	arcrec arc[MAX_ARCS];
	
	strncpy(arc[0].name,"Zip",32);
	strncpy(arc[0].extension,"ZIP",4);
	strncpy(arc[0].arca,"zip.exe %1 %2",50);
	strncpy(arc[0].arce,"unzip.exe -o %1 %2",50);
	strncpy(arc[0].arcl,"unzip.exe -l %1",50);
	strncpy(arc[0].arcd,"zip.exe -d %1 -@ < BBSADS.TXT",50);
	strncpy(arc[0].arck,"zip.exe -z %1 < COMMENT.TXT",50);
	strncpy(arc[0].arct,"unzip.exe -t %1",50);
	strncpy(arc[1].name,"ARJ",32);
	strncpy(arc[1].extension,"ARJ",4);
	strncpy(arc[1].arca,"arj.exe a %1 %2",50);
	strncpy(arc[1].arce,"arj.exe e %1 %2",50);
	strncpy(arc[1].arcl,"arj.exe i %1",50);
	strncpy(arc[1].arcd,"arj.exe d %1 !BBSADS.TXT",50);
	strncpy(arc[1].arck,"arj.exe c %1 -zCOMMENT.TXT",50);
	strncpy(arc[1].arct,"arj.exe t %1",50);
	strncpy(arc[2].name,"RAR",32);
	strncpy(arc[2].extension,"RAR",4);
	strncpy(arc[2].arca,"rar.exe a -std -s- %1 %2",50);
	strncpy(arc[2].arce,"rar.exe e -std -o+ %1 %2",50);
	strncpy(arc[2].arcl,"rar.exe l -std %1",50);
	strncpy(arc[2].arcd,"rar.exe d -std %1 < BBSADS.TXT ",50);
	strncpy(arc[2].arck,"rar.exe zCOMMENT.TXT -std %1",50);
	strncpy(arc[2].arct,"rar.exe t -std %1",50);
	strncpy(arc[3].name,"LZH",32);
	strncpy(arc[3].extension,"LZH",4);
	strncpy(arc[3].arca,"lha.exe a %1 %2",50);
	strncpy(arc[3].arce,"lha.exe eo  %1 %2",50);
	strncpy(arc[3].arcl,"lha.exe l %1",50);
	strncpy(arc[3].arcd,"lha.exe d %1  < BBSADS.TXT",50);
	strncpy(arc[3].arck,"",50);
	strncpy(arc[3].arct,"lha.exe t %1",50);
	strncpy(arc[4].name,"PAK",32);
	strncpy(arc[4].extension,"PAK",4);
	strncpy(arc[4].arca,"pkpak.exe -a %1 %2",50);
	strncpy(arc[4].arce,"pkunpak.exe -e  %1 %2",50);
	strncpy(arc[4].arcl,"pkunpak.exe -p %1",50);
	strncpy(arc[4].arcd,"pkpak.exe -d %1  @BBSADS.TXT",50);
	strncpy(arc[4].arck,"pkpak.exe -c %1 < COMMENT.TXT ",50);
	strncpy(arc[4].arct,"pkunpak.exe -t %1",50);
	
	
	for(int i = 5; i < MAX_ARCS; i++ )
	{
		strncpy(arc[i].name,"New Archiver Name",32);
		strncpy(arc[i].extension,"EXT",4);
		strncpy(arc[i].arca,"archive add command",50);
		strncpy(arc[i].arce,"archive extract command",50);
		strncpy(arc[i].arcl,"archive list command",50);
		strncpy(arc[i].arcd,"archive delete command",50);
		strncpy(arc[i].arck,"archive comment command",50);
		strncpy(arc[i].arct,"archive test command",50);
	}
	
	char szFileName[MAX_PATH];
	sprintf(szFileName,"%sARCHIVER.DAT",syscfg.datadir);
	int hFile = _open(szFileName,O_WRONLY | O_BINARY | O_CREAT | O_EXCL, S_IWRITE);
	if (hFile<0) 
	{
		Printf("Couldn't open '%s' for writing.\n",szFileName);
		textattr(7);
		exit( 1 );
	}
	_write(hFile, (void *)arc, MAX_ARCS * sizeof(arcrec));
	_close(hFile);
}
