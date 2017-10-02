#include "NURSERY.h"

SpecieSettingsCtrl::SpecieSettingsCtrl()
{
	DB_TABLE = DB_SPECIE_SETTINGS;
	onInitGUI();
	onSelect();
}

SpecieSettingsCtrl::~SpecieSettingsCtrl()
{
}

void SpecieSettingsCtrl::onCreateTableColumns()
{
	AddColumn( SPS_NAME					, t_("Name")		, 200).Edit( this ->m_editors.Create <EditString>() );
	AddColumn( SPS_TOTAL_DAYS_COUNT		, t_("N days")		, 100, 0x01);
	AddColumn( SPS_TOTAL_WORKER_COUNT	, t_("N Workers ")	, 100, 0x01);
	AddColumn( SPS_TOTAL_LOSS			, t_("Total loss")	, 100, 0x01);
}

void SpecieSettingsCtrl::onInitGUI()
{
	BaseCtrl::onInitGUI();
	chpRight.Add(m_details.SizePos());
	//m_details.m_Details.WhenInsertRow << THISBACK(onNewDetail);
	m_details.m_Details.WhenAcceptRow << THISBACK(onNewUpdateDetail);
	m_details.m_Details.WhenRemoveRow << THISBACK(onDeleteDetail);
	
	chpTable.WhenCursor << THISBACK(onCursor);
	chpSepVert.SetPos(5000);
}

void SpecieSettingsCtrl::onSettings()
{
}

void SpecieSettingsCtrl::onPrint()
{
}

void SpecieSettingsCtrl::onCursor()
{
	m_details.loadSpecieSettingsDetails(getID());
}



void SpecieSettingsCtrl::onToolBar(Bar& bar)
{
	BaseCtrl::onToolBar(bar);
}

void SpecieSettingsCtrl::onNewUpdateDetail()
{
	int id = getID() ;
	if ( IsNull ( id ) )
	{
		Exclamation(DeQtf(t_("Please select a specie setting to continue !"))) ;
		return;
	}
	
	if (!m_details.m_Details.IsCursor()) return;
	
	int did = m_details.m_Details(ID);
	if ( did <= 0 )
	{
		int sel = m_details.m_Details.GetCursor() + 1 ;
		m_details.onInsert(id,sel) ;
	} else
	{
		m_details.onUpdate();
	}
	
	
	// TODO calcul totals
}


void SpecieSettingsCtrl::onDeleteDetail()
{
	m_details.onDelete();
	// TODO calcul totals
}

