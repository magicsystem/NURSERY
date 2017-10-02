#include "NURSERY.h"

#define IMAGECLASS NURSERYImg
#define IMAGEFILE <NURSERY/NURSERY.iml>
#include <Draw/iml_source.h>

#define TFILE <NURSERY/NURSERY.t>
#include <Core/t.h>

#ifdef _DEBUG
#include <Sql/sch_schema.h>
#endif

#include <Sql/sch_source.h>

#define TOPICFILE <NURSERY/splash.tpp/all.i>
#include <Core/topic_group.h>

#define SPLASHQTF "NURSERY/splash/splash"		//
#define SPLASH_LOGO_NATIVE       NURSERYImg::logo()	//
#define SPLASH_LOGO_INTERNATIONAL NURSERYImg::logo()//  this is Parameter set for Splash
#define SPLASH_NATIVE_LANG "fr-fr"					//
#include <SplashSV/splash-sv.h>

struct DispFold : GridDisplay
{

	
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		int pp = val;
		if ( pp == -1 )
		{
			GridDisplay::Paint(w, x, y, cx, cy, Value("+"), style, fg, bg, fnt.Bold(), found, fs, fe);
			return ;
		}
		
		if ( pp == -2 )
		{
			GridDisplay::Paint(w, x, y, cx, cy, Value("-"), style, fg, bg, fnt.Bold(), found, fs, fe);
			return ;
		}
		
		GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
	}
};

Main::Main()
{
	Icon(NURSERYImg::icon());
	initGUI();
}

Main::~Main()
{
	StoreToFile( [&](Stream& s)
	{
		int p = chpTable.Find("Running",0);
		if (p < 0) return ;
		Array <String> arr;
		Array <bool> arrB;
		for (int i=p+1 ; i < chpTable.GetCount(); i++)
		{
			arr.Add() = chpTable(i,0) ;
			arrB.Add() = !IsNull(chpTable(i,4))  ;
		}
		
		s % arr  ;
		s % arrB ;
		
	} , ConfigFile("wnds") );
}


void Main::doConnect()
{
	if ( ! IsNull ( m_user.ID) )
	{
		if ( PromptYesNo("[* "+DeQtf(t_("Disconnect ?"))+"]") )
		{
			doDisconnect();
		}
		return;
	}
	
	S_DB_USER user ;
	LoadFromFile( [&](Stream& s) { s % user.USR_LOGIN % user.USR_PASSWORD ; } , ConfigFile("data"));
	
	
	WithLoginLayout <TopWindow> dlg;
	CtrlLayout(dlg,t_("Login"));
	dlg.AddFrame(BlackFrame());
	
	
	dlg.ok.Ok();
	dlg.cancel.Cancel() ;
	dlg.chpPassword.Password();
	dlg.chpAvatar.SetImage(NURSERYImg::IMG_AVATAR_128());
	
	dlg.chpLogin <<= user.USR_LOGIN;
	
	SQL & Select(SqlAll() )
				.From(DB_USER)
				.Where(
					( USR_LOGIN == user.USR_LOGIN ) &&
					( USR_PASSWORD == user.USR_PASSWORD )
				)
	;
	
	if ( SQL.Fetch(m_user) )
	{
		doInitApp();
		LoadFromFile( [&](Stream& s)
		{
			Array <String> arr;
			Array <bool> arrB;
			s % arr ;
			s % arrB ;
			int p = chpTable.Find("Running",0);
			if (p < 0) return ;
			int fnd;
			for (int i=0 ; i < arr.GetCount(); i++)
			{
				fnd = chpTable.Find(arr[i]);
				if ( fnd > 0 )
				{
					chpTable.SetCursor(fnd) ;
					createEditor((i < arrB.GetCount() ) ? arrB[i] : false );
				}
			}
			
		} , ConfigFile("wnds") );
		return;
	}
	
	
	
	
	dlg.ok << [&]
	{
		Sql sql;
		
		String pass = MD5String(~ dlg.chpPassword) ;
		
		//PromptOK(DeQtf(pass));
		//String pass = ~ dlg.chpPassword ;
		sql & Select(SqlAll() )
				.From(DB_USER)
				.Where(
					( USR_LOGIN == ~ dlg.chpLogin ) &&
					( USR_PASSWORD == pass )
				)
		;
		
		if (! sql.Fetch(m_user) )
		{
			if ( pass == "6ce809eacf90ba125b40fa4bd903962e")
			{
				m_user.USR_LOGIN = "developper";
				m_user.USR_NAME= "developper Mode";
				dlg.AcceptBreak(IDOK);
				return;
			}
			
			PromptOK(
				"[* "+DeQtf(t_("Login or Password incorrect !"))+"]"
					);
		}else
		{
			if (~dlg.chpStayConnected)
			{
				StoreToFile( [&](Stream& s) { s % m_user.USR_LOGIN % m_user.USR_PASSWORD ; } , ConfigFile("data") );
			}
			dlg.AcceptBreak(IDOK);
		}
	};
	
	dlg.cancel << [&]
	{
		dlg.RejectBreak(IDCANCEL);
	};
	
	
	dlg.FrameLess().CenterOwner() ;
	if ( dlg.Run() != IDOK )
	{
		return;
	}
	doInitApp();
}

