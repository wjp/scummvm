	void multidump();
	void frameoutv(uint8* dst, const uint8* src, uint16 pitch, uint16 width, uint16 height, uint16 x, uint16 y);
	void frameoutnm(uint8* dst, const uint8* src, uint16 pitch, uint16 width, uint16 height, uint16 x, uint16 y);
	void frameoutbh(uint8* dst, const uint8* src, uint16 pitch, uint16 width, uint16 height, uint16 x, uint16 y);
	void frameoutfx(uint8* dst, const uint8* src, uint16 pitch, uint16 width, uint16 height, uint16 x, uint16 y);
	void worktoscreen();
	void multiget();
	void convertkey();
	void cls();
	void printsprites();
	void quickquit();
	void readoneblock();
	void printundermon();
	void seecommandtail();
	void randomnumber();
	void quickquit2();
	Sprite* spritetable();
	void showframe();
	uint16 showframeCPP(uint16 dst, uint16 src, uint16 x, uint16 y, uint8 frameNumber, uint8 effectsFlag);
	void printasprite(const Sprite* sprite);
	void width160();
	void multiput();
	void eraseoldobs();
	void clearsprites();
	void makesprite();
	Sprite* makesprite(uint8 x, uint8 y, uint16 updateCallback, uint16 somethingInDx, uint16 somethingInDi);
	void spriteupdate();
	void mainmanCPP(Sprite* sprite);
	void backobject(Sprite* sprite);
	void constant(Sprite* sprite, ObjData* objData);
	void steady(Sprite* sprite, ObjData* objData);
	void dodoor();
	void dodoor(Sprite* sprite, ObjData* objData);
	void liftsprite();
	void liftsprite(Sprite* sprite, ObjData* objData);
	void turnpathonCPP(uint8 param);
	void turnpathoffCPP(uint8 param);
	void modifychar();
	void lockmon();

