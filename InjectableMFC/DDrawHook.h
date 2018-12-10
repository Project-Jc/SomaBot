#pragma once

#define VTABLE(x) DWORD *vTable = (DWORD *) ((DWORD *) *x)[0];

namespace VTable_IDirectDraw7 {
	enum VTable_IDirectDraw7 {
		/*** IUnknown methods ***/
		QueryInterface,
		AddRef,
		Release,

		/*** IDirectDraw methods ***/
		Compact,
		CreateClipper,
		CreatePalette,
		CreateSurface,
		DuplicateSurface,
		EnumDisplayBehaviours,
		EnumSurfaces,
		FlipToGDISurface,
		GetCaps,
		GetDisplayBehaviour,
		GetFourCCCodes,
		GetGDISurface,
		GetMonitorFrequency,
		GetScanLine,
		GetVerticalBlankStatus,
		Initialize,
		RestoreDisplayBehaviour,
		SetCooperativeLevel,
		SetDisplayBehaviour,
		WaitForVerticalBlank,

		/*** Added in the v2 interface ***/
		GetAvailableVidMem,

		/*** Added in the v4 interface ***/
		GetSurfaceFromDC,
		RestoreAllSurfaces,
		TestCooperativeLevel,
		GetDeviceIdentifier,
		StartBehaviourTest,
		EvaluateBehaviour
	};
}
namespace VTable_IDirectDrawSurface7 {
	enum VTable_IDirectDrawSurface7 {
		/*** IUnknown methods ***/
		QueryInterface,
		AddRef,
		Release,

		// IDirectDrawSurface7
		AddAttachedSurface,
		AddOverlayDirtyRect,
		Blt,
		BltBatch,
		BltFast,
		DeleteAttachedSurface,
		EnumAttachedSurfaces,
		EnumOverlayZOrders,
		Flip,
		GetAttachedSurface,
		GetBltStatus,
		GetCaps,
		GetClipper,
		GetColorKey,
		GetDC,
		GetFlipStatus,
		GetOverlayPosition,
		GetPalette,
		GetPixelFormat,
		GetSurfaceDesc,
		Initialize,
		IsLost,
		Lock,
		ReleaseDC,
		Restore,
		SetClipper,
		SetColorKey,
		SetOverlayPosition,
		SetPalette,
		Unlock,
		UpdateOverlay,
		UpdateOverlayDisplay,
		UpdateOverlayZOrder,

		/*** Added in the v2 interface ***/
		GetDDInterface,
		PageLock,
		PageUnlock,

		/*** Added in the v3 interface ***/
		SetSurfaceDesc,

		/*** Added in the v4 interface ***/
		SetPrivateData,
		GetPrivateData,
		FreePrivateData,
		GetUniquenessValue,
		ChangeUniquenessValue,

		/*** Moved Texture7 methods here ***/
		SetPriority,
		GetPriority,
		SetLOD,
		GetLOD
	};
}

typedef HRESULT(WINAPI *tDDraw_DirectDrawCreateEx)(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown FAR *pUnkOuter);
typedef HRESULT(WINAPI *tDDraw_CreateSurface)(LPDIRECTDRAW7 FAR *pDevice, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 FAR *lplpDDSurface, IUnknown FAR *pUnkOuter);
typedef HRESULT(WINAPI *tDDraw_Lock)(IDirectDrawSurface7 FAR *pSurface, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent);

enum DmgType : BYTE {
	HEAL,
	ATTACK
};

struct DmgInfo {

	int Amount;
	DWORD Time;
	DmgType Type;
	bool DrawForMe;

	DmgInfo(int amount, DWORD time, DmgType type, bool me) :
		Amount(amount), Time(time), Type(type), DrawForMe(me) {}
};

class DDrawHook {
private:
	// The total number of created surfaces (# of times IDirectDraw7::CreateSurface was called).
	static int SurfaceCount;

	// The surface that we want to draw on.
	static IDirectDrawSurface7 FAR *HookedSurface;