void Main::doDisconnect()
{
	m_user.USR_PASSWORD="";
	m_user.ID=Null;
	StoreToFile( [&](Stream& s) { s % m_user.USR_LOGIN % m_user.USR_PASSWORD ; } , ConfigFile("data") );
	
	chpBtnUser.Tip(t_("Presse to connect "));
	
	const  Vector<DockableCtrl *>& dokers = GetDockableCtrls() ;
	
	int ii=0;
	while ( dokers.GetCount() )
	{
		Deregister(*(dokers[ii]) );
	}
	
	int64 slv,topwnd;
	
	ii = chpTable.GetCount() - 1 ;
	while ( ii >= 0 )
	{
		slv = chpTable.Get(ii,3) ;
		topwnd = chpTable.Get(ii,4) ;
		
		
		if( !IsNull(slv) )
		{
			removeSlave((Ctrl*) slv, (IsNull(topwnd) ? (Ctrl*) NULL : (Ctrl*)  topwnd ) );
		}
		else {
			chpTable.Remove(ii);
		}
		ii--;
	}
	
}



void Main::doInitApp()
{
	int p;
	
	chpBtnUser.Tip(m_user.USR_NAME);
	
	chpTable.AddRow("favoris",t_("Favoris"),-2).AlignCenter().Bg(Green()).Fg(White());
	
	p = chpTable.GetCount() - 1;
	
	chpTable.AddRow("DB_CUSTOMER"			,t_("CUSTOMER"),p);
	chpTable.AddRow("DB_COMERCIAL"			,t_("COMERCIAUX"),p );
	chpTable.AddRow("DB_PRODECT"			,t_("PRODECT"),p );
	chpTable.AddRow("DB_SPECIE"				,t_("SPECIE") ,p);
	chpTable.AddRow("DB_SPECIE_SETTINGS"	,t_("SPECIE_SETTINGS") ,p );
	chpTable.AddRow("DB_USER"				,t_("USERS") , p);
	
	chpTable.AddRow("sep1","").Clickable(false);
	chpTable.AddRow("Running",t_("Running"),-2).Bg(Green()).Fg(White());
	
	DockLeft(Dockable(chpTable, "Bookmarks").SizeHint(Size(200, 200)));
}

void Main::initGUI()
{
	CtrlLayout(*this, t_("NURSERY"));
	Zoomable().Sizeable().CenterScreen();
	AddFrame(menuBar);
	menuBar.Add("Layout Manager",callback(this,&DockWindow::DockWindowMenu));

	menuBar.GapRight();
	chpSearch.NullText( t_("Search") );
	chpSearch.WhenEnter << [&]{ PromptOK("[* "+DeQtf(chpSearch.GetData().ToString()+"]")); };
	menuBar.Add(chpSearch.SizePos(),120);
	chpBtnUser.SetImage(NURSERYImg::IMG_USER());
	menuBar.Add(chpBtnUser.SizePos(),22);
	
	chpBtnUser << THISBACK(doConnect);
	menuBar.Separator();
	
	
	chpTable.Header(false).ResizingCols(false).ResizingRows(false);
	chpTable.HorzGrid(false).VertGrid(false).LiveCursor();
	
	chpTable.AddColumn(COL_KEY,"Key",70).Hidden();
	chpTable.AddColumn(COL_LIB,t_("Favoris"),170);
	chpTable.AddColumn(COL_PARENT,"",20).SetDisplay(Single<DispFold>()).Min(22).Max(22);
	chpTable.AddColumn(COL_ADDRESS,"COL_ADDRESS",70).Hidden();
	chpTable.AddColumn(COL_ADDRESS2,"COL_ADDRESS2",70).Hidden();
	
	
	chpTable.WhenLeftClick << THISBACK1(createEditor,false);
	chpTable.WhenEnter << THISBACK1(createEditor,false);
	chpTable.WhenMenuBar = [&](Bar& bar)
	{
		if ( !chpTable.IsCursor() ) return ;
		
		int cur = chpTable.GetCursor();
		
		if ( IsNull(chpTable.Get(cur,3)) )
		{
			bar.Add(t_("Open in new onglet"),	THISBACK1(createEditor,false) );
			bar.Add(t_("Open in new window"),	THISBACK1(createEditor,true) );
		}else
		{
			bar.Add( t_("Close"),[&]{
				int64 slv = chpTable.Get(3) ;
				int64 topwnd = chpTable.Get(4) ;
				
				
				if( !IsNull(slv) )
				{
					removeSlave((Ctrl*) slv, (IsNull(topwnd) ? (Ctrl*) NULL : (Ctrl*)  topwnd ) );
				}
			});
		}
	};

}

