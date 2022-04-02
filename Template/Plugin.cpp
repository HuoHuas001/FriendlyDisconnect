#include "pch.h"
#include <EventAPI.h>
#include <LoggerAPI.h>
#include <MC/Level.hpp>
#include <MC/BlockInstance.hpp>
#include <MC/Block.hpp>
#include <MC/BlockSource.hpp>
#include <MC/Actor.hpp>
#include <MC/Player.hpp>
#include <MC/ItemStack.hpp>
#include <LLAPI.h>
#include <MC/NetworkIdentifier.hpp>
#include <MC/LoginPacket.hpp>
#include <MC/SharedConstants.hpp>
#include <MC/ServerPlayer.hpp>
#include <Nlohmann/json.hpp>

using json = nlohmann::json;
std::unordered_map<string, int>ProtectVersion;
typedef unsigned int uint32_t;
#define _BYTE char
#define _QWORD unitptr_t
#define _DWORD uint32_t
#define LOWORD(l) ((WORD)((DWORD_PTR)(l) & 0xffff))

using namespace std;
Logger logger("FriendlyDisconnect");

void PluginInit()
{
	logger.info("FriendlyDisconnect Loaded.");
}

string getLanguage(string name){
	try {
		json j;
		std::ifstream jfile("plugins/FriendlyDisConnect/language.json");
		if (jfile) {
			jfile >> j;
		}
		else {
			logger.error("No language.json file was detected. Please confirm whether the installation package is complete");
			return "";
		}
		return j[name].get<string>();
	}
	catch(...) {
		return "";
	}
}

string m_replace(string strSrc,
	const string& oldStr, const string& newStr, int count = -1)
{
	string strRet = strSrc;
	size_t pos = 0;
	int l_count = 0;
	if (-1 == count) // replace all
		count = strRet.size();
	while ((pos = strRet.find(oldStr, pos)) != string::npos)
	{
		strRet.replace(pos, oldStr.size(), newStr);
		if (++l_count >= count) break;
		pos += newStr.size();
	}
	return strRet;
}

THook(
	void,
	"?disconnectClient@ServerNetworkHandler@@QEAAXAEBVNetworkIdentifier@@EAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
	ServerNetworkHandler* _this,
	NetworkIdentifier* a2,
	int a3,
	string a4,
	int a5) {

	//客户端协议号
	int clientProtocol = *((_DWORD*)a2 + 12);
	//服务端协议号
	int serverProtocol = SharedConstants::NetworkProtocolVersion;
	
	string title = getLanguage(m_replace(a4,"disconnectionScreen.",""));
	title = m_replace(title, "{clientProtocol}", to_string(clientProtocol));
	title = m_replace(title, "{serverProtocol}", to_string(serverProtocol));

	
	return original(_this, a2, a3, title, a5);
}

