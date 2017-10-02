#ifndef _NURSERY_species_h_
#define _NURSERY_species_h_

struct DispCombo : GridDisplay
{
	DropGrid& m_DropGrid;
	Event <Value& ,int> formater;Value result ;
	typedef DispCombo CLASSNAME;
	DispCombo(DropGrid& dropGrid) : m_DropGrid(dropGrid)
	{
		formater << THISBACK(defFormatter);
	}
	
	virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
			           Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
	{
		if( IsNull(val) )
		{
			GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
			return ;
		}
		
		int indx = m_DropGrid.Find(val);
		if (indx < 0)
		{
			GridDisplay::Paint(w, x, y, cx, cy, Value(""), style, fg, bg, fnt, found, fs, fe);
			return ;
		}
		
		
		formater(result , indx);
		GridDisplay::Paint(w, x, y, cx, cy, result , style, fg, bg, fnt, found, fs, fe);
	}
	
	void defFormatter(Value& rest,int ind)
	{
		rest = m_DropGrid.Get(ind,1);
	}
};


class SpecieSettingsDetails : public ParentCtrl
{
public:
	typedef SpecieSettingsDetails CLASSNAME;
	SpecieSettingsDetails();
	virtual ~SpecieSettingsDetails();
public:
	
	virtual void loadSpecieSettingsDetails(int sps_id);
	virtual void onInsert(int sps_id,int order);
	virtual void onUpdate();
	virtual void onDelete();
	virtual void loadWORKERSHOP();
	
	GridCtrl m_Details;
	Splitter m_HSerp;
	ImageCtrl m_Schema;
	
	
	GridDisplay *m_WOR_ID_Disp;
	
	DropGrid			m_WOR_ID;
	EditInt64Spin		m_SPS_WORKER_COUNT;
	EditDoubleSpin		m_SPS_LOSS;
	EditString			m_SPS_GREFFE_NAME;
};

class SpecieSettingsCtrl : public BaseCtrl
{
public:
	typedef SpecieSettingsCtrl CLASSNAME;
	SpecieSettingsCtrl();
	virtual ~SpecieSettingsCtrl();
public:
	virtual void onCreateTableColumns();
	virtual void onInitGUI();
	virtual void onSettings();
	virtual void onPrint();
	virtual void onCursor();
	virtual void onToolBar(Bar& bar);
	virtual void onNewUpdateDetail();
	virtual void onDeleteDetail();
	
	SpecieSettingsDetails m_details;
};

class ProdectCtrl : public BaseCtrl
{
public:
	typedef ProdectCtrl CLASSNAME;
	ProdectCtrl();
	virtual ~ProdectCtrl();
public:
	virtual void onCreateTableColumns();
	virtual void onInitGUI();
	virtual void onSettings();
	virtual void onPrint();
	virtual void onToolBar(Bar& bar);
};

class SpeciesCtrl : public BaseCtrl
{
public:
	typedef SpeciesCtrl CLASSNAME;
	SpeciesCtrl();
	virtual ~SpeciesCtrl();
public:
	virtual void onCreateTableColumns();
	virtual void onInitGUI();
	virtual void onSettings();
	virtual void onPrint();
	virtual void onToolBar(Bar& bar);
	virtual void onNewSpecieSettings();
	virtual void onNewProdect();
	virtual void initSpecieSettings();
	virtual void initProdect();
	
	DropGrid m_specieSettings,m_prodect;
	GridDisplay *m_specieSettingsDisp,*m_prodectDisp;
};

#endif