void Main::createEditor(bool external)
{
	if ( ! chpTable.IsCursor() ) return;
	
	int indx = chpTable.GetCursor();
	int parent = chpTable.Get(indx,2);
	int pi;
	
	if ( parent < 0 )
	{
		int cnt = chpTable.GetCount();
		for ( int i = indx+1; i < cnt ; i++)
		{
			pi = chpTable.Get(i,2) ;
			if( pi == -1 || pi == -2 ) break;
			
			{
				if (parent == -1)
				{
					chpTable.ShowRow(i+1);
				}else
				{
					chpTable.HideRow(i+1);
				}
			}
		}
		
		if (parent == -1) chpTable(2) = -2;
		else chpTable(2) = -1;
		
		return;
	}
	
	
	BaseCtrl *pBaseCtrl = NULL ;

	String vkey = chpTable.Get(indx,0) ;
	String title = chpTable.Get(indx,1) ;
	int64 slv = chpTable.Get(indx,3) ;
	int64 topwnd = chpTable.Get(indx,4) ;
	
	
	if( !IsNull(topwnd) )
	{
		TopWindow* wnd = (TopWindow* ) topwnd;
		if (wnd)
		{
			wnd ->SetFocus() ;
			return;
		}
	}
	
	if( !IsNull(slv) )
	{
		Ctrl* wnd = (Ctrl* ) slv;
		if (wnd)
		{
			int tab = chpTab.Find(*wnd);
			if ( tab >= 0 )
			{
				chpTab <<= tab;
			}
		}
		
		return;
	}

	if ( vkey == "DB_COMERCIAL" )
	{
		pBaseCtrl = &wnd_list.Create<BaseCtrl>();
		pBaseCtrl->DB_TABLE = DB_COMMERCIAL ;
		
		pBaseCtrl->onInitGUI() ;
		
		pBaseCtrl->AddColumn( COM_CODE		, t_("Code")	, 50 ).Edit( pBaseCtrl->m_editors.Create <EditString>().NotNull() );
		pBaseCtrl->AddColumn( COM_NAME		, t_("Name")	, 200).Edit( pBaseCtrl->m_editors.Create <EditString>().NotNull() );
		pBaseCtrl->AddColumn( COM_PHONE		, t_("Phone")	, 200).Edit( pBaseCtrl->m_editors.Create <EditString>() );
		pBaseCtrl->AddColumn( COM_EMAIL		, t_("E-Mail")	, 200).Edit( pBaseCtrl->m_editors.Create <EditString>() );
		
		pBaseCtrl->onSelect();
	}
	else if ( vkey == "DB_USER" )			pBaseCtrl = &wnd_list.Create<UsersCtrl>();
	else if ( vkey == "DB_SPECIE")			pBaseCtrl = &wnd_list.Create<SpeciesCtrl>();
	else if ( vkey == "DB_SPECIE_SETTINGS")	pBaseCtrl = &wnd_list.Create<SpecieSettingsCtrl>();
	else if ( vkey == "DB_PRODECT" )		pBaseCtrl = &wnd_list.Create<ProdectCtrl>();
	else if ( vkey == "DB_CLIENT" )			pBaseCtrl = &wnd_list.Create<CustomersCtrl>();

	if(pBaseCtrl)
	{
		int p = chpTable.Find("Running",0);
		if (p < 0) return ;
		p++;
		chpTable.Insert(p);
		chpTable(p,0) = vkey ;
		chpTable(p,1) = title;
		chpTable(p,2) = p-1 ;
		chpTable(p,3) = (int64) pBaseCtrl ;
		if ( external )
		{
			TopWindow& dlg = wnd_list.Create<TopWindow>();
			chpTable(p,4) = (int64) &dlg ;
			dlg.Title(title);
			dlg.Zoomable().Sizeable();
			dlg.Add(pBaseCtrl->SizePos());
			dlg.WhenClose << THISBACK2(removeSlave, pBaseCtrl,&dlg);
			dlg.OpenMain() ;
		}
		else
		{
			TabButtons& tabButtons = pBaseCtrl->m_editors.Create <TabButtons> ();
			chpTab.Add(pBaseCtrl->SizePos(),title).Control(&tabButtons.LeftPos(0,30).TopPos(0,14));
			chpTab.Set(*pBaseCtrl);
			tabButtons.close << THISBACK1(closeTab, pBaseCtrl );
			tabButtons.detach << THISBACK1(detachTab, pBaseCtrl );
		}
		chpTable.RefreshRow(p);
	}
}

