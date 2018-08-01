// test namemng

#include "namemng.h"
#include "wizdebug.h"

#define CHK(expr) { \
 if(!expr) {\
	 wDebug(("*** Error file %s line %d while %s",\
			__FILE__, __LINE__, #expr ));\
	 return false;\
 }\
}

bool nameMngTest()
{
	NameMng mng(QString("TestMng"));
	bool gotExp, ar;
	void *caller1=(void *)1; 
	void *caller2=(void *)2; 

	try {
		// test good Alloc
		ar = mng.alloc( QString("hello"), caller1);
		CHK( ar==true );
		ar = mng.alloc( QString("blabla"), caller2);
		CHK( ar==true );

		// test isAvail
		CHK( mng.isAvail( QString("hello")) == false );
		CHK( mng.isAvail( QString("hello1")) == true );

		// test free
		mng.free( QString("hello"), caller1);

		gotExp = false;
		try {
			mng.free( QString("hello1"), caller1);
		}
		catch( NameMng::NotAllocated &e ){
			gotExp = true;
		}
		CHK( gotExp==true );

		gotExp = false;
		try {
			mng.free( QString("blabla"), caller1);
		}
		catch( NameMng::NotOwner &e ){
			gotExp = true;
		}
		CHK( gotExp==true );

		// test suggest
		CHK( mng.suggest("A201") == QString("A201_1"));

		ar = mng.alloc( QString("A201_1"), caller1);
		CHK( ar==true );
		
		CHK( mng.suggest("A201") == QString("A201_2"));

		mng.dump();
	}
	catch( NameMng::Except &e ){
		wDebug(("Got exception %s", (const char *)e.getMsg()));
		CHK(0);
	}
	wDebug(("nameMngTest ok!\n"));
	return true;
}
