#ifndef __PACKET_DEF
#define __PACKET_DEF

#define PKT_LOGIN							0x01
#define PKT_LOGOUT							0x02
#define PKT_MOVEFIRST						0x03
#define PKT_MOVEMIDDLE						0x04
#define PKT_MOVEEND							0x06
#define PKT_CHAT							0x07
#define PKT_RESTED							0x08
#define PKT_USERMODIFY						0x09
#define PKT_ZONECHANGE						0x0A
#define PKT_GAMESTART           			0x0B
#define PKT_RESTARTGAME         			0x0C
#define PKT_GAME_PAUSE						0x0D
#define PKT_MOBHP							0x0E
#define PKT_PENALTY							0x0F

#define PKT_ATTACK							0x10
#define PKT_ATTACK_ARROW					0x11
#define PKT_ATTACK_MAGIC_RAIL				0x12
#define PKT_MAGIC_READY         			0x13
#define PKT_MAGIC_CIRCLE        			0x14
#define PKT_RUN_MOVEFIRST					0x15
#define PKT_RUN_MOVEMIDDLE					0x16
#define PKT_RUN_MOVEEND						0x17
#define PKT_BATTLEMODE						0x18
#define PKT_STATUS_INFO						0x19

#define PKT_POISON							0x20
#define PKT_DEAD                			0x23
#define PKT_DEAD_INFO           			0x24
#define PKT_LIFE                			0x25
#define PKT_MAGIC_ALL						0x26
#define PKT_MAGIC_BELT						0x27
#define PKT_SET_TIME						0x29
#define PKT_SHOW_MAGIC						0x2A

#define PKT_INV_ALL             			0x30
#define PKT_CHANGE_ITEM_INDEX   			0x32
#define PKT_ITEM_INFO           			0x33
#define PKT_ITEM_THROW          			0x34
#define PKT_ITEM_PICKUP         			0x35
#define PKT_ITEM_FIELD_INFO     			0x36
#define PKT_GETITEM             			0x37
#define PKT_OUTITEM             			0x38
#define PKT_MONEYCHANGE						0x39
#define PKT_WEIGHT							0x3A
#define PKT_ITEM_USE						0x3C
#define PKT_ITEM_CHANGE_INFO				0x3D
#define PKT_ITEM_DUMP						0x3E

#define PKT_NEW_HERO            			0x40
#define PKT_DEL_HERO            			0x41

#define PKT_CHARTER_DATA        			0x50

#define PKT_CLIENTEVENT         			0x60
#define BBS_OPEN							0x65
#define BBS_NEXT							0x66
#define BBS_READ							0x67
#define BBS_WRITE							0x68
#define BBS_EDIT							0x69
#define BBS_DELETE							0x6A

#define PKT_SHOPOPEN            			0x70
#define PKT_SHOP_ITEM_COUNT					0x71
#define PKT_SELL_SHOP           			0x72
#define PKT_BUY_SHOP            			0x73
#define PKT_MAKEROPEN						0x74
#define PKT_MAKEITEM						0x75
#define PKT_STORAGEOPEN						0x76
#define PKT_SAVEITEM						0x77
#define PKT_TAKEBACKITEM					0x78
#define PKT_SAVEMONEY						0x79
#define PKT_TAKEBACKMONEY					0x7A
#define PKT_TRADE_REQ						0x7B
#define PKT_TRADE_ACK						0x7C
#define PKT_TRADE_ADDITEM					0x7D
#define PKT_TRADE_SETMONEY					0x7E
#define PKT_TRADE_OK						0x7F

#define PKT_TRADE_CANCEL					0x80
#define PKT_TRADE_RESULT					0x81
#define PKT_AUCTION_SELL_OPEN				0x82
#define PKT_AUCTION_SELL_SEND				0x83
#define PKT_AUCTION_SELL_RCV				0x84
#define PKT_AUCTION_BUY_OPEN				0x85
#define PKT_AUCTION_BUY_SEND				0x86
#define PKT_PUTITEM_BELT					0x87
#define PKT_PUTITEM_INV						0x88
#define PKT_CHANGEBELTINDEX					0x89

#define PKT_SELHOUSEPLACE					0x90
#define PKT_HOUSEPLACE						0x91
#define PKT_EVENTSELBOX         			0x92
#define PKT_EVENTOKBOX          			0x93
#define PKT_EVENTNORMAL         			0x94
#define	PKT_GUILD_NEW						0x95
#define PKT_HPMP_RECOVERY       			0x96
#define PKT_GUILD_OPEN						0x97
#define PKT_GUILD_LIST						0x98
#define PKT_REPAIR_OPEN						0x99
#define PKT_GUILD_REQ						0x9A
#define PKT_GUILD_MOVE_REQ					0x9B
#define PKT_GUILD_REMOVE_REQ				0x9C
#define PKT_GUILD_DISSOLVE					0x9D
#define PKT_GUILD_REMOVE_USER				0x9E
#define PKT_GUILD_INFO						0x9F