	static struct Screen {
		int Width;
		int Height;
	} Screen;

public:
	static bool Init(void);
	static void Exit(void);

	struct CC {
		static CodeCave DirectDrawCreateEx;

		struct IDD7 {
			static CodeCave CreateSurface;
		};

		struct IDDSurface7 {
			static CodeCave Lock;
		};
	};

	struct MY {
		static HRESULT WINAPI DirectDrawCreateEx(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown FAR *pUnkOuter);

		struct IDD7 {
			static HRESULT WINAPI CreateSurface(LPDIRECTDRAW7 FAR *pDevice, LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 FAR *lplpDDSurface, IUnknown FAR *pUnkOuter);
		};

		struct IDDSurface7 {
			static HRESULT WINAPI Lock(IDirectDrawSurface7 FAR *pSurface, LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent);
		};
	};

	static struct BotUserDmgData {

		vector<DmgInfo> FloatingDamageList,
						StaticDamageList;

		vector<DmgInfo>::const_iterator FloatingDmgBegin(void) {
			return FloatingDamageList.begin();
		}
		vector<DmgInfo>::const_iterator FloatingDmgEnd(void) {
			return FloatingDamageList.end();
		}
		void FloatingDmgAdd(DmgInfo NewDmgInfo) {
			FloatingDamageList.push_back(NewDmgInfo);
		}
		vector<DmgInfo>::const_iterator FloatingDmgRemove(vector<DmgInfo>::const_iterator pos) {
			return FloatingDamageList.erase(pos);
		}

		vector<DmgInfo>::const_iterator StaticDmgBegin(void) {
			return StaticDamageList.begin();
		}
		vector<DmgInfo>::const_iterator StaticDmgEnd(void) {
			return StaticDamageList.end();
		}
		void StaticDmgAdd(DmgInfo NewDmgInfo) {
			//if (StaticDamageList.size() == 6) {
			//	StaticDamageList.erase(StaticDamageList.begin());
			//}
			StaticDamageList.push_back(NewDmgInfo);
		}
		vector<DmgInfo>::const_iterator StaticDmgRemove(vector<DmgInfo>::const_iterator pos) {
			return StaticDamageList.erase(pos);
		}

	} BotUserDmg;

	static void MyDrawBitMap(HDC hDC, byte bmp, int x, int y) {
		HBITMAP hBitMap = LoadBitmap(GetModuleHandle(szDLLName), MAKEINTRESOURCE(bmp));

		BITMAP bMp;
		GetObject(hBitMap, sizeof(BITMAP), &bMp);

		HDC hDCBmp = CreateCompatibleDC(hDC);
		SelectObject(hDCBmp, hBitMap);

		//BitBlt(hDC, x, y, bMp.bmWidth, bMp.bmHeight, hDCBmp, 0, 0, SRCCOPY);
		TransparentBlt(hDC, x, y, bMp.bmWidth, bMp.bmHeight, hDCBmp, 0, 0, bMp.bmWidth, bMp.bmHeight, RGB(248, 0, 248));

		DeleteDC(hDCBmp);
		DeleteObject(hBitMap);
	}

	static void MyDrawText(HDC hDC, COLORREF color, UINT align, int x, int y, LPCSTR format, ...) {
		char buffer[1024];

		va_list argPtr;

		va_start(argPtr, format);
		wvsprintf(buffer, format, argPtr);
		va_end(argPtr);

		COLORREF oldColor = SetTextColor(hDC, color);

		UINT previousAlign = SetTextAlign(hDC, align);

		TextOut(hDC, x, y, buffer, strlen(buffer));

		SetTextAlign(hDC, previousAlign);
		SetTextColor(hDC, oldColor);
	}