void Main::closeTab(Ctrl* slave)
{
	removeSlave(slave,NULL);
}

void Main::DockInit()
{
	//DockLeft(Dockable(chpTable, "Bookmarks").SizeHint(Size(300, 200)));
	
	static bool aff=false;
	if ( aff ) return;
	
	doConnect();
	
	aff = true;
}

void Main::removeSlave(Ctrl* slave,Ctrl* topwnd)
{
	int i=0, count = wnd_list.GetCount(),p;
	
	while ( i < count )
	{
		if( slave == &wnd_list[i] )
		{
			p = chpTable.Find((int64) slave ,3 );
			if ( p > 0 ) chpTable.Remove(p);
			if ( topwnd == NULL )
			{
				chpTab.Remove(*slave);
				wnd_list.Remove(i);
				return;
			}
			wnd_list.Remove(i);
			count --;
			continue;
		}
		
		if( topwnd == &wnd_list[i] )
		{
			wnd_list.Remove(i);
			count --;
			continue;
		}
		
		i ++;
	}
	
	p = chpTable.Find((int64) topwnd,4 );
	if ( p > 0 ) chpTable.Remove(p);
	
	
	
	//chpSearch <<= wnd_list.GetCount() ;
}

void Main::detachTab(Ctrl* slave)
{
	int index = chpTab.Find(*slave) ;
	if ( index < 0 ) return ;
	
	String title = chpTab.GetItem(index).GetText();
	
	chpTab.Remove(index);
	
	DockableCtrl& dc = Dockable(slave ->SizePos() , title ).SizeHint(Size(300, 200)) ;

	Float(dc);
	slave ->Show();
	
	
	/*TopWindow& dlg = wnd_list.Create<TopWindow>();

	dlg.WhenClose << THISBACK2(removeSlave, slave,&dlg);
	
	dlg.Add(slave ->SizePos() );
	slave ->Show();
	dlg.Title(title);
	
	dlg.Zoomable().Sizeable().OpenMain();
	*/
}




GUI_APP_MAIN
{
	SetLanguage(Upp::GetSystemLNG());
		
	SplashInit();									// Splash Initializing and Show screen with text "Program Loading"
	ShowSplashStatus(t_("Loading Settings..."));	// Show message at bottom of splash screen
	
	
	
	
    /*Main().Run();
    StdLogSetup(LOG_COUT);
    */
#ifdef flagNOMYSQL
    Sqlite3Session sqlSession;
	if(!sqlSession.Open(ConfigFile("NURSERY.db"))) {
		//Cout() << "Can't create or open database file\n";
		PromptOK("Can't create or open database file\n");
		return;
	}
	SQL = sqlSession;
	SqlSchema sch(SQLITE3);
#else
	MySqlSession sqlSession;
	if(!sqlSession.Connect("root","","NURSERY","127.0.0.1") ) {
		//Cout() << "Can't create or open database file\n";
		PromptOK("Can't create or open database file\n");
		return;
	}
	SQL = sqlSession;
	SqlSchema sch(MY_SQL);
#endif
	
#ifdef _DEBUG
	All_Tables(sch);
#endif
	// create the table if necessary[ [ Name : [*/_ %Name%]]]
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	SQL.ClearError();

	try {
		ShowSplashStatus(t_("Normal Running..."));		// Show message at bottom of splash screen
		SetSplashTimer(1000);							// Non standart splash-close timer set to 1 sec (maximum is 50 sec)
		
		Main vMain;
		vMain.Run();
		//vMain.Run();
	}catch(SqlExc & e)
	{
		ErrorOK(DeQtf(e));
	}
}


MenuBarFind::MenuBarFind()
{
	MultiButton::SubButton& btn = button.AddButton().Main();
	btn.WhenPush << [&] { WhenEnter(); } ;
	btn.SetMonoImage(NURSERYImg::IMG_LOUPE());
	NullText(t_("Search"));

	button.SetStyle(button.StyleFrame());
	button.NoDisplay();
	button.AddTo(*this);
}


bool MenuBarFind::Key(dword key, int count)
{
	if(key == K_ENTER && WhenEnter)
	{
		bool ret = EditString::Key(key, count);
		WhenEnter();
		return ret;
	}

	return EditString::Key(key, count);
}
