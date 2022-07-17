#include "CGridShell.h"
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
CGridShell::CGridShell()
{
  m_strUsername = "";
  m_uiLastHB    = 0;
  m_uiLastReconnection = 0;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
CGridShell& CGridShell::GetInstance()
{
  static CGridShell sInstance;

  return sInstance;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
bool CGridShell::Init(const String& strUsername)
{
  GDEBUG("Lib start");

  // Validate username length
  if (strUsername.length() == 40)
    m_strUsername = strUsername;
  else
  {
    GDEBUG("Username is wrong");
    return false;
  }
  // Obtain MAC for ident purposes
  m_strMACAddress =  WiFi.macAddress();
  m_strMACAddress.replace(":", "");

  GDEBUG("Init completed");

  //
  return true;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Keep alive with server
//
// -----------------------------------------------------------------------------
void CGridShell::Pong()
{
  // Keep Alive
  if (m_Client.connected())
  {
    if (millis() - m_uiLastHB >= GMINER_PING_TIME)
    {
      Send("PONG\r\n");
      m_uiLastHB = millis();
    }
  }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
bool CGridShell::Connected()
{
  return m_Client.connected();
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
void CGridShell::Tick()
{
  // Keep alive
  Pong();

  // Are we up?
  if (m_Client.connected() == false)
  {
    m_Client.stop();
    m_Client.flush();

    // Check if user set and reconnection timer is expired
    if (m_strUsername.length() == 40 && (millis() - m_uiLastReconnection > GMINER_RECON_TIMER))
    {
      GDEBUG("Reconnecting to server");

      // Connect
      if (m_Client.connect(GMINER_SERVER, GMINER_POOL_PORT))
      {
        // Ident and provide payload if any, this is base64 encoded already by ::Init
        String strServerWelcome = m_Client.readStringUntil('\n');

        // ****************************
        // Diffie-Hellman Key Exchange
        // ****************************

        // Get P,G and Server public key for our miner
        String strServerP = m_Client.readStringUntil('\n');
        String strServerG = m_Client.readStringUntil('\n');
        String strServerPublicKey = m_Client.readStringUntil('\n');
        GDEBUG("Got P=" + strServerP + " G=" + strServerG + " ServPubKey=" + strServerPublicKey);

        //
        uint64_t uiServerP          = strServerP.toInt();
        uint64_t uiServerG          = strServerG.toInt();
        uint64_t uiServerPublicKey  = strServerPublicKey.toInt();

        // Calculate our private key
        uint64_t uiDHPrivateKey = esp_random();

        // Calculate our public key
        uint64_t uiDHPublicKey = power(uiServerG, uiDHPrivateKey, uiServerP);

        // Compute symmetric (secret) key
        uint64_t uiKey = power(uiServerPublicKey, uiDHPrivateKey, uiServerP);

        // ****************************
        // SHA1
        // ****************************

        char cTemp[40];
        sprintf(cTemp, "%llu", uiKey);
        String sha1HashKey = sha1HW(cTemp);
        
        // ****************************
        // XOR
        // ****************************
        
        String strCipher = XOR(m_strUsername, sha1HashKey); 

        // ****************************
        // BASE64ENCODE
        // ****************************

        String strBase64EncodedGUID = EncodeBase64(strCipher); 
        
        // Pass my Public Key and GUID encoded
        Send("JOB," + String(std::to_string(uiDHPublicKey).c_str()) + "," + strBase64EncodedGUID + "," + GMINER_VERSION + "," + m_strMACAddress + "\r\n");
      }

      //
      m_uiLastReconnection = millis();
    }
  }
  else
  {
    // Wait for server to post a message
    if (m_Client.available())
    {

      //
      String strJobType = m_Client.readStringUntil(',');

      //
      GDEBUG("Server request " + strJobType);

      // Task coming
      if (strJobType == "EXEC")
      {
        //
        int iRetCode = MB_FUNC_ERR;
        String strOutput = "";

        //
        String strScript  = DecodeBase64(m_Client.readStringUntil(','));
        String strPayload = DecodeBase64(m_Client.readStringUntil(','));

        GDEBUG("Script Starting");

        uint32_t uiStart = millis();
        void **l = NULL;
        struct mb_interpreter_t* bas = NULL;

        // Initialize MYBASIC
        mb_init();
        mb_open(&bas);

        // Enable step by step execution to keep alive with the server
        mb_debug_set_stepped_handler(bas, CGridShell::MBStep);

        // Load up the script
        if (mb_load_string(bas, strScript.c_str(), true) == MB_FUNC_OK)
        {
          // payload check and upload
          if (strPayload != "")
          {
            mb_value_t valAdd;
            valAdd.type = MB_DT_STRING;
            valAdd.value.string =  (char *)strPayload.c_str();
            mb_add_var(bas, l, "INPUTPAYLOAD", valAdd, true);
          }

          // Run
          iRetCode = mb_run(bas, true);

          // Obtain output
          mb_value_t valGet;
          int iRes = mb_get_value_by_name(bas, l, "OUTPUTPAYLOAD", &valGet);

          // Needs to be a string
          if (valGet.type == MB_DT_STRING)
            strOutput = String(valGet.value.string);
        }

        GDEBUG("Script completed in " + String(millis() - uiStart) + " ms, RESCODE: " + String(iRetCode) + ", MEM: " + String(ESP.getFreeHeap()) + " OUTP: '" + strOutput + "'");

        Send("RESULTS," + String(iRetCode) + "," + EncodeBase64(strOutput) + "\r\n");

        mb_close(&bas);
        mb_dispose();
      }
    }
  }
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
void CGridShell::Send(const String& strData)
{
  m_Client.write( strData.c_str(),  strData.length() );
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// ---------------------------------------------------------------------------
String CGridShell::EncodeBase64(const String& strString)
{
  if (strString.length() > 0)
  {
    String strTemp = strString;
    size_t stLen = 0;
    mbedtls_base64_encode(NULL, 0, &stLen, (unsigned char*)strTemp.c_str(), strTemp.length());

    unsigned char ucEncoded[stLen];
    mbedtls_base64_encode(ucEncoded, stLen, &stLen, (unsigned char*)strTemp.c_str(), strTemp.length());
    return String((char*)ucEncoded);
  }
  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// ---------------------------------------------------------------------------
String CGridShell::DecodeBase64(const String& strString)
{
  if (strString.length() > 0)
  {
    String strTemp = strString;
    size_t stLen = 0;
    mbedtls_base64_decode( NULL, 0, &stLen, (unsigned char*)strTemp.c_str(), strTemp.length());

    unsigned char ucDecoded[stLen];
    mbedtls_base64_decode(ucDecoded, stLen, &stLen, (unsigned char*)strTemp.c_str(), strTemp.length());
    ucDecoded[stLen] = '\0';
    return String((char*)ucDecoded);
  }
  return "";
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : Handling BAS execution and keeping alive with the server, also checks for task endless loop
//
// ---------------------------------------------------------------------------
int CGridShell::MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col)
{

  //
  static uint32_t uiTTL = 0;

  // Start of the program
  // https://github.com/paladin-t/my_basic/issues/56
  if (row == 2)
    uiTTL = millis();

  // Avoid endless loops
  if (millis() - uiTTL > GMINER_EXEC_TIMEOUT)
    return MB_FUNC_BYE;

  // Breath
  yield();
  CGridShell::GetInstance().Pong();

  //
  return MB_FUNC_OK;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA1
//
// -----------------------------------------------------------------------------
String CGridShell::sha1HW(String payload)
{
  return sha1HW((unsigned char *)payload.c_str(), payload.length());
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : HW SHA1
//
// -----------------------------------------------------------------------------
String CGridShell::sha1HW(unsigned char *payload, int len)
{
  //
  int size = 20;
  byte shaResult[size];

  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;

  const size_t payloadLength = len;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  String hashStr = "";

  for (uint16_t i = 0; i < size; i++) {
    String hex = String(shaResult[i], HEX);
    if (hex.length() < 2) {
      hex = "0" + hex;
    }
    hashStr += hex;
  }

  return hashStr;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
uint64_t CGridShell::power(uint64_t a, uint64_t b, uint64_t mod)
{
  uint64_t t;
  if (b == 1)
    return a;
  t = power(a, b / 2, mod);
  if (b % 2 == 0)
    return (t * t) % mod;
  else
    return (((t * t) % mod) * a) % mod;
} 
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
String CGridShell::XOR(const String& toEncrypt, const String& rstrKey)
{
  String output = toEncrypt;

  for (int i = 0; i < toEncrypt.length(); i++)
    output[i] = toEncrypt[i] ^ rstrKey[i % (rstrKey.length() / 1)];

  return output;
}
// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   :
//
// -----------------------------------------------------------------------------
CGridShell::~CGridShell()
{

}
