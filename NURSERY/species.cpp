#include "NURSERY.h"

SpeciesCtrl::SpeciesCtrl()
{
	DB_TABLE = DB_SPECIE;
	m_specieSettingsDisp = new DispCombo(m_specieSettings);
	m_prodectDisp = new DispCombo(m_prodect);
	onInitGUI();
	onSelect();
}

SpeciesCtrl::~SpeciesCtrl()
{
	if(m_specieSettingsDisp) delete m_specieSettingsDisp;
	if(m_prodectDisp) delete m_prodectDisp;
}

void SpeciesCtrl::onCreateTableColumns()
{
	this ->AddColumn( SPS_ID		, t_("Species settings")	, 150 ).Edit( m_specieSettings.NotNull() ).SetDisplay(*m_specieSettingsDisp);
	this ->AddColumn( PRD_ID		, t_("Prodect ID")			, 200).Edit( m_prodect.NotNull() ).SetDisplay(*m_prodectDisp);
	this ->AddColumn( SPE_NAME		, t_("Name")				, 200).Edit( this ->m_editors.Create <EditString>() );
	this ->AddColumn( SPE_TYPE		, t_("Type")				, 200).Edit( this ->m_editors.Create <EditString>() );
}

void SpeciesCtrl::onInitGUI()
{
	m_specieSettings.AddPlus(THISBACK(onNewSpecieSettings));
	initSpecieSettings();
	
	m_prodect.AddPlus(THISBACK(onNewProdect));
	initProdect();
	
	BaseCtrl::onInitGUI();
	//chpRight.Add(chpLabel.SizePos());
}

void SpeciesCtrl::onToolBar(Bar& bar)
{
	bar.Add(t_("Settings")	, NURSERYImg::IMG_CLIENTS_SETTINGS()	,THISBACK(onSettings)) ;
	BaseCtrl::onToolBar(bar);
}

void SpeciesCtrl::onNewSpecieSettings()
{
	TopWindow dlg;
	SpecieSettingsCtrl ctrl;
	dlg.Add(ctrl.SizePos());
	dlg.Zoomable().Sizeable().Run();
	initSpecieSettings();
}

void SpeciesCtrl::onNewProdect()
{
	TopWindow dlg;
	ProdectCtrl ctrl;
	dlg.Add(ctrl.SizePos());
	dlg.Zoomable().Sizeable().Run();
	initProdect();
}

void SpeciesCtrl::initProdect()
{
	m_prodect.Clear();
	Sql sql;
	sql & Select(ID , PRD_CODE , PRD_NAME )
		.From(DB_PRODECT)
/*		.Where(GR_ID == ID.Of(GROUPS))*/
		.OrderBy(PRD_CODE );

	while(sql.Fetch())
		m_prodect.Add(sql[0], Format("%s - %s", sql[1], sql[2]));
}

void SpeciesCtrl::initSpecieSettings()
{
	m_specieSettings.Clear();
	Sql sql;
	sql & Select(ID , SPS_NAME , SPS_TOTAL_DAYS_COUNT , SPS_TOTAL_LOSS )
		.From(DB_SPECIE_SETTINGS)
/*		.Where(GR_ID == ID.Of(GROUPS))*/
		.OrderBy(SPS_NAME );

	while(sql.Fetch())
		m_specieSettings.Add(sql[0], Format("%s - %s", sql[2], sql[1]));
}

void SpeciesCtrl::onSettings()
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

void SpeciesCtrl::onPrint()
{
		
}


