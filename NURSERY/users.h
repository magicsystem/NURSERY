#ifndef _NURSERY_users_h_
#define _NURSERY_users_h_


class UsersCtrl
					: public BaseCtrl
{
public:
	typedef UsersCtrl CLASSNAME;
	UsersCtrl();
	virtual ~UsersCtrl();
	
	virtual void onCreateTableColumns();
	virtual void onInitGUI();
	virtual void onSettings();
	virtual void onPrint();
	virtual void onToolBar(Bar& bar);

	virtual void onInsert();
	virtual void onUpdate();
	virtual void onUpdatePassword();
	
	virtual void onCursor();
	virtual void addUserToGroup();
	virtual void removeUserFroGroup();
	
	String password;
	GridCtrl m_group_list;
};

#endif
