// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CGridShell
//  - Prototype :
//
//  - Purpose   : GridShell Arduino Library  https://www.gridshell.net/
//
// -----------------------------------------------------------------------------
#ifndef __CLIB_GRID__
#define __CLIB_GRID__

#include <WiFi.h>
#include <HTTPClient.h>
#include "my_basic.hpp"
#include "mbedtls/base64.h" 

#define GMINER_SERVER "work.gridshell.net"
#define GMINER_DEBUG 0
#define GMINER_VERSION "01"
#define GMINER_PING_TIME 10000
#define GMINER_EXEC_TIMEOUT 60000
#define GMINER_RECON_TIMER 5000
#define GMINER_POOL_PORT 1911
#ifdef GMINER_DEBUG
#define GDEBUG Serial.println
#endif

class CGridShell
{
  public:
    static CGridShell& GetInstance();
    static int MBStep(struct mb_interpreter_t* s, void** l, const char* f, int p, unsigned short row, unsigned short col);
    bool Init(const String& strUsername);
    void Pong();
    void Tick();
    bool Connected();
    ~CGridShell();
  private:
    CGridShell();
    String XOR(const String& toEncrypt,const String& rstrKey);
    String sha1HW(String payload);
    String sha1HW(unsigned char *payload, int len);
    uint64_t power(uint64_t a, uint64_t b, uint64_t mod);
    String EncodeBase64(const String& strString);
    String DecodeBase64(const String& strString);
    void Send(const String& strData);
    String m_strUsername;
    String m_strMACAddress;
    uint32_t m_uiLastHB;
    uint32_t m_uiLastReconnection;
    
    WiFiClient m_Client;
};
#endif
