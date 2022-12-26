#ifndef _NURSERY_NURSERY_h
#define _NURSERY_NURSERY_h

#include <CtrlLib/CtrlLib.h>
#include <MySql/MySql.h>
#include <SqlCtrl/SqlCtrl.h>
#include <plugin/sqlite3/Sqlite3.h>
#include <GridCtrl/GridCtrl.h>
// #include <Styler/Styler.h>
#include <Docking/Docking.h>
#include <RichEdit/RichEdit.h>
#include <DropGrid/DropGrid.h>
//#include <PopUpC/PopUpC.h>


using namespace Upp;

class MenuBarFind : public EditString
{
	private:
		MultiButtonFrame button;
	public:

		MenuBarFind();

		//Event<> WhenEnter;
		virtual bool Key(dword key, int count);
		/*virtual Size GetMinSize() const;*/

		void Push();

		typedef MenuBarFind CLASSNAME;

};

#define LAYOUTFILE <NURSERY/NURSERY.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS NURSERYImg
#define IMAGEFILE <NURSERY/NURSERY.iml>
#include <Draw/iml_header.h>

#ifdef flagNOMYSQL
	#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
	#define MODEL <NURSERY/schema.sch>
	#include "Sql/sch_header.h"
#else
	#define SCHEMADIALECT <MySql/MySqlSchema.h>
	#define MODEL <NURSERY/schema.sch>
	#include "Sql/sch_header.h"
#endif





#include "BaseCtrl.h"
#include "customers.h"
#include "species.h"
#include "users.h"


class Main : public WithMainLayout<DockWindow> {
public:
	typedef Main CLASSNAME;
	Main();
	virtual ~Main();
	
	void closeTab(Ctrl* slave);
	void removeSlave(Ctrl* slave,Ctrl* topwnd);
	void detachTab(Ctrl* slave);
	
	virtual void DockInit();

	virtual void doConnect();
	virtual void doDisconnect();
	virtual void doInitApp();
	
	virtual void createEditor(bool external);
	void initGUI();
	
	S_DB_USER m_user;
	MenuBar menuBar;
	Id COL_KEY,COL_LIB,COL_PARENT,COL_ADDRESS,COL_ADDRESS2;
	Array <Ctrl> wnd_list;
};

#endif
