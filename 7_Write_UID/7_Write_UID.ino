#include <MFRC522.h>
#define RST_PIN    9     // Reset腳
#define SS_PIN     10     // 晶片選擇腳
MFRC522 mfrc522(SS_PIN, RST_PIN);    // 建立MFRC522物件
MFRC522::MIFARE_Key key;  // 儲存金鑰
MFRC522::StatusCode status;

//----------function宣告------------
void writeBlock(byte , byte , byte* );
void readBlock(byte , byte , byte* );
void printBlock(byte*);
byte buffer[18];

//----------初始變數宣告------------
byte sector = 0;   // 指定讀寫的「區」，可能值:0~15
byte block = 0;     // 指定讀寫的「塊」，可能值:0~3
byte yourUID[4] = {0x0A,0xFA,0xFA,0x1A};
byte verifyUID = yourUID[0]^yourUID[1]^yourUID[2]^yourUID[3];
byte blockData[16] = {yourUID[0],yourUID[1],yourUID[2],yourUID[3],verifyUID, 0x8, 0x4, 0x0,0x62, 0x63, 0x64, 0x65,0x66, 0x67, 0x68, 0x69};

//----------setup------------
void setup() {
  Serial.begin(9600);
  SPI.begin();               // 初始化SPI介面
  mfrc522.PCD_Init();        // 初始化MFRC522卡片
  Serial.println(F("Please scan MIFARE Classic card..."));
  // 準備金鑰（用於key A和key B），出廠預設為6組 0xFF。
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

//----------loop------------
void loop() {
  // 查看是否感應到卡片
  if ( mfrc522.PICC_IsNewCardPresent() == 0) {
    return; // 退回loop迴圈的開頭
  }
  // 選取一張卡片
  if (mfrc522.PICC_ReadCardSerial() == 0) {
    return;  // 若傳回1，代表已讀取到卡片的ID
  }

  writeBlock(sector, block, blockData);  // 區段編號、區塊編號、包含寫入資料的陣列
  readBlock(sector, block, buffer);      // 區段編號、區塊編號、存放讀取資料的陣列
  printBlock(buffer);                    // 輸出儲存的資料

  // 令卡片進入停止狀態
  mfrc522.PICC_HaltA();
}



//--------------------------------------------------------
//---------------------function---------------------------
//--------------------------------------------------------
void writeBlock(byte _sector, byte _block, byte _blockData[]) {
  if (_sector < 0 || _sector > 15 || _block < 0 || _block > 3) {
    // 顯示「區段或區塊碼錯誤」，然後結束函式。
    Serial.println(F("Wrong sector or block number."));
    return;
  }
  byte blockNum = _sector * 4 + _block;  // 計算區塊的實際編號（0~63）
  byte trailerBlock = _sector * 4 + 3;   // 控制區塊編號

  // 驗證金鑰
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  // 若未通過驗證…
  if (status != MFRC522::STATUS_OK) {
    // 顯示錯誤訊息
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 在指定區塊寫入16位元組資料
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockNum, _blockData, 16);
  // 若寫入不成功…
  if (status != MFRC522::STATUS_OK) {
    // 顯示錯誤訊息
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 顯示「寫入成功！」
  Serial.println(F("Data was written."));
}
void readBlock(byte _sector, byte _block, byte _blockData[])  {
  if (_sector < 0 || _sector > 15 || _block < 0 || _block > 3) {
    // 顯示「區段或區塊碼錯誤」，然後結束函式。
    Serial.println(F("Wrong sector or block number."));
    return;
  }

  byte blockNum = _sector * 4 + _block;  // 計算區塊的實際編號（0~63）
  byte trailerBlock = _sector * 4 + 3;   // 控制區塊編號

  // 驗證金鑰
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  // 若未通過驗證…
  if (status != MFRC522::STATUS_OK) {
    // 顯示錯誤訊息
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  byte buffersize = 18;
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockNum, _blockData, &buffersize);

  // 若讀取不成功…
  if (status != MFRC522::STATUS_OK) {
    // 顯示錯誤訊息
    Serial.print(F("MIFARE_read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // 顯示「讀取成功！」
  Serial.println(F("Data was read."));
}

void printBlock(byte buffer[]) {
  Serial.print(F("Read block: "));        // 顯示儲存讀取資料的陣列元素值
  for (byte i = 0 ; i < 16 ; i++) {
    Serial.write (buffer[i]);
  }
  Serial.println();
}
