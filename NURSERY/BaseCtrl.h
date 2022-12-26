#ifndef _NURSERY_BaseCtrl_h_
#define _NURSERY_BaseCtrl_h_

const Button::Style& CloseStyle();
const Button::Style& Detachtyle();

struct TabButtons : public ParentCtrl
{
	typedef TabButtons CLASSNAME;
	TabButtons()
	{
		Add(detach.LeftPos(0,14).TopPos(0,14));
		Add(close.LeftPos(15,14).TopPos(0,14));
		//close.SetStyle(CloseStyle());
		//detach.SetStyle(Detachtyle());
		close.Tip(t_("Close"));
		detach.Tip(t_("Detach"));
		
	}
	Button close,detach;
};

class BaseCtrl : public WithBaseLayout <ParentCtrl>
{
public:
	typedef BaseCtrl CLASSNAME;
	BaseCtrl();
	virtual ~BaseCtrl();
	
	virtual void onInitGUI();
	virtual void onCreateTableColumns();
	virtual void onSelect();
	virtual void onEdit(bool forceInsert);
	virtual void onInsert() ;
	virtual void onUpdate();
	virtual void onDelete();
	virtual void onToolBar(Bar& bar);
	virtual void initToolPosDropDown();
	virtual void onPrint();
	
	virtual Value getID();
	
	GridCtrl::ItemRect& AddColumn(const Id& id,const String& title,int w=100,byte req=0x0F);
	
	struct ColInfo
	{
		SqlId column;
		byte sqlReq=0x00;
	};
	
	SqlId				DB_TABLE
						,DB_ID;
	Array <ColInfo>		m_db_columns;
	Array <Ctrl>		m_editors;
	ToolBar toolBar;
};

#endif