	static void MyDrawTrackTarget(HDC hDC, int x, int y, CUser* pUser) {

		// Draw the name.
		MyDrawTextWithCustomFont(hDC, RGB(0, 0, 0), TA_LEFT, 
			CreateFont(23, 0, 0, 0,
			FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
			CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial")), x + 1, y + 1, pUser->Data->Name.GetString());

		MyDrawTextWithCustomFont(hDC, RGB(255, 150, 0), TA_LEFT, 
			CreateFont(23, 0, 0, 0,
			FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
			CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial")), x, y, pUser->Data->Name.GetString());

		x += 30;
		y += 25;

		// Draw the HP.
		MyDrawTextWithCustomFont(hDC, RGB(0, 0, 0), TA_LEFT,
			CreateFont(20, 0, 0, 0,
			FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
			CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial")), x + 1, y + 1, "%d / %d", pUser->Data->Health.Current, pUser->Data->Health.Maximum);

		MyDrawTextWithCustomFont(hDC, RGB(230, 140, 0), TA_LEFT,
			CreateFont(20, 0, 0, 0,
			FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
			CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial")), x, y, "%d / %d", pUser->Data->Health.Current, pUser->Data->Health.Maximum);
	}

	static void MyDrawTextWithCustomFont(HDC hDC, COLORREF color, UINT align, HFONT hFont, int x, int y, LPCSTR format, ...) {
		char buffer[1024];

		va_list argPtr;

		va_start(argPtr, format);
		wvsprintf(buffer, format, argPtr);
		va_end(argPtr);

		COLORREF oldColor = SetTextColor(hDC, color);
		HFONT oldHFont = (HFONT)SelectObject(hDC, hFont);
		UINT previousAlign = SetTextAlign(hDC, align);

		TextOut(hDC, x, y, buffer, strlen(buffer));

		DeleteObject(SelectObject(hDC, oldHFont));
		SetTextAlign(hDC, previousAlign);
		SetTextColor(hDC, oldColor);
	}

	static void MyDrawTextWithFont(HDC hDC, COLORREF color, UINT align, int x, int y, LPCSTR format, ...) {
		char buffer[1024];

		va_list argPtr;

		va_start(argPtr, format);
		wvsprintf(buffer, format, argPtr);
		va_end(argPtr);

		HFONT hFont = CreateFont(17, 0, 0, 0,
						700, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
						CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Franklin Gothic Medium"));

		COLORREF oldColor = SetTextColor(hDC, color);
		HFONT oldHFont = (HFONT)SelectObject(hDC, hFont);
		UINT previousAlign = SetTextAlign(hDC, align);
      
		TextOut(hDC, x, y, buffer, strlen(buffer));

		DeleteObject(SelectObject(hDC, oldHFont));
		SetTextAlign(hDC, previousAlign);
		SetTextColor(hDC, oldColor);
	}

	static void MyDrawBotUserDmg(HDC hDC, COLORREF color, UINT align, int x, int y, LPCSTR format, ...) {

		//pDC->SetBkMode(TRANSPARENT);
		//pFont.CreateFont(-16, 0, 0, 0, 
		//	FW_BOLD, FALSE, FALSE, FALSE,
		//	DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, FONTNAME);

		char buffer[1024];

		va_list argPtr;

		va_start(argPtr, format);
		wvsprintf(buffer, format, argPtr);
		va_end(argPtr);

		HFONT hFont = CreateFont(25, 0, 0, 0,
			FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
			CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"));

		COLORREF oldColor = SetTextColor(hDC, color);
		HFONT oldHFont = (HFONT)SelectObject(hDC, hFont);
		UINT previousAlign = SetTextAlign(hDC, align);

		TextOut(hDC, x, y, buffer, strlen(buffer));

		DeleteObject(SelectObject(hDC, oldHFont));
		SetTextAlign(hDC, previousAlign);
		SetTextColor(hDC, oldColor);
	}

	static void MyDrawRect(HDC hDC, COLORREF color, int x, int y, int width, int height) {
		HBRUSH hBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

		Rectangle(hDC, x, y, x + width, y + height);

		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}

	static void MyDrawX(HDC hDC, int fromx, int fromy, int tox, int toy) {
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

		MoveToEx(hDC, fromx, fromy, NULL);
		LineTo(hDC, tox, toy);

		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
	}

	static void MyDrawLine(HDC hDC, int fromx, int fromy, int tox, int toy) {
		HPEN hPen = CreatePen(PS_DASHDOTDOT, 2, RGB(248, 248, 248));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

		MoveToEx(hDC, fromx, fromy, NULL);
		LineTo(hDC, tox, toy);

		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
	}

	static void MyDrawFillRect(HDC hDC, COLORREF color, int x, int y, int width, int height) {
		HBRUSH hBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);

		RECT rect = { x, y, x + width, y + height };

		FillRect(hDC, &rect, hBrush);

		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}

	static void MyDrawFillRectWithBorder(HDC hDC, COLORREF bcolor, COLORREF color, int bx, int by, int bwidth, int bheight, Border bBorder) {
		MyDrawRect(hDC, bcolor, bx, by, (bwidth + (bBorder.Right + bBorder.Left)), bheight);
		MyDrawFillRect(hDC, color, (bx + bBorder.Left), (by + bBorder.Top), bwidth, (bheight - (bBorder.Bottom + bBorder.Top)));
	}

	static void MyDrawGroupBox(HDC hDC, COLORREF border, COLORREF color, CString text, int x, int y, int width, int height, Border bBorder) {

		// Draw the box.
		MyDrawFillRectWithBorder(hDC, border, color, x, y, width, height, bBorder);

		// Get the width from the length of the text.
		width = (text.GetLength() * 9);

		// Remove a portion from the top of the box for the text.
		MyDrawFillRect(hDC, color, (x + 20), y, width, bBorder.Top);

		// Draw the text.
		MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_CENTER, (x + 20 + (width / 2)), (y - 6), text);
	}

	static void MyDrawCircle(HDC hDC, COLORREF color, int x, int y, int radius) {
		HBRUSH hBrush = CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);

		Ellipse(hDC, x - radius, y - radius, x + radius, y + radius);

		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}

	static void MyDrawButton(HDC hDC, COLORREF border, COLORREF color, CString label, RECT rect) {
		MyDrawRect(hDC, border, rect.left, rect.top, (label.GetLength() * 10), 30);
		//MyDrawFillRect(hDC, color, x + 2, y + 2, 48, 48);
		//MyDrawText(hDC, RGB(0, 0, 0), x + 5, y + 5, text);
	}

	static void MyDrawButtonWithBorder(HDC hDC, COLORREF border, COLORREF color, CString label, Purpose Purpose, int x, int y, Border bBorder) {

		int width = (label.GetLength() * 11), height = 28;

		if (width == 33) {
			width += 5;
		}

		MyDrawRect(hDC, border, x, y, (width + (bBorder.Right + bBorder.Left)), height);

		MyDrawFillRect(hDC, color, (x + bBorder.Left), (y + bBorder.Top), width, (height - (bBorder.Bottom + bBorder.Top)));

		MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_CENTER, (x + bBorder.Left + (width / 2)), (y + bBorder.Top + 2), label);

		Gui::NewControl(Purpose, RECT { x, y, (x + width), (y + height) });
	}

	static void MyDrawCheckBox(HDC hDC, COLORREF color, CString label, Purpose Purpose, int x, int y) {

		//bool checked; int width = 10, height = 10;

		//// Evaluate the controls purpose.
		//switch (Purpose) {
		//	case Purpose::TOGGLE_CHATNOTIFY: {
		//		checked = Bot::Configuration.ChatNotify;
		//	} break;
		//	case Purpose::TOGGLE_CHATRESPOND: {
		//		checked = Bot::Configuration.ChatRespond;
		//	} break;
		//	case Purpose::TOGGLE_GMLIVENOTIFY: {
		//		checked = Bot::Configuration.GmLive.NotifyOnLive;
		//	} break;
		//	case Purpose::TOGGLE_AUTOSERUM: {
		//		checked = Bot::Configuration.AutoSerum;
		//	} break;
		//	case Purpose::TOGGLE_RANDOMIZATION: {
		//		checked = Bot::Configuration.Randomization;
		//	} break;
		//	case Purpose::TOGGLE_NEARBYNOTIFY: {
		//		//checked = Bot::Configuration.NearbyNotify;
		//	} break;
		//	case Purpose::TOGGLE_DAMAGEOVERHEAD: {
		//		checked = Bot::Configuration.DamageOverHead;
		//	} break;
		//	case Purpose::TOGGLE_DRAWINFO: {
		//		checked = Bot::Configuration.DrawInfo;
		//	} break;
		//	case Purpose::TOGGLE_NOLOOT: {
		//		checked = Bot::Configuration.NoLoot;
		//	} break;
		//	case Purpose::TOGGLE_NOWARP: {
		//		checked = Bot::Configuration.NoWarp;
		//	} break;
		//	case Purpose::TOGGLE_IGNOREPK: {
		//		checked = Bot::Configuration.IgnorePk;
		//	} break;
		//	case Purpose::TOGGLE_REGAINSTAM: {
		//		checked = Bot::Configuration.RegainStam;
		//	} break;
		//	case Purpose::TOGGLE_HEALENABLE: {
		//		checked = Bot::State.Heal.Enabled;
		//	} break;
		//}

		//// Draw the box either checked or unchecked.
		////MyDrawFillRectWithBorder(hDC, RGB(0, 0, 0), RGB(248, 248, 248), x, y, width, height, Border(1, 1, 1, 1));

		//if (checked) {
		//	MyDrawLine(hDC, x, (y - 1 + (height / 2)), (x - 2 + (width / 2)), (y + (height + 1)));
		//	MyDrawLine(hDC, (x - 2 + (width / 2)), (y + height), (x + width + 1), y);
		//}
		//else {
		//	MyDrawLine(hDC, x, y, (x + (width - 1)), (y + (height + 1)));
		//	MyDrawLine(hDC, (x + width), y, x, (y + (height - 1)));
		//}
		//
		//// Print the label.
		//MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_LEFT, (x + width + 7), (y - 3), label);

		//// Add the control to the control list.
		//Gui::NewControl(Purpose, RECT{ x, y, (x + width + (label.GetLength() * 9)), (y + height) });
	}

	static void MyDrawMultipleOptionBox(HDC hDC, COLORREF border, COLORREF color, CString label, Purpose Purpose, int x, int y, Border bBorder) {

		CString option;

		switch (Purpose) {
			case Purpose::SELECT_MODE: {
				option = Bot::Configuration.ModeAsCString();
			} break;
			case Purpose::SELECT_BEHAVIOUR: {
				option = Bot::Configuration.BehaviourAsCString();
			} break;
			case Purpose::SELECT_AURA: {
				if (!Bot::Configuration.Aura.IsEmpty()) {
					option = Bot::Configuration.Aura;
					if (option.IsEmpty()) {
						option = "\t\t\t\t\t\t";
					}
				}
			}
		}

		int width, height = 28;

		// Get the text length of the longest option for the box's size.
		width = (option.GetLength() * 9);

		// Draw the box itself and print the option.
		MyDrawFillRectWithBorder(hDC, border, color, x, y, width, height, bBorder);
		MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_CENTER, (x + bBorder.Left + (width / 2)), (y + bBorder.Top + 2), option);

		// Print the label.
		MyDrawTextWithFont(hDC, RGB(0, 0, 0), NULL, (x + 5), (y - 20), label);

		// Add the control to the control list.
		Gui::NewControl(Purpose, RECT{ x, y, (x + width), (y + height) });
	}

	static void MyDrawTabs(HDC hDC, COLORREF border, COLORREF color, vector<CString> tabs, int x, int y, Border bBorder) {

	}

	static void MyDrawModifiableValueBox(HDC hDC, COLORREF color, CString label, Purpose Purpose, int x, int y) {

		int width = 27, height = 20, value;

		if (Purpose == Purpose::MODIFY_VALUE_CASTREDUCTION) {
			MyDrawFillRect(hDC, color, x, y, (width + 13), height);
		}
		else {
			MyDrawFillRect(hDC, color, x, y, width, height);
		}

		switch (Purpose) {
			case Purpose::MODIFY_PERCENT_HP: {
				value = Bot::Configuration.HpThreshold;
			} break;
			case Purpose::MODIFY_PERCENT_MP: {
				value = Bot::Configuration.MpThreshold;
			} break;
			case Purpose::MODIFY_PERCENT_HEAL: {
				value = Bot::Configuration.HealThreshold;
			} break;
			case Purpose::MODIFY_PERCENT_DURA: {
				value = Bot::Configuration.DuraThreshold;
			} break;
			case Purpose::MODIFY_VALUE_AURAREUSE: {
				value = Bot::Configuration.AuraReuseTime;
			} break;
			case Purpose::MODIFY_VALUE_SEARCHRANGE: {
				value = Bot::Configuration.SearchRange;
			} break;
			case Purpose::MODIFY_VALUE_APPROACHTIMEOUT: {
				value = Bot::Configuration.ApproachTimeOut;
			} break;
			case Purpose::MODIFY_VALUE_ATTACKTIMEOUT: {
				value = Bot::Configuration.AttackTimeOut;
			} break;
			case Purpose::MODIFY_VALUE_LOOTTIMEOUT: {
				value = Bot::Configuration.LootTimeOut;
			} break;
			case Purpose::MODIFY_VALUE_CASTREDUCTION: {
				//value = Bot::Configuration.CastReduction;
			} break;
			case Purpose::MODIFY_VALUE_DEATHLIMIT: {
				value = Bot::Configuration.Death.Limit;
			} break;
		}

		// Print the value within the box.
		MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_LEFT, (x + 5), (y + 2), "%d", value);

		// Print the label.
		MyDrawTextWithFont(hDC, RGB(0, 0, 0), TA_RIGHT, (x - 7), (y + 2), label);

		Gui::NewControl(Purpose, RECT{ x, y, (x + width), (y + height) });
	}

	static void DrawMinimap(HDC hDC, int x, int y, int width) {

		// Draw the border.
		MyDrawRect(hDC, RGB(248, 248, 248), x, y, width, width);
		// Fill the box.
		MyDrawFillRect(hDC, RGB(24, 133, 255), x + 5, y + 5, width - 10, width - 10);
		
		int Radius = 5;
		// Draw each user as a circle.
		for (auto item = UserManager::UserList.Begin(); item != UserManager::UserList.End(); ++item) {

			CUser *other = item->second;
			if (BotUser::IsValid(other)) {

				int offsetX = (other->Data->CellPos.x - BotUser::Me->Data->CellPos.x) * 5;
				int offsetY = (other->Data->CellPos.y - BotUser::Me->Data->CellPos.y) * 5;
				int value = 2;
				COLORREF colour;

				if (BotUser::Me->Data == other->Data) {
					// Draw me as a green circle.
					colour = RGB(0, 255, 0);
				}  else if (other->IsPlayer()) {
					// Draw players as white circles.
					colour = RGB(0, 0, 0);
				} else if ((BotUser::Me->Target == other) || (other->Target == BotUser::Me) || other->IsAggressive()) {
					// Draw my target or attacker as a red circle.
					colour = RGB(255, 0, 0);
				}  else {
					// Draw all other users as yellow circles.
					colour = RGB(255, 255, 0);
				} 
				// Draw the circle.
				MyDrawCircle(hDC, colour, x + (width / value) + offsetX, y + (width / value) + offsetY, Radius);
				// Print the name.
				MyDrawTextWithFont(hDC, RGB(255, 255, 255), TA_LEFT, x + (width / value) + offsetX, y + (width / value) + offsetY + (Radius), other->Data->Name);
			}
		}
	}
};