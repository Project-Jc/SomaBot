#pragma once

#define PREPARE_USERMANAGER_CALL() __asm mov ecx, 0x006D8388;
#define PREPARE_ISMOVEABLE_CALL() __asm mov ecx, [0x6D8388+0x00001A48];
#define PREPARE_PACKETPARSER_CALL() __asm mov ecx, 0x006D8370;

typedef PUSER(WINAPI *tGetMe)(void);
typedef PUSER(WINAPI *tGetUserWithUid)(int Uid);

typedef void(WINAPI *tMoveRequest)(CUser *pUser, CPoint Destination, int FindStep);
typedef void(WINAPI *tMoveRunRequest)(CUser *pUser, CPoint Destination, int FindStep);

typedef BOOL(WINAPI *tIsMoveable)(int x, int y);

class UserList {
private:
	unordered_map<int, CUser *> List;
	int ListCount = 0;

public:
	unordered_map<int, CUser *>::iterator Begin(void) {
		return List.begin();
	}
	unordered_map<int, CUser *>::iterator End(void) {
		return List.end();
	}

	void Add(CUser *user) {
		if (!Contains(user->Data->Uid)) {
			List.emplace(user->Data->Uid, user);
			ListCount++;
		}
	}

	bool Get(int Uid, CUser *&pUser) {
		unordered_map<int, CUser *>::const_iterator iter = List.find(Uid);

		if (iter != List.end()) {
			pUser = iter->second;
			return true;
		} else {
			pUser = nullptr;
			return false;
		}
	}

	bool ContainsName(CString Name) {
		CUser *pUser;
		for (auto item = List.begin(); item != List.end(); item++) {
			pUser = item->second;
			if (pUser && pUser->Data && pUser->Data->Name.GetString() == Name) {
				return true;
			}
		} return false;
	}
	bool GetByName(CString Name, CUser *&pUser) {
		for (auto item = List.begin(); item != List.end(); item++) {
			pUser = item->second;
			if (pUser) {
				if (pUser->Data) {
					if (pUser->Data->Name.GetString() == Name) {
						return true;
					}
				}
			}
		} return false;
	}
	bool GetByName(CString Name, CUser *&pUser, CPoint Cell, int Range) {
		for (auto item = List.begin(); item != List.end(); item++) {
			pUser = item->second;

			//CString pName = pUser->Data->Name.GetString();
			//printf_s("%s [%d]\t%s [%d]\r\n", pName, pName.GetLength(), Name, Name.GetLength());
			//printf_s("%s [%d]\t%s [%d]\r\n", pUser->Data->Name, pUser->Data->Name.GetLength(), Name, Name.GetLength());

			if (pUser->Data->Name.GetString() == Name) {
				if (pUser->RangeFromCell(Cell) <= Range) {
					return true;
				}
			}
		} return false;
	}

	bool Contains(int Uid) {
		unordered_map<int, CUser *>::const_iterator xt = List.find(Uid);
		return xt != List.end();
	}

	void Remove(int Uid) {
		if (Contains(Uid)) {
			List.erase(Uid);
			ListCount--;
		}
	}
	void Delete(int Uid) {
		CUser *pUser = nullptr;
		if (Get(Uid, pUser)) {		
			//printf_s("[%d:%d:%d] Deleting user [Uid: %d] from the UserList\n", CTime::GetTickCount().GetHour(), CTime::GetTickCount().GetMinute(), CTime::GetTickCount().GetSecond(), Uid);
			delete pUser;
			List.erase(Uid);
			ListCount--;
		}
	}

	void ClearAll() {
		for (auto item = List.begin(); item != List.end(); item++) {
			CUser *pUser = item->second;
			delete item->second;
		}
		List.clear();
		ListCount = 0;
	}
	void Clear() {
		for (auto item = List.begin(); item != List.end(); item++) {
			CUser *pUser = item->second;
			// Do not delete me.
			if (!pUser->Data->IsMe) {
				List.erase(item);
				delete item->second;
				ListCount--;
			}
		}
	}

	int Count(void) {
		return ListCount;
	}
};

class UserManager {
public:

	static tIsMoveable fIsMoveable;

	static tMoveRequest fMoveRequest;

	static tMoveRunRequest fMoveRunRequest;

	static tGetMe fGetMe;

	static tGetUserWithUid fGetUserWithUid;

	static PUSER GetMe(void) {
		PREPARE_USERMANAGER_CALL();
		return fGetMe();
	}

	static PUSER GetUserWithUid(int Uid) {
		PREPARE_USERMANAGER_CALL();
		return fGetUserWithUid(Uid);
	}

	static UserList UserList;

	static Direction GetDirection(CPoint FromCell, CPoint ToCell);

	static int GetCellRange(CPoint StartCell, CPoint FinalCell);

	static void MoveRequest(CPoint Destination);

	static void MoveRunRequest(CPoint Destination);

	static BOOL IsMoveable(CPoint Point) {
		int x = Point.x, y = Point.y;
		PREPARE_ISMOVEABLE_CALL();
		return fIsMoveable(x, y);
	}
};