#include "NURSERY.h"

SpecieSettingsDetails::SpecieSettingsDetails()
{
	DispCombo* vDispCombo = new DispCombo(m_WOR_ID) ;
	vDispCombo ->formater <<  [&](Value& res, int ind) {
		res = Format("[%s] %s" , m_WOR_ID(ind,1), m_WOR_ID(ind,2)) ;
	};
	m_WOR_ID_Disp = vDispCombo;
	m_WOR_ID.AddColumn(ID,"ID",30,true);
	m_WOR_ID.AddColumn(WOR_CODE,t_("Code"),70);
	m_WOR_ID.AddColumn(WOR_NAME,t_("Name"),130);
	m_WOR_ID.AddColumn(WOR_TRAY_COUNT,t_("Tay count"),100);
	m_WOR_ID.AddColumn(WOR_WORKER_COUNT,t_("Worker count"),100);
	
	m_WOR_ID.SetValueColumn(2);
	m_HSerp << m_Schema << m_Details;
	m_HSerp.Vert();
	m_HSerp.SetPos(2000);
	
	m_Details.AddColumn(ID			,"ID",70,true);
	m_Details.AddColumn(SSD_ORDER			,t_("ORDER"),70);
	m_Details.AddColumn(WOR_ID				,t_("Workshop"),170)		.Edit(m_WOR_ID).SetDisplay(*m_WOR_ID_Disp);
	m_Details.AddColumn(SPS_GREFFE_NAME		,t_("Porte Greffe"),170).Edit(m_SPS_GREFFE_NAME);
	m_Details.AddColumn(SPS_WORKER_COUNT	,t_("N WORKER"),70)		.Edit(m_SPS_WORKER_COUNT);
	m_Details.AddColumn(SPS_LOSS			,t_("% Loss"),70)		.Edit(m_SPS_LOSS);
	
	Add(m_HSerp.SizePos()) ;
	
	m_Details
		.GridColor(Color(229, 229, 229))
		.Sorting(false)
		.MovingCols(true)
		.Indicator(false)
		.Inserting(true)
		.Appending(true)
		.Duplicating(true)
		.Removing(true)
		.Accepting(true)
		.Canceling(true)
		.Editing(true)
		.Chameleon(true)
		.SetToolBar()
		;
	loadWORKERSHOP();
}

SpecieSettingsDetails::~SpecieSettingsDetails()
{
	if (m_WOR_ID_Disp) delete m_WOR_ID_Disp;
}

void SpecieSettingsDetails::loadSpecieSettingsDetails(int sps_id)
{
	m_Details.Clear() ;
	if ( sps_id <= 0 ) return;
	
	Sql sql;
	SqlSet s;
	
	for (int i=0; i < m_Details.GetColumnCount(); i++)
	{
		s.Cat(SqlId(m_Details.GetColumnId(i))) ;
	}
	
	sql & Select (s)
			.From(DB_SPECIE_SETTINGS_DETAILS)
			.Where( SPS_ID == sps_id )
			.OrderBy(SSD_ORDER)
			;
	while (sql.Fetch())
	{
		sps_id = m_Details.Append() - 1 ;
		for (int i=0; i < m_Details.GetColumnCount(); i++)
		{
			sql.GetColumn(i,m_Details(sps_id,i));
		}
	}
}

void SpecieSettingsDetails::onInsert(int sps_id,int order)
{
	if ( ! m_Details.IsCursor() ) return ;
	if ( IsNull(sps_id) ) return ;
	
	Sql sql ;
	sql & Upp::Insert(DB_SPECIE_SETTINGS_DETAILS)
			(SPS_ID , sps_id)
			(SSD_ORDER , order)
			(WOR_ID , m_Details(WOR_ID) )
			(SPS_GREFFE_NAME , m_Details(SPS_GREFFE_NAME) )
			(SPS_WORKER_COUNT , m_Details(SPS_WORKER_COUNT) )
			(SPS_LOSS , m_Details(SPS_LOSS) )
	;
	
	m_Details(ID) = sql.GetInsertedId() ;
	m_Details(SSD_ORDER) = order ;
	
	for ( int i = order ; i < m_Details.GetCount() ; i++ )
	{
		sql & Upp::Update(DB_SPECIE_SETTINGS_DETAILS)
					(SSD_ORDER , (i+1) )
				.Where( ID == m_Details(i,ID) )
				;
		m_Details(i,SSD_ORDER) = i+1;
	}
	
	m_Details.Refresh();
}

void SpecieSettingsDetails::onUpdate()
{
	Sql sql ;
	sql & Upp::Update(DB_SPECIE_SETTINGS_DETAILS)
			(SPS_GREFFE_NAME , m_Details(SPS_GREFFE_NAME) )
			(SPS_WORKER_COUNT , m_Details(SPS_WORKER_COUNT) )
			(SPS_LOSS , m_Details(SPS_LOSS) )
		.Where( ID == m_Details(ID) )
	;
}

void SpecieSettingsDetails::onDelete()
{
	Sql sql ;
	sql & Upp::Delete(DB_SPECIE_SETTINGS_DETAILS)
		.Where( ID == m_Details(ID) )
	;
}

void SpecieSettingsDetails::loadWORKERSHOP()
{
	m_WOR_ID.Clear() ;
	Sql sql;
	
	sql & Select (ID,WOR_CODE,WOR_NAME,WOR_TRAY_COUNT,WOR_WORKER_COUNT)
			.From(DB_WORKSHOP)
			.OrderBy(WOR_CODE,WOR_NAME)
	;
	
	while ( sql.Fetch() )
	{
		m_WOR_ID.Add(sql[0],sql[1],sql[2],sql[3],sql[4]);
	}
}


