//DWORD serialNumber = -1;
//if (::GetVolumeInformation(LPCTSTR("C:\\"), NULL, NULL, &serialNumber, NULL, NULL, NULL, NULL)) {

//	//printf_s("\n\t[%d]", serialNumber);
//	//printf_s("\n\t[0x%X]", ((serialNumber * -1) * 0x0D8E ^ 0xFF + 31337));
//	//Sleep(30000);
//	for (auto xt = Subscribers.begin(); xt != Subscribers.end(); xt++) {

//		//printf_s("\n\tYear [%d] Month [%d] Day [%d] Serial [0x%X] User [%s]", xt->Expiration.Year, xt->Expiration.Month, xt->Expiration.Day, xt->SerialNumber, xt->Person);

//		// First check the serial.
//		if (xt->SerialNumber == ((serialNumber * -1) * 0x0D8E ^ 0xFF + 31337)) {
//			//printf_s("\n\nSerial is valid.");

//			// Then check the expiration.
//			if (CTime::GetTickCount().GetYear() <= xt->Expiration.Year) {
//				if (CTime::GetTickCount().GetMonth() <= xt->Expiration.Month) {
//					if (CTime::GetTickCount().GetMonth() == xt->Expiration.Month) {
//						if (CTime::GetTickCount().GetDay() < xt->Expiration.Day) {
//							//printf_s("\n\nProgram has not yet expired. Days left [%d]\n\n", (xt->Expiration.Day - CTime::GetTickCount().GetDay()));
//							Unlocked = true; return true;
//						}
//					} else {
//						//printf_s("\n\nProgram has not yet expired. Days left [%d]\n\n", (30 + xt->Expiration.Day));
//						Unlocked = true; return true;
//					}
//				}
//			}
//			//printf_s("\n\nProgram has now expired."); break;
//		}
//	}
//} 
//return false;