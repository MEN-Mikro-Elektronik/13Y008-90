// test resourcemng

#include "resourcemng.h"
#include "hwcomponent.h"
#include "wizdebug.h"

#define CHK(expr) { \
 if(!(expr)) {\
	 wDebug(("*** Error file %s line %d while %s",\
			__FILE__, __LINE__, #expr ));\
	 return false;\
 }\
}

bool resourceMngTest()
{
	ResourceMng mng(QString("TestRes"));
	bool gotExp;
	ulong s;
	HwComponent n1;
	HwComponent *owner;
	ResourceMng::AllocResult ar;
	ResourceMng::IsAvailResult isAvail;
	
	try {
		mng.addRegion( 0x1344, 0x5000 );
		mng.addRegion( 0x10000, 0x1000 );
		mng.addRegion( 0x11000, 0x5000 ); // test region combining
		//mng.dump();
	
		// attempt to add region that was already added
		gotExp = false;

		try {
			mng.addRegion( 0x1348, 0x1 );
		} 
		catch( ResourceMngAlreadyAvail &e ){
			gotExp = true;
		}
		CHK( gotExp );

		mng.dump();

		// test good Alloc
		wDebug(("Test alloc1"));
		ar = mng.alloc( 0x200, 0x100, s, &n1, (ulong)0x1566 );
		CHK( ar == ResourceMng::AllocResOk );
		CHK( s==0x1600 );
		mng.dump();

		wDebug(("Test alloc2"));

		ar = mng.alloc( 0x4b44, 0x100, s, &n1 ); // covers entire region #1
		CHK( ar == ResourceMng::AllocResOk );
		CHK( s==0x1800 );
		mng.dump();

		// test bad alloc (size too big)
		ar = mng.alloc( 0x20000, 0x100, s, &n1 );
		CHK( ar == ResourceMng::AllocResFail );

		// free the regions
		wDebug(("Test free"));
		mng.free( 0x1800, &n1 );
		mng.dump();
		mng.free( 0x1600, &n1 );
		mng.dump();
	
		// test isAvail
		wDebug(("Test isAvail"));
		isAvail = mng.isAvail( 0x10000, 0x6000, &owner );
		CHK( isAvail==ResourceMng::Avail );

		isAvail = mng.isAvail( 0x10000, 0x6001, &owner );
		CHK( isAvail==ResourceMng::Unavail );

		isAvail = mng.isAvail( 0x20000, 0x6001, &owner );
		CHK( isAvail==ResourceMng::Unavail );

		ar = mng.alloc( 0x1, 0x1, s, &n1, 0x10000 );
		mng.dump();

		isAvail = mng.isAvail( 0x10000, 0x1, &owner );
		CHK( isAvail==ResourceMng::InUse );
		CHK( owner==&n1 );
		
		isAvail = mng.isAvail( 0x10000, 0x2, &owner );
		CHK( isAvail==ResourceMng::InUse );
		CHK( owner==&n1 );
		
		isAvail = mng.isAvail( 0x10001, 0x1, &owner );
		CHK( isAvail==ResourceMng::Avail );
				
	}
	catch( ResourceMngExcept &e ){
		wDebug(("Got exception %s", (const char *)e.getMsg()));
		CHK(0);
	}
	wDebug(("resourceMngTest ok!\n"));
	return true;
}
