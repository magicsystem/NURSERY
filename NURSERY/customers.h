#ifndef _NURSERY_customers_h_
#define _NURSERY_customers_h_

class CustomersCtrl
					: public BaseCtrl
{
public:
	typedef CustomersCtrl CLASSNAME;
	CustomersCtrl();
	virtual ~CustomersCtrl();
	
	virtual void onCreateTableColumns();
	virtual void onInitGUI();
	virtual void onSettings();
	virtual void onPrint();
	virtual void onToolBar(Bar& bar);
	
	//RichTextCtrl
	RichEditWithToolBar
							chpLabel;
};

#endif
