#include "NURSERY.h"
/*
CH_LOCSTYLE(Button, Style, CloseStyle)
{
	look[0] = NURSERYImg::IMG_CLOSE1();
	look[1] = NURSERYImg::IMG_CLOSE2();
	look[2] = NURSERYImg::IMG_CLOSE3();
	look[3] = NURSERYImg::IMG_CLOSE4();
}

CH_LOCSTYLE(Button, Style, Detachtyle)
{
	look[0] = NURSERYImg::IMG_DETACH1();
	look[1] = NURSERYImg::IMG_DETACH2();
	look[2] = NURSERYImg::IMG_DETACH3();
	look[3] = NURSERYImg::IMG_DETACH4();
}*/

BaseCtrl::BaseCtrl() : DB_ID("ID")
{
	CtrlLayout(*this);
}

BaseCtrl::~BaseCtrl()
{
}

void BaseCtrl::onInitGUI()
{
	chpSepVert << chpTable << chpRight ;
	chpSepVert.SetPos(10000);
	/* ID */
	chpTable.AddColumn(DB_ID, "ID",30)
#ifndef _DEBUG
	.Hidden()
#endif
	;
	
	onCreateTableColumns();
	chpTable.AskRemove();
	//chpTable.SetToolBar(true,BarCtrl::BAR_TOP);
	chpTable.WhenAcceptedRow	<< THISBACK1(onEdit, false) ;
	chpTable.WhenDuplicateRow	<< THISBACK1(onEdit, true) ;
	chpTable.WhenRemoveRow		<< THISBACK(onDelete);
	chpTable.WhenMenuBar		<< THISBACK(onToolBar);
	chpTable.WhenToolBar		<< THISBACK(onToolBar);
	AddFrame( chpTable.GetToolBar() );
	initToolPosDropDown();
}

void BaseCtrl::onCreateTableColumns()
{
}



void BaseCtrl::onSelect()
{
	SqlSet s;
	SqlId id;
	s.Cat(DB_ID) ;
	for (int i=0; i < m_db_columns.GetCount(); i++)
	{
		if ( m_db_columns[i].sqlReq & 0x01 )
			s.Cat(m_db_columns[i].column);
	}
	
	Sql sql;
	sql & Select(s)
		.From (DB_TABLE);
	int added;
	
	chpTable.Clear();
	
	while ( sql.Fetch() )
	{
		added = chpTable.Append(1) - 1;
		
		sql.GetColumn( DB_ID , chpTable( added, DB_ID) );
		
		for (int i=0; i < m_db_columns.GetCount(); i++)
		{
			if ( m_db_columns[i].sqlReq & 0x01 )
				sql.GetColumn( m_db_columns[i].column , chpTable( added, m_db_columns[i].column ) );
		}
	}
}

void BaseCtrl::onEdit(bool forceInsert)
{
	if (forceInsert)
		onInsert() ;
	else
	{
		Value id = this ->getID() ;
		
		if( !IsNull(id) )
		{
			onUpdate();
		}
		else
		{
			onInsert();
		}
	};
}

void BaseCtrl::onInsert()
{
	SqlInsert s(DB_TABLE);
	for (int i=0; i < m_db_columns.GetCount(); i++)
	{
		if ( m_db_columns[i].sqlReq & 0x02 )
			s(m_db_columns[i].column, chpTable(m_db_columns[i].column) );
	}
	
	Sql sql;
	sql & s ;
	
	chpTable( DB_ID ) = sql.GetInsertedId() ;
}

void BaseCtrl::onUpdate()
{
	SqlUpdate s(DB_TABLE);
	for (int i=0; i < m_db_columns.GetCount(); i++)
	{
		if ( m_db_columns[i].sqlReq & 0x04 )
			s(m_db_columns[i].column, chpTable(m_db_columns[i].column) );
	}
	s.Where( DB_ID == chpTable(DB_ID) ) ;
	
	Sql sql;
	sql & s ;
}

void BaseCtrl::onDelete()
{
	Sql sql;
	sql & Delete(DB_TABLE)
		.Where( DB_ID == chpTable(DB_ID) ) ;
}

void BaseCtrl::initToolPosDropDown()
{
	chpDrop.Add(t_("Bottom"));
	chpDrop.Add(t_("Top"));
	chpDrop.Add(t_("Left"));
	chpDrop.Add(t_("Right"));
	
	chpDrop <<= chpDrop.GetValue(1) ;
	
	chpDrop << [&]
	{
		int index = chpDrop.GetIndex();
		switch (index)
		{
			case 1:
				chpTable.GetToolBar().Align(BarCtrl::BAR_TOP);
				break;
			case 2:
				chpTable.GetToolBar().Align(BarCtrl::BAR_LEFT);
				break;
			case 3:
				chpTable.GetToolBar().Align(BarCtrl::BAR_RIGHT);
				break;
			default:
				chpTable.GetToolBar().Align(BarCtrl::BAR_BOTTOM);
				break;
		};
	};
}
/*

::@2 [s0;= [1 %CLT`_CODE%]]
:: [s0;= [1 %CLT`_NOM%]]
:: [s0;= [1 %CLT`_ADDRESSE%]]
:: [s0;= [1 %CLT`_TEL%]]
:: [s0;= [1 %CLT`_EMAIL%]]
:: [s0;= [1 %CLT`_CONTACT%]]


#include "templates.brc"
*/
void BaseCtrl::onPrint()
{
	//if ( chpSepVert.GetPos() == 10000 )
	//{
	/*
		WhenSwitch();
		
		String qtf (ETAT_CLIENTS,ETAT_CLIENTS_length) ,mQtf,tmp;
		
	
		for (int j=0; j < chpTable.GetCount(); j++)
		{
			tmp =
"::@2 [s0;= [1 %CLT`_CODE%]]"\
":: [s0;= [1 %CLT`_NOM%]]"\
":: [s0;= [1 %CLT`_ADDRESSE%]]"\
":: [s0;= [1 %CLT`_TEL%]]"\
":: [s0;= [1 %CLT`_EMAIL%]]"\
":: [s0;= [1 %CLT`_CONTACT%]]";

			for (int i=0; i < m_db_columns.GetCount(); i++)
			{
				
				if ( m_db_columns[i].sqlReq & 0x01 )
					tmp.Replace("%"+DeQtf(m_db_columns[i].column.ToString())+"%",DeQtf(chpTable(j,m_db_columns[i].column).ToString() ));
			}
			mQtf << tmp ;
		}
		
		chpLabel.SetQTF(Format(qtf,mQtf,chpTable.GetCount())) ;
		
		
		//chpSepVert.SetPos(pos); RichEditWithToolBar
		
		
	/*} else
	{
		pos = chpSepVert.GetPos() ;
		chpSepVert.SetPos(10000);
	}
	*/
}



void BaseCtrl::onToolBar(Bar& bar)
{
	bar.Add(t_("Refresh")	,CtrlImg::Toggle()	,THISBACK(onSelect))	.Key(K_F5);
	bar.Add(t_("Print")		,CtrlImg::print()	,THISBACK(onPrint))	.Key(K_CTRL_P);
	
	bar.AddTool(chpDrop.SizePos(),70);
}


Value BaseCtrl::getID()
{
	if(!chpTable.IsCursor()) return Null ;
	return chpTable(DB_ID);
}


GridCtrl::ItemRect& BaseCtrl::AddColumn(const Id& id,const String& title,int w, byte req)
{
	ColInfo& si = m_db_columns.Add() ;
	si.column = id;
	si.sqlReq = req;
	
	return chpTable.AddColumn(id,title,w);
}



