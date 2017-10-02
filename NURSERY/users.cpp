#include "NURSERY.h"

UsersCtrl::UsersCtrl()
{
	DB_TABLE = DB_USER;
	onInitGUI();
	onSelect();
}

UsersCtrl::~UsersCtrl()
{
}

void UsersCtrl::onCreateTableColumns()
{
	this ->AddColumn( USR_NAME		, t_("Name")	, 50 ).Edit( this ->m_editors.Create <EditString>().NotNull() );
	this ->AddColumn( USR_LOGIN		, t_("Login")	, 200).Edit( this ->m_editors.Create <EditString>().NotNull() );
	this ->AddColumn( USR_PASSWORD	, t_("Password"), 200).Edit( this ->m_editors.Create <EditString>() );
}

void UsersCtrl::onInitGUI()
{
	BaseCtrl::onInitGUI();
	m_group_list
		.GridColor(Color(229, 229, 229))
		.FullColResizing(false)
		.ResizingRows(true)
		.MovingCols(true)
		//.ResizeColMode(0)
		.LiveCursor(true)
		.ColorRows(true)
		.Indicator(false)
		.Appending(true)
		.Duplicating(true)
		.Removing(true)
		.Accepting(false)
		.Canceling(false)
		.Navigating(false)
		.Searching(true)
		.Editing(false)
		.Clipboard(false)
		.TabAddsRow(false)
		.EnterLikeTab(false)
		.Chameleon(true)
		.SetToolBar()
		;
		
	m_group_list.AddColumn( ID , "Id",50)
#ifdef _DEBUG
	.Hidden()
#endif
	;
	
	m_group_list.AddColumn( USG_NAME , t_("Group") , 250);
	chpRight.Add(m_group_list.SizePos());
	chpSepVert.SetPos(6000);
	chpTable.WhenCursor << THISBACK(onCursor) ;
	m_group_list.WhenNewRow << THISBACK(addUserToGroup);
	m_group_list.WhenRemoveRow << THISBACK(removeUserFroGroup);
}

void UsersCtrl::onToolBar(Bar& bar)
{
	bar.Add(t_("Settings")	, NURSERYImg::IMG_CLIENTS_SETTINGS()	,THISBACK(onSettings)) ;
	BaseCtrl::onToolBar(bar);
}

void UsersCtrl::onSettings()
{
	PromptOK("Not Defined yet !");
}

void UsersCtrl::onPrint()
{
	PromptOK("Not Defined yet !");
}

void UsersCtrl::onInsert()
{
	Sql sql;
	sql & Insert(DB_TABLE)
				(USR_LOGIN, ~ chpTable( USR_LOGIN ))
				(USR_NAME, ~ chpTable( USR_NAME ))
				(USR_PASSWORD, MD5String(~ chpTable( USR_PASSWORD )))
				;
	
	int userID = sql.GetInsertedId() ;
	chpTable( DB_ID ) = userID ;
	
	sql & Insert(DB_USER_GROUP)
					(USG_NAME,chpTable(USR_LOGIN))
				;
	int groupID = sql.GetInsertedId() ;
	
	sql & Insert(DB_USER_REFERENCE)
					(USG_ID,groupID)
					(USR_ID,userID)
					;
	
}

void UsersCtrl::onUpdate()
{
	Sql sql;
	sql & Upp::Update(DB_TABLE)
				(USR_LOGIN, ~ chpTable( USR_LOGIN ))
				(USR_NAME, ~ chpTable( USR_NAME ))
				(USR_PASSWORD, MD5String(~ chpTable( USR_PASSWORD )))
			.Where ( DB_ID == ~ chpTable( DB_ID ) )
			;
}


void UsersCtrl::onCursor()
{
	int userID = getID() ;
	if ( IsNull (userID) ) return;
	
	Sql sql;
	
	sql & Select (
		ID.Of ( DB_USER_GROUP ),
		USG_NAME.Of ( DB_USER_GROUP )
	)
			.From ( DB_USER_REFERENCE )
			.InnerJoin ( DB_USER_GROUP )
			.On ( ID.Of ( DB_USER_GROUP ) == USG_ID.Of ( DB_USER_REFERENCE ) )
			.Where ( USR_ID.Of ( DB_USER_REFERENCE ) == userID )
			.OrderBy(USG_NAME.Of ( DB_USER_GROUP ))
	;
	
	m_group_list.Clear() ;
	
	while( sql.Fetch() )
	{
		m_group_list.AddRow(sql[0] , sql[1] );
	}
	
	chpTable.SetFocus();
}


/*

void PopUpC::PopUp(Ctrl* owner)
{
	Size sz = AddFrameSize(GetSize());
	Rect wr = GetWorkArea();
	Rect r = owner->GetScreenRect();
	int x = r.left;
	int y = r.bottom;
	if(x + sz.cx >= wr.right)
		x = r.right - sz.cx;
	if(y + sz.cy >= wr.bottom)
		y = r.top - sz.cy;

	Rect rt = RectC(x, y, sz.cx, sz.cy);
	if(GUI_PopUpEffect()) {
		sPaintRedirectCtrl pb;
		pb.ctrl = this;
		Add(pb.BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		SetRect(RectC(rt.left, rt.top, 1, 1));
		Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(*this, rt, GUIEFFECT_SLIDE);
		pb.Remove();
	}
	else {
		SetRect(rt);
		Ctrl::PopUp(owner, true, true, true);
	}
	SetFocus();
}

*/
void UsersCtrl::addUserToGroup()
{
	int userID = getID() ;
	if ( IsNull (userID) ) return;

	int groupID = Null ;
	GridCtrl grpList;
	
	grpList
		.GridColor(Color(229, 229, 229))
		.LiveCursor(true)
		.ColorRows(true)
		.Searching(true)
		.Chameleon(true)
		.SetToolBar(true,BarCtrl::BAR_TOP);
	;
	grpList.AddColumn ( ID, t_ ( "Id" ), 0 , true )
#ifndef _DEBUG
	.Hidden()
#endif
	;

	grpList.AddColumn ( USG_NAME, t_ ( "Group" ), 150 );
	
	
	Sql sql;
	sql & Select ( SqlAll() )
					.From ( DB_USER_GROUP )
					.Where ( TRUE )
					.OrderBy(USG_NAME)
	;


	while ( sql.Fetch (  ) )
	{
		if ( m_group_list.Find(sql[0],ID) < 0 )
			grpList.Add (sql[0],sql[1]);
	}


	TopWindow dlg;
	dlg.Title ( t_ ( "Select group" ) )
			.SetRect ( 0, 0, 300, 300 )
			;

	dlg.Add ( grpList.SizePos() ) ;
	dlg.Run();

	
	grpList.WhenLeftDouble << [&]
	{

		if ( grpList.IsCursor() )
		{
			groupID = grpList.Get ( ID );
			dlg.Break ( IDOK );
		}
	};
	
	grpList.WhenEnter << grpList.WhenLeftDouble ;
	
	if ( !IsNull ( groupID ) )
	{
		sql & Insert ( DB_USER_REFERENCE )
						( USR_ID, userID )
						( USG_ID, groupID )
		;
		chpTable.WhenCursor();
	}
	
}

void UsersCtrl::removeUserFroGroup()
{
	int groupID=Null,userID=Null;
	userID = getID() ;
	if (IsNull (userID) || !m_group_list.IsCursor() ) return;
	
	groupID = m_group_list(ID);
	
	Sql sql;
	sql & Delete ( DB_USER_REFERENCE )
				.Where ( USG_ID == groupID && USR_ID == userID )
	;
		
}


