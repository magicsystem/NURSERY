#include "NURSERY.h"


ProdectCtrl::ProdectCtrl()
{
	DB_TABLE = DB_PRODECT;
	onInitGUI();
	onSelect();
}

ProdectCtrl::~ProdectCtrl()
{
}

void ProdectCtrl::onCreateTableColumns()
{
	AddColumn( PRD_CODE		, t_("Code")	, 50 ).Edit( m_editors.Create <EditString>() .NotNull() );
//	AddColumn( PRD_ID		, t_("Prodect ID")			, 200).Edit( m_prodect.NotNull() );
	AddColumn( PRD_NAME		, t_("Name")				, 200).Edit( m_editors.Create <EditString>() );
//	AddColumn( SPE_TYPE		, t_("Type")				, 200).Edit( m_editors.Create <EditString>() );
}

void ProdectCtrl::onInitGUI()
{
	BaseCtrl::onInitGUI();
}

void ProdectCtrl::onSettings()
{
}

void ProdectCtrl::onPrint()
{
}

void ProdectCtrl::onToolBar(Bar& bar)
{
	BaseCtrl::onToolBar(bar);
}

