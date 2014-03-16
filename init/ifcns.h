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
#ifndef _IFCNS_H_
#define _IFCNS_H_

/* File: autosel.cpp */
void pr1(char *s);


/* File: compile.cpp */
void get_descriptions(char *pth, char ***descrs, int *n, autosel_data **ad, int *nn);
int compile(char *infn, char *outfn, char *notes, int notelen, char *conf, unsigned *pnModemBaud);
int select_strings(char **strs, int count, int cur, int yt, int yb, int xl, int xr);


/* File: convert.cpp */
void c_setup();
void c_old_to_new();
bool c_IsUserListInOldFormat();
int c_check_old_struct();

/* File: init.cpp */

void load_modems();
int set_modem_info( char *mt, bool bPause );
char *mdm_name(int mdm_num);
void select_modem();
void edit_net(int nn);
void networks();
void calc_CRC(unsigned char b);
void tweak_dir(char *s);
void convcfg();
void printcfg();
int verify_dir(char *typeDir, char *dirName);

class InitSession;
class WInitApp;
InitSession* GetSession();
WInitApp* GetApplication();


/* File: init1.cpp */

void init();
void *malloca(unsigned long nbytes);
int check_comport(int pn);
void outcomch(char ch);
char peek1c();
char get1c();
int comhit();
void dump();
void set_baud(unsigned int rate);
void initportb(int port_num);
void closeport();
void dtr(int i);
void rts(int i);
void pausescr();
void backspace();
unsigned char inkey();
unsigned char upcase(unsigned char ch);
unsigned char getkey();
void input1( char *pszOutText, int nMaxLength, bool bAllowLowerCase );
void input( char *pszOutText, int nMaxLength );
int yn();
int ny();
char onek(char *s);
void OutputStringRaw(char *s);
void editline(char *s, int len, int status, int *returncode,char *ss);
int toggleitem(int value, char **strings, int num, int *returncode);
double timer();
long timer1();
int number_userrecs();
int open_qscn();
void close_qscn();
void read_qscn(unsigned int un, unsigned long *qscn, int stayopen);
void write_qscn(unsigned int un, unsigned long *qscn, int stayopen);
void wait1(long l);
int exist(char *s);
void save_status();
char *date();
char *times();
bool read_status();
int save_config();
void Puts( const char *pszText );
void nlx( int numLines = 1 );
void Printf( const char *pszFormat, ... );
void create_text(char *fn);
void cvtx(unsigned short sp, char *rc);
void convert_result_codes();
void init_files();
void convert_modem_info(char *fn);
void init_modem_info();
void new_init();
int verify_inst_dirs(configoverrec *co, int inst);
void read_user(unsigned int un, userrec *u);
void write_user(unsigned int un, userrec *u);
int GetNextSelectionPosition( int nMin, int nMax, int nCurrentPos, int nReturnCode );

/* File: init2.cpp */

char *ctim(double d);
void trimstr(char *s);
void trimstrpath(char *s);
void print_time(unsigned short t, char *s);
unsigned short get_time(char *s);
void sysinfo1();
void sysinfo2();
void setpaths();
void setupcom();
int xlate_from_char(char ch);
int read_subs();
void write_subs();
void del_net(int nn);
void insert_net(int nn);
void convert_to(int num_subs, int num_dirs);
void up_subs_dirs();
void edit_lang(int nn);
void up_langs();
void edit_editor(int n);
void extrn_editors();
char *prot_name(int pn);
void edit_prot(int n);
void extrn_prots();

/* File: init3.cpp */

void edit_arc(int nn);
void externals();
void up_str(char *s, int cursl, int an);
void pr_st(int cursl, int ln, int an);
void up_sl(int cursl);
void ed_slx(int *sln);
void sec_levs();
void list_autoval();
void edit_autoval(int n);
void autoval_levs();
void create_arcs();

// ** OSD CODE **

/* File: filesupp.cpp */
void WOSD_ChangeDirTo(const char *s);
void WOSD_GetDir(char *s, int be);


/* File: utility2.cpp */
void WWIV_Delay(unsigned long usec);


#endif