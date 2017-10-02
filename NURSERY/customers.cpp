#include "NURSERY.h"

CustomersCtrl::CustomersCtrl()
{
	DB_TABLE = DB_CUSTOMER;
	onInitGUI();
	onSelect();
}

CustomersCtrl::~CustomersCtrl()
{
}

void CustomersCtrl::onCreateTableColumns()
{
	this ->AddColumn( CUS_CODE		, t_("Code")	, 50 ).Edit( this ->m_editors.Create <EditString>().NotNull() );
	this ->AddColumn( CUS_NAME		, t_("Name")	, 200).Edit( this ->m_editors.Create <EditString>().NotNull() );
	this ->AddColumn( CUS_CONTACT	, t_("Contact")	, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( CUS_ADDRESS	, t_("Address")	, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( CUS_PHONE		, t_("Phone")	, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( CUS_FAX		, t_("Fax")		, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( CUS_CITY		, t_("City")	, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( CUS_EMAIL		, t_("E-Mail")	, 200).Edit( this ->m_editors.Create <EditString>() );
}

void CustomersCtrl::onInitGUI()
{
	BaseCtrl::onInitGUI();
	chpRight.Add(chpLabel.SizePos());
}

void CustomersCtrl::onToolBar(Bar& bar)
{
	bar.Add(t_("Settings")	, NURSERYImg::IMG_CLIENTS_SETTINGS()	,THISBACK(onSettings)) ;
	BaseCtrl::onToolBar(bar);
}

void CustomersCtrl::onSettings()
{
	TopWindow dlg;
	BaseCtrl baseCtrl;
	baseCtrl.DB_TABLE = DB_SETTINGS;
	
	DropList drpType;
	drpType.Add(1,t_("Header"));
	drpType.Add(2,t_("Body"));
	drpType.Add(3,t_("Footer"));
	
	LineEdit rch;
	
	RichEditWithToolBar rewtb;
	
	baseCtrl.chpRight.Add(rewtb.SizePos()) ;
	
	baseCtrl.chpTable.WhenCursor << [&]
	{
		if( ! baseCtrl.chpTable.IsCursor() ) return ;
		rewtb.SetQTF(baseCtrl.chpTable(CLS_TEMPLATE).ToString() );
	};
	
	baseCtrl.onInitGUI();
	
	baseCtrl.AddColumn( CLS_TYPE		, t_("Type")	, 50 ).Edit( drpType.NotNull() );
	baseCtrl.AddColumn( CLS_TEMPLATE	, t_("Template"), 250 ).Edit( rch );
	
	
	baseCtrl.onSelect();
	dlg.Add(baseCtrl.SizePos());
	dlg.Zoomable().Zoomable().Run();
}

void CustomersCtrl::onPrint()
{
	Array <S_DB_SETTINGS> settings;
	int h=-1,b=-1,f=-1,cnt;
	Sql sql;
	
	sql & Select (SqlAll() )
		.From(DB_SETTINGS)
	;
	
	cnt	= 0;
	while (sql.Fetch(settings.Add()))
	{
		switch ( (int ) settings[cnt].CLS_TYPE )
		{
			case 1:
				h = cnt;
				break;
			case 2:
				b = cnt;
				break;
			case 3:
				f = cnt;
				break;
		}
		cnt++;
	}
	
	
	String qtf,mQtf,tmp;
		
	if ( b >= 0 )
	{
		for (int j=0; j < chpTable.GetCount(); j++)
		{
			tmp = settings[b].CLS_TEMPLATE ;

			for (int i=0; i < m_db_columns.GetCount(); i++)
			{
				if ( m_db_columns[i].sqlReq & 0x01 )
					tmp.Replace(
						"%"+DeQtf( m_db_columns[i].column.ToString() )+"%",
						DeQtf( chpTable(j,m_db_columns[i].column).ToString() )
					);
			}
			mQtf << tmp ;
		}
		
	}
	
	if ( h >= 0 )
	{
		qtf = settings[h].CLS_TEMPLATE ;
		
		chpLabel.SetQTF(Format(qtf,mQtf,chpTable.GetCount())) ;
	}
	
}


