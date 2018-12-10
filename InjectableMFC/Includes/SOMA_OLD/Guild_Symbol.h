// Guild_Symbol.h: interface for the CGuildSymbol class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GUILD_SYMBOL_H_
#define GUILD_SYMBOL_H_

#define GUILDSYMBOL_BMPSIZE 224 // 448 / 2

class CDDSurface;

struct GUILDSYMBOL
{
	WORD m_GuildSymbolBmp[GUILDSYMBOL_BMPSIZE];
	int	 m_nGuildPicId;
	BOOL m_bLoad;	
};

typedef CMap<int, int, GUILDSYMBOL*, GUILDSYMBOL*> GUILDSYMBOLS;

class CGuild_Symbol
{
public:
	CGuild_Symbol();
	virtual ~CGuild_Symbol();

	void Draw(CDDSurface *pDest, int iGuild, short sGuildPicId, CPoint ptDrawPos);
	void RcvGuildSymbol(PBYTE bypData);

private:
	BOOL Load(int iGuild, short sGuildPicId);
	void Save(int iGuild);
	void SendReqGuildSymbol(int iGuild);

	WORD m_wColorKey;
	GUILDSYMBOLS m_GuildSymbols;
};

#endif // GUILD_SYMBOL_H_