#define PKT_REPAIR_REQ						0xA0
#define PKT_GUILD_REQ_USER_INFO				0xA1
#define PKT_GUILD_MAN_LIST					0xA2
#define PKT_GUILD_MOVE_REJECT				0xA3
#define PKT_GUILD_APPLICATION_LIST			0xA4
#define PKT_CHANGE_MEMBER					0xA5
#define PKT_CHANGE_GUILD_POS				0xA6
#define PKT_CHANGEDIR						0xA7
#define PKT_TOWNPORTALREQ					0xA8
#define PKT_TOWNPORTALEND					0xA9
#define PKT_EFFECT							0xAA
#define PKT_CHANGE_STORAGEIDX				0xAB
#define PKT_UNABLE_DIRITEM					0xAC
#define PKT_GUILD_EDIT_INFO_REQ				0xAE
#define PKT_SETGRAY							0xAD
#define PKT_GUILD_EDIT_INFO					0xAF

#define PKT_GUILD_SYMBOL_CHANGE_REQ			0xB0
#define PKT_GUILD_SYMBOL_CHANGE				0xB1
#define PKT_GUILDWAR_REQ					0xB2
#define PKT_GUILDWAR_REQ_RESULT				0xB3
#define PKT_GUILDWAR_REQ_CANCEL				0xB4
#define PKT_GUILDWAR_OK						0xB5
#define PKT_GUILD_STORAGEOPEN				0xB6
#define PKT_GUILD_SAVEITEM					0xB7
#define PKT_GUILD_TAKEBACKITEM				0xB8
#define PKT_GUILD_STORAGECLOSE				0xB9
#define PKT_GUILD_STORAGE_LOG				0xBA
#define PKT_GUILDWAR_ITEM					0xBB
#define PKT_GUILDWAR_MONEY					0xBC
#define PKT_GIVE_DONATION					0xBD
#define PKT_ATTACK_SPECIALARROW 			0xBE
#define PKT_ATTACK_SPECIALRAIL				0xBF

#define PKT_ATTACK_SPECIALCIRCLE 			0xC0
#define PKT_ATTACK_SPECIALCIRCLE2 			0xC1
#define PKT_STORAGELOG_NEXT					0xC2
#define PKT_SPECIALATTACKCANCEL 			0xC3
#define PKT_CONTRIBUTION					0xC5
#define PKT_CONTRIBUTION_STATUS 			0xC6
#define PKT_DUMMY_ATTACK					0xC7
#define PKT_AUCTION_MESSAGE					0xC8
#define PKT_LOTTERY							0xC9
#define PKT_AREASTATE						0xCA
#define PKT_PARTY_DENY						0xCC
#define PKT_PARTY_INVITE					0xCD
#define PKT_PARTY_INVITE_RESULT				0xCE
#define PKT_LIVEBROADCAST					0xCF

#define PKT_GUILD_STORAGE_LIMIT_CHANGE		0xD0
#define PKT_RESURRECTION_SCROLL				0xD1
#define PKT_HAIRSHOPOK						0xD2
#define PKT_GUILD_SYMBOL_DATA				0xD3
#define PKT_SKILL_ABILITY					0xD4
#define PKT_SPECIALMAKESKILLOTHER 			0xD5
#define PKT_SPECIAL_MOVE					0xD6
#define PKT_GUILDTOWN_TAXRATE				0xD7
#define PKT_GUILDTOWN_STORE					0xD8
#define PKT_GUILDTOWNSTONE_OPEN 			0xD9
#define PKT_SETRUNMODE						0xDA
#define PKT_TRADEREXCHANGE					0xDB
#define PKT_MAGICPRECAST					0xDC
#define PKT_GUILD_ALLY						0xDD
#define PKT_SPEEDHACK						0xDE
#define PKT_QUEST_VIEW						0xDF

#define PKT_MONSTER_SELECT					0xE0
#define PKT_CHANGE_OTHER_ITEM				0xE1
#define PKT_MONSTER_ZONE					0xE2
#define PKT_CHANGE_HAIR						0xE3
#define PKT_CLASSPOINTBUY					0xE5
#define PKT_PARTY_LOCATION					0xE7

#define PKT_CHECKSERVERALIVE				0xFB
#define PKT_VERSION							0xFC
#define PKT_ENCRYPTION_START_RESULT 		0xFD
#define PKT_ACOUNT_LOGIN        			0xFE

#endif

/*
0x6f PKT_SAFETYZONE - i think :S
0x71 something to do with buying an item :S
0xc4 PKT_CONTRIBUTION_OPEN
0xc8 PKT_PARTY - some party packet hmm
0xc9 PKT_LOTTERY
0xca PKT_AREASTATE
0xcb PKT_ALLCHAT_STATUS
0xd1 exists but dont know what it is
0xe6 PKT_DELETE_OTHER_CLASS
-----------------------------
0x08 UNKNOWN / PRETTY SURE ITS RESTED EXP!!
0x0e UNKNOWN / PRETTY SURE ITS MOB HP!!
0xDE SPEEDHACK/ maybe rested exp??
*